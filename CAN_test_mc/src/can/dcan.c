/**
 * @file   dcan.c
 * @author Michel Catudal
 * @date   16 June 2022
 * @version 0.1
 * @brief  CAN interface
 *
*/

/* This test code uses standard CAN
   Once that is shown to work, will add support for extended
*/

#include "system.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "dcan.h"
#include "pcan.h"
#include "spi.h"

uint8_t ucDCAN_iCAN_Speed = 0;  /* Defaults to 250k #*/

static int8_t iDCAN_iSetFilterAndMask(CAN_FIFO_CHANNEL FIFO, CAN_FILTER Filter);
static int8_t iDCAN_iFilterObjectConfigure(CAN_FILTER filter, CAN_FILTEROBJ_ID *id);
static int8_t iDCAN_iFilterMaskConfigure(CAN_FILTER filter, CAN_MASKOBJ_ID *mask);
static int8_t iDCAN_iSetCAN_Speed(void);
static int8_t iDCAN_iTransmitChannelLoad(CAN_FIFO_CHANNEL channel, CAN_TX_MSGOBJ *txObj,
                                         uint8_t *txd, uint32_t txdNumBytes);
static int8_t iDCAN_iTransmitChannelUpdate(CAN_FIFO_CHANNEL channel);
static int16_t iDCAN_iTransmitChannelEventGet(CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT *flags);

/*****************************************************************************
 *      @brief   Initialize the CAN port
 *
 *      @param[in] None
 *
 *      @return 0 or success, negative value on error
******************************************************************************/
int8_t iDCAN_eInit(void)
{
    int8_t spiTransferError;
    uint32_t readBackValue, i;
    uint16_t TRxWait;  /* Receive waiting counter */
    uint8_t d = 0;
    uint16_t address;

    //----------------------------------- Request configuration
    // Write
    spiTransferError = SPI_WriteByte(cREGADDR_CiCON + 3,  CAN_CONFIGURATION_MODE | (1 << 3)); // Request configuration mode, abort all transmissions
    if (spiTransferError != SPI_OK) return spiTransferError;

    TRxWait = ((uint16_t)(2000));     /*  Wait (2 ms max) until requested mode is reached */

    while (d != CAN_CONFIGURATION_MODE)
    {
        TRxWait--;

        if (!TRxWait)
        {
            return SPI_ERR_HARD;      /* Timed out */
        }

        spiTransferError = SPI_ReadByte(cREGADDR_CiCON + 2, &d);
        if (spiTransferError != SPI_OK) return spiTransferError;

        // Get Opmode bits
        d = (d >> 5) & 7;
    }

    spiTransferError = SPI_Send_Receive((uint16_t)2);

    if (spiTransferError != SPI_OK) return spiTransferError;

    // We write and the read back 2517 RAM at address 0x400
    spiTransferError = SPI_OK;

    for (i = 1; i != 0; i <<= 1)
    {
        spiTransferError = SPI_WriteWord(0x400, i);
        if (spiTransferError != SPI_OK) return spiTransferError;

        spiTransferError = SPI_ReadWord(0x400, &readBackValue);
        if (spiTransferError != SPI_OK) return spiTransferError;

        if (readBackValue != i) return SPI_ERR_HARD;
    }

    /* Enable PLL for 4Mhz XTAL for the production board
     * but not for the click board used for testing
     * SYSCLK will be 40 Mhz
     * Oscillator Enabled  and Divide by 1
     * Output clock divide by 10
    */
    REG_OSC osc;
    osc.word = 0;

    //    osc.bF.PllEnable = 1; /* The production board has a xtal */
    osc.bF.CLKODIV = OSC_CLKO_DIV10;   /* Clock output divided by 10 */

    // Write
    spiTransferError = SPI_WriteByte(cREGADDR_OSC, osc.byte[0]);

    iDCAN_iSetCAN_Speed();

    //----------------------------------- Reset RAM
    for (address = 0x400; address < 0xC00; address += 4)
    {
        spiTransferError = SPI_WriteWord(address, 0);
        if (spiTransferError != SPI_OK) return spiTransferError;
    }

    d = 0;

    // Configure transmit and receive interrupt for TXQ and FIFO 2
    spiTransferError = SPI_WriteByte(cREGADDR_IOCON + 3, d);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    // TXQ Configuration: 8 messages, 8 byte maximum payload, high priority
    REG_CiTXQCON ciTxqCon;
    ciTxqCon.word = 0UL;

    ciTxqCon.txBF.TxAttempts = 1;    /* 3 Retries */
    ciTxqCon.txBF.TxPriority = 1;    /* High priority */
    ciTxqCon.txBF.FifoSize = 31;
    ciTxqCon.txBF.PayLoadSize = CAN_PLSIZE_8;
    ciTxqCon.txBF.TxEnable = 1;

    spiTransferError = SPI_WriteWord(cREGADDR_CiTXQCON, ciTxqCon.word);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    /*
       CAN_FIFO_CH0 is reserved for TXQ

       CAN_FIFO_CH1 is used to transmit message

       CAN_FIFO_CH2 is used for the Reception of messages
       We use one filter for DiagOnCan message, the rest are used by J1939 in the app
       */

    // FIFO 1: Transmit FIFO; 8 messages, 8 bytes maximum payload, low priority
    REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = 0UL;

    ciFifoCon.txBF.TxAttempts = 1;    /* 3 Retries */
    ciFifoCon.txBF.TxPriority = 1;    /* High priority */
    ciFifoCon.txBF.TxEnable = 1;
    ciFifoCon.txBF.FifoSize = 31;
    ciFifoCon.txBF.PayLoadSize = CAN_PLSIZE_8;
    ciFifoCon.txBF.TxAttemptIE = 1;

    address = cREGADDR_CiFIFOCON + (CAN_FIFO_CH1 * CiFIFO_OFFSET);

    spiTransferError = SPI_WriteWord(address, ciFifoCon.word);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    ciFifoCon.word = 0;

    ciFifoCon.rxBF.FifoSize = 31;
    ciFifoCon.rxBF.PayLoadSize = CAN_PLSIZE_8;
    ciFifoCon.rxBF.RxNotEmptyIE = 1;    /* Interrupt Enabled for FIFO not Empty  */

    address = cREGADDR_CiFIFOCON + (CAN_FIFO_CH2 * CiFIFO_OFFSET);

    spiTransferError = SPI_WriteWord(address, ciFifoCon.word);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    /* The receive data is at CAN_FIFO_CH1
       We only have 32 filters, we use the first filter for DiagOnCan Messages
       we use the next 21 filters for individual J1939 messages
       Most of those come fast, 20 to 500 msecs
       The ones that share filters come around 1 sec
    */
    spiTransferError = iDCAN_iSetFilterAndMask(CAN_FIFO_CH1, CAN_FILTER0);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    REG_CiINTENABLE intEnables;
    intEnables.word = 0;

    //----------------------------------- Activate interrupts (C1INT page 34)
    intEnables.IE.RXIE = 1;     /* Transmit FIFO Interrupt Enable */
    intEnables.IE.TXIE = 1;     /* Receive FIFO Interrupt Enable */
    intEnables.IE.TXATIE = 1;   /*  Transmit Attempt Interrupt Enable bit */

    // Write
    spiTransferError = SPI_WriteHalfWord(cREGADDR_CiINTFLAG, intEnables.word);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    // Select classic CAN2B mode
    spiTransferError = SPI_WriteByte(cREGADDR_CiCON + 3,  CAN_CLASSIC_MODE | (1 << 3)); // Request classic mode, abort all transmissions
    if (spiTransferError != SPI_OK) return spiTransferError;

    TRxWait = ((uint16_t)(2000));     /*  Wait (2 ms max) until requested mode is reached */

    while (d != CAN_CLASSIC_MODE)
    {
        TRxWait--;

        if (!TRxWait)
        {
            return SPI_ERR_HARD;      /* Timed out */
        }

        spiTransferError = SPI_ReadByte(cREGADDR_CiCON + 2, &d);
        if (spiTransferError != SPI_OK) return spiTransferError;
        // Get Opmode bits
        d = (d >> 5) & 7;
    }

    return SPI_OK;
}

/*****************************************************************************
 *      @brief  Set Filter and Mask for Standard CAN messages
 *
 *      @param[in] FIFO Number
 *      @param[in] Filter Number
 *
 *      @return None
*****************************************************************************/
static int8_t iDCAN_iSetFilterAndMask(CAN_FIFO_CHANNEL FIFO, CAN_FILTER Filter)
{
    int8_t spiTransferError;
    REG_CiFLTOBJ fObj;
    REG_CiMASK mObj;
    REG_CiFLTCON_BYTE fCtrl;

    // Setup RX Filter
    fObj.word = 0;
    fObj.bF.SID = DuiPCAN_eRX_DiagnosticPacketID;

    spiTransferError = iDCAN_iFilterObjectConfigure(Filter, &fObj.bF);
    if (spiTransferError != SPI_OK) return spiTransferError;

    // Setup RX Mask
    mObj.word = 0;
    mObj.bF.MIDE = 1; // Only allow standard IDs
    mObj.bF.MSID = 0x7FF;
    spiTransferError = iDCAN_iFilterMaskConfigure(Filter, &mObj.bF);
    if (spiTransferError != SPI_OK) return spiTransferError;

    // Link FIFO and Filter
    fCtrl.bF.Enable = 1;
    // Link
    fCtrl.bF.BufferPointer = FIFO;

    return SPI_WriteByte(cREGADDR_CiFLTCON + Filter, fCtrl.byte);
}

static int8_t iDCAN_iFilterObjectConfigure(CAN_FILTER filter, CAN_FILTEROBJ_ID *id)
{
    uint16_t a;
    REG_CiFLTOBJ fObj;

    // Setup
    fObj.word = 0;
    fObj.bF = *id;
    a = cREGADDR_CiFLTOBJ + (filter * CiFILTER_OFFSET);

    return SPI_WriteWord(a, fObj.word);
}

static int8_t iDCAN_iFilterMaskConfigure(CAN_FILTER filter, CAN_MASKOBJ_ID *mask)
{
    uint16_t a;
    REG_CiMASK mObj;

    // Setup
    mObj.word = 0;
    mObj.bF = *mask;
    a = cREGADDR_CiMASK + (filter * CiFILTER_OFFSET);

    return SPI_WriteWord(a, mObj.word);
}

static int8_t iDCAN_iSetCAN_Speed(void)
{
    int8_t spiTransferError;
    REG_CiNBTCFG ciNbtcfg;
    REG_CiDBTCFG ciDbtcfg;
    REG_CiTDC ciTdc;

    ciNbtcfg.word = 0;
    ciDbtcfg.word = 0;
    ciTdc.word = 0;

    // Configure Bit time and sample point
    ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;

    ciNbtcfg.bF.BRP = 0;
    // Data BR
    ciDbtcfg.bF.BRP = 0;
    ciDbtcfg.bF.TSEG1 = 6;
    ciDbtcfg.bF.TSEG2 = 1;
    ciDbtcfg.bF.SJW = 1;
    ciDbtcfg.bF.SJW = 1;
    // SSP
    ciTdc.bF.TDCOffset = 7;
    ciTdc.bF.TDCValue = 0;

//    if (ucDCAN_iCAN_Speed == ~ucDCAN_iInvCAN_Speed) CAN_Speed = ucDCAN_iCAN_Speed;
//    else CAN_Speed = ucDCAN_iCAN_Speed;

    if (ucDCAN_iCAN_Speed == 1)    /* 500k */
    {
        ciNbtcfg.bF.TSEG1 = 62;
        ciNbtcfg.bF.TSEG2 = 15;
        ciNbtcfg.bF.SJW = 15;
    }
    else if (ucDCAN_iCAN_Speed == 2)    /* 1M */
    {
        ciNbtcfg.bF.TSEG1 = 30;
        ciNbtcfg.bF.TSEG2 = 7;
        ciNbtcfg.bF.SJW = 7;
    }
    else           /* 250k */
    {
        ciNbtcfg.bF.TSEG1 = 126;
        ciNbtcfg.bF.TSEG2 = 31;
        ciNbtcfg.bF.SJW = 31;
    }

    // Write Bit time registers
    spiTransferError = SPI_WriteWord(cREGADDR_CiNBTCFG, ciNbtcfg.word);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    // Write Transmitter Delay Compensation
#ifdef REV_A
    ciTdc.bF.TDCOffset = 0;
    ciTdc.bF.TDCValue = 0;
#endif

    spiTransferError = SPI_WriteWord(cREGADDR_CiTDC, ciTdc.word);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    // Write Bit time registers
    spiTransferError = SPI_WriteWord(cREGADDR_CiDBTCFG, ciDbtcfg.word);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    spiTransferError = SPI_WriteWord(cREGADDR_CiTDC, ciTdc.word);

    return spiTransferError;
}

#pragma diag_suppress = Pe550

static int8_t iDCAN_iTransmitChannelLoad(CAN_FIFO_CHANNEL channel, CAN_TX_MSGOBJ *txObj,
                                         uint8_t *txd, uint32_t txdNumBytes)
{
    uint16_t a;
    uint32_t fifoReg[3];
    REG_CiFIFOCON ciFifoCon;
    REG_CiFIFOSTA ciFifoSta;
    REG_CiFIFOUA ciFifoUa;
    int8_t spiTransferError;

    // Get FIFO registers
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = SPI_ReadWordArray(a, fifoReg, 3);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    // Check that it is a transmit buffer
    ciFifoCon.word = fifoReg[0];
    if (!ciFifoCon.txBF.TxEnable)
    {
        return -2;
    }

    // Check that DLC is big enough for data
    if (txdNumBytes > 8)
    {
        return -3;
    }

    // Get status
    ciFifoSta.word = fifoReg[1];

    // Get address
    ciFifoUa.word = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
    a = 4 * ciFifoUa.bF.UserAddress;
#else
    a = ciFifoUa.bF.UserAddress;
#endif
    a += cRAMADDR_START;

    uint8_t txBuffer[MAX_MSG_SIZE];

    txBuffer[0] = txObj->byte[0]; //not using 'for' to reduce no of instructions
    txBuffer[1] = txObj->byte[1];
    txBuffer[2] = txObj->byte[2];
    txBuffer[3] = txObj->byte[3];

    txBuffer[4] = txObj->byte[4];
    txBuffer[5] = txObj->byte[5];
    txBuffer[6] = txObj->byte[6];
    txBuffer[7] = txObj->byte[7];

    uint8_t i;
    for (i = 0; i < txdNumBytes; i++)
    {
        txBuffer[i + 8] = txd[i];
    }

    // Make sure we write a multiple of 4 bytes to RAM
    uint16_t n = 0;
    uint8_t j = 0;

    if (txdNumBytes % 4)
    {
        // Need to add bytes
        n = 4 - (txdNumBytes % 4);
        i = txdNumBytes + 8;

        for (j = 0; j < n; j++)
        {
            txBuffer[i + 8 + j] = 0;
        }
    }

    spiTransferError = SPI_WriteByteArray(a, txBuffer, txdNumBytes + 8 + n);
    if (spiTransferError  != SPI_OK)
    {
        return spiTransferError;
    }

    // Set UINC and TXREQ
    spiTransferError = iDCAN_iTransmitChannelUpdate(channel);

    return spiTransferError;
}

#pragma diag_default = Pe550


/*****************************************************************************
 *      @brief  Add DiagOnCan message to TXQUEUE
 *
 *      @param[in] Pointer to buffer
 *      @param[in] Number of bytes to send
 *
 *      @return Success if 0, 1 if busy, -1 on error
*****************************************************************************/
int8_t iDCAN_iSendStdCANMsg(uint16_t CANID, uint8_t *txd, uint8_t txdNumBytes)
{
    CAN_TX_MSGOBJ txObj;
    int8_t spiTransferError = 1;

    if (txdNumBytes > 8) txdNumBytes = 8;

    // Configure transmit message
    txObj.word[0] = 0;
    txObj.word[1] = 0;

    txObj.bF.id.SID = CANID;
    txObj.bF.ctrl.DLC = txdNumBytes; // 8 data bytes

    // Check that FIFO is not full
    // DRV_CANFDSPI_INT0 queries the input pin of the MCU, which is connected to nINT0
    if (!DRV_CANFDSPI_INT0)
    {
        // Load message and transmit
        spiTransferError = iDCAN_iTransmitChannelLoad(CAN_TXQUEUE_CH0, &txObj, txd, txdNumBytes);
    }

    return spiTransferError;
}

/*****************************************************************************
 *      @brief  Extended CAN
 *
 *      @param[in] PGN
 *      @param[in] Pointer to buffer
 *      @param[in] Number of bytes to send
 *
 *      @return Success if 0, Failure if negative returned value
*****************************************************************************/
int16_t iDCAN_iSendExtCANMsg(uint32_t CANID,uint8_t *txd, uint8_t txdNumBytes)
{
    int16_t status = -1;
    CAN_TX_MSGOBJ txObj;
    CAN_TX_FIFO_EVENT txFlags;

    // Configure transmit message
    txObj.word[0] = 0;
    txObj.word[1] = 0;

    CANID &= 0x1FFFF00UL;
    CANID |= 0x17;

    txObj.bF.id.EID = CANID;

    txObj.bF.ctrl.DLC = CAN_DLC_8;
    txObj.bF.ctrl.IDE = 1;

    // Check that FIFO is not full
    iDCAN_iTransmitChannelEventGet(CAN_FIFO_CH1, &txFlags);

    if (txFlags & CAN_TX_FIFO_NOT_FULL_EVENT) /* Load message and transmit n*/
        status = iDCAN_iTransmitChannelLoad(CAN_FIFO_CH1, &txObj, txd, txdNumBytes);

    return status;
}

static int8_t iDCAN_iTransmitChannelUpdate(CAN_FIFO_CHANNEL channel)
{
    uint16_t a;
    REG_CiFIFOCON ciFifoCon;
    int8_t spiTransferError;

    // Set UINC
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
    ciFifoCon.word = 0;
    ciFifoCon.txBF.UINC = 1;

    // Set TXREQ
    ciFifoCon.txBF.TxRequest = 1;

    spiTransferError = SPI_WriteByte(a, ciFifoCon.byte[1]);

    return spiTransferError;
}

// *****************************************************************************
// *****************************************************************************
// Section: Receive FIFO Events

int8_t iDCAN_iReceiveChannelEventGet(CAN_FIFO_CHANNEL channel, CAN_RX_FIFO_EVENT *flags)
{
    int8_t spiTransferError;
    uint16_t a = 0;

    if (channel == CAN_TXQUEUE_CH0) return -100;

    // Read Interrupt flags
    REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spiTransferError = SPI_ReadByte(a, &ciFifoSta.byte[0]);
    if (spiTransferError) return spiTransferError;

    // Update data
    *flags = (CAN_RX_FIFO_EVENT)(ciFifoSta.byte[0]&CAN_RX_FIFO_ALL_EVENTS);

    return spiTransferError;
}

int8_t iDCAN_iReceiveMessageGet(CAN_FIFO_CHANNEL channel, CAN_RX_MSGOBJ *rxObj,
                                uint8_t *rxd, uint8_t nBytes)
{
    uint8_t n = 0;
    uint8_t i = 0;
    uint16_t a;
    uint32_t fifoReg[3];
    REG_CiFIFOCON ciFifoCon;
    REG_CiFIFOUA ciFifoUa;
    int8_t spiTransferError = 0;

    // Get FIFO registers
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

    spiTransferError = SPI_ReadWordArray(a, fifoReg, 3);
    if (spiTransferError)
    {
        return -1;
    }

    // Check that it is a receive buffer
    ciFifoCon.word = fifoReg[0];
    if (ciFifoCon.txBF.TxEnable)
    {
        return -2;
    }

    // Get address
    ciFifoUa.word = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
    a = 4 * ciFifoUa.bF.UserAddress;
#else
    a = ciFifoUa.bF.UserAddress;
#endif
    a += cRAMADDR_START;

    // Number of bytes to read
    n = nBytes + 8; // Add 8 header bytes

    // Make sure we read a multiple of 4 bytes from RAM
    if (n % 4)
    {
        n = n + 4 - (n % 4);
    }

    // Read rxObj using one access
    uint8_t ba[MAX_MSG_SIZE];

    if (n > MAX_MSG_SIZE)
    {
        n = MAX_MSG_SIZE;
    }

    spiTransferError = SPI_ReadByteArray(a, ba, n);
    if (spiTransferError)
    {
        return -3;
    }

    // Assign message header
    REG_t myReg;

    myReg.byte[0] = ba[0];
    myReg.byte[1] = ba[1];
    myReg.byte[2] = ba[2];
    myReg.byte[3] = ba[3];
    rxObj->word[0] = myReg.word;

    myReg.byte[0] = ba[4];
    myReg.byte[1] = ba[5];
    myReg.byte[2] = ba[6];
    myReg.byte[3] = ba[7];
    rxObj->word[1] = myReg.word;

    rxObj->word[2] = 0;

    // Assign message data
    for (i = 0; i < nBytes; i++)
    {
        rxd[i] = ba[i + 8];
    }

    // UINC channel
    ciFifoCon.word = 0;

    // Set UINC
    a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
    ciFifoCon.rxBF.UINC = 1;

    // Write byte
    spiTransferError = SPI_WriteByte(a, ciFifoCon.byte[1]);
    if (spiTransferError)
    {
        return -4;
    }

    return spiTransferError;
}

static int16_t iDCAN_iTransmitChannelEventGet(CAN_FIFO_CHANNEL channel, CAN_TX_FIFO_EVENT *flags)
{
    int16_t spi_status = 0;
    uint16_t a = 0;

    // Read Interrupt flags
    REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;
    a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

    spi_status = SPI_ReadByte(a, &ciFifoSta.byte[0]);
    if (spi_status < 0)
    {
        return -1;
    }

    // Update data
    *flags = (CAN_TX_FIFO_EVENT)(ciFifoSta.byte[0]&CAN_TX_FIFO_ALL_EVENTS);

    return spi_status;
}

