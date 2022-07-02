/**
 * @file   spi.c
 * @author Michel Catudal
 * @date   15 June 2022
 * @version 0.1
 * @brief  SPI Support for CAN driver
 *
*/
#include "system.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "dcan.h"
#include "spi.h"
#include "string.h"

//! SPI Transmit buffer
uint8_t spiTransmitBuffer[SPI_DEFAULT_BUFFER_LENGTH];

//! SPI Receive buffer
uint8_t spiReceiveBuffer[SPI_DEFAULT_BUFFER_LENGTH];

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0000_SAU_CK00_FCLK_0 | _0000_SAU_CK01_FCLK_0;
    R_CSI00_Create();
}

/***********************************************************************************************************************
* Function Name: R_CSI00_Create
* Description  : This function initializes the CSI00 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Create(void)
{
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable CSI00 */
    CSIMK00 = 1U;    /* disable INTCSI00 interrupt */
    CSIIF00 = 0U;    /* clear INTCSI00 interrupt flag */
    /* Set INTCSI00 low priority */
    CSIPR100 = 1U;
    CSIPR000 = 1U;
    SIR00 = _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
    SCR00 = _C000_SAU_RECEPTION_TRANSMISSION | _3000_SAU_TIMING_4 | _0000_SAU_MSB | _0007_SAU_LENGTH_8;
    SDR00 = _1E00_CSI00_DIVISOR;
    SO0 &= ~_0100_SAU_CH0_CLOCK_OUTPUT_1;    /* CSI00 clock initial level */
    SO0 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;    /* CSI00 SO initial level */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable CSI00 output */
    /* Set SI00 pin */
    PM5 |= 0x01U;
    /* Set SO00 pin */
    P5 |= 0x02U;
    PM5 &= 0xFDU;
    /* Set SCK00 pin */
    P3 |= 0x01U;
    PM3 &= 0xFEU;
}

/***********************************************************************************************************************
* Function Name: R_CSI00_Start
* Description  : This function starts the CSI00 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Start(void)
{
    SO0 &= ~_0100_SAU_CH0_CLOCK_OUTPUT_1;   /* CSI00 clock initial level */
    SO0 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;           /* CSI00 SO initial level */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;           /* enable CSI00 output */
    SS0 |= _0001_SAU_CH0_START_TRG_ON;             /* enable CSI00 */
}

/***********************************************************************************************************************
* Function Name: R_CSI00_Stop
* Description  : This function stops the CSI00 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI00_Stop(void)
{
    CSIMK00 = 1U;    /* disable INTCSI00 interrupt */
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;        /* disable CSI00 */
    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable CSI00 output */
}

/*******************************************************************************************************************//**
 * Function Name: SPI_wait
 * @brief     loop timer processing.
 * @param     time Timer value.
 * @return    none.
 * @details   loop timer processing.
***********************************************************************************************************************/
void SPI_wait(uint16_t time)
{
    while (0 != time) time--;
}

/************************************************************************************************
* Function Name : SPI_Send_Receive
* @brief       : Consecutively receives data for the specified number of bytes.
* @param[in]   : ByteCount     ; Reception byte count
* @Return      : SPI_OK        ; Successful operation
*              : SPI_ERR_HARD  ; Hardware error
*************************************************************************************************/
int16_t SPI_Send_Receive(uint16_t ByteCount)
{
    uint16_t TRxWait;  /* Receive waiting counter */
    uint8_t RxBuf,TxBuf;
    uint16_t i;

    DRV_CANFDSPI_CS = 0;

    for (i = 0; i < ByteCount; i++)            /* Loop for tx byte  */
    {
        /* Clear IR bit of IR register */
        SIO_RXNEXT = SIO_FALSE;

        TxBuf = spiTransmitBuffer[i];

        SIO00 = TxBuf;

        /* Transmit&receive completion check */
        TRxWait = SPI_RX_WAIT;

        while (SIO_RXNEXT == SIO_FALSE)   /* Loop for receive completion */
        {
            TRxWait--;
            if (TRxWait == 0)           /* Transmit&receive waiting time over */
            {
                DRV_CANFDSPI_CS = 1;
                return SPI_ERR_HARD;
            }
            SPI_wait(SPI_T_SAU_WAIT);
        }

        /* Stores the received data from SDR to pData buffer. */
        RxBuf = SIO00;        /* Receive data */
        spiReceiveBuffer[i] = RxBuf;
    }

    DRV_CANFDSPI_CS = 1;

    return SPI_OK;
}

/*****************************************************************************
 Section: SPI Access Functions
*****************************************************************************/
int16_t SPI_ReadByte(uint16_t address, uint8_t *rxd)
{
    int16_t spi_status;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t)((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t)(address & 0xFF);
    spiTransmitBuffer[3] = 0xFF;

    spi_status = SPI_Send_Receive((uint16_t) 3);

    // Update data
    *rxd = spiReceiveBuffer[2];

    return spi_status;
}

int16_t SPI_WriteByte(uint16_t address, uint8_t txd)
{
    int16_t spi_status;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t)((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t)(address & 0xFF);
    spiTransmitBuffer[2] = txd;

    spi_status = SPI_Send_Receive((uint16_t) 3);

    return spi_status;
}

int16_t SPI_ReadWord(uint16_t address, uint32_t *rxd)
{
    uint8_t i;
    uint32_t x;
    int16_t spi_status = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t)((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t)(address & 0xFF);
    spiTransmitBuffer[2] = 0xFF;
    spiTransmitBuffer[3] = 0xFF;
    spiTransmitBuffer[4] = 0xFF;
    spiTransmitBuffer[5] = 0xFF;

    spi_status = SPI_Send_Receive((uint16_t) 6);
    if (spi_status < 0)
    {
        return spi_status;
    }

    // Update data
    *rxd = 0;
    for (i = 2; i < 6; i++)
    {
        x = (uint32_t)spiReceiveBuffer[i];
        *rxd += x << ((i - 2) * 8);
    }

    return spi_status;
}

int16_t SPI_WriteWord(uint16_t address, uint32_t txd)
{
    uint8_t i;
    int16_t spi_status;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t)((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t)(address & 0xFF);

    // Split word into 4 bytes and add them to buffer
    for (i = 0; i < 4; i++)
    {
        spiTransmitBuffer[i + 2] = (uint8_t)((txd >> (i * 8)) & 0xFF);
    }

    spi_status = SPI_Send_Receive((uint16_t) 6);

    return spi_status;
}

int16_t SPI_ReadHalfWord(uint16_t address, uint16_t *rxd)
{
    uint8_t i;
    uint32_t x;
    int16_t spi_status = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t)((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t)(address & 0xFF);
    spiTransmitBuffer[2] = 0xFF;
    spiTransmitBuffer[3] = 0xFF;

    spi_status = SPI_Send_Receive((uint16_t) 4);
    if (spi_status < 0)
    {
        return spi_status;
    }

    // Update data
    *rxd = 0;
    for (i = 2; i < 4; i++)
    {
        x = (uint32_t)spiReceiveBuffer[i];
        *rxd += x << ((i - 2) * 8);
    }

    return spi_status;
}

int16_t SPI_WriteHalfWord(uint16_t address, uint16_t txd)
{
    uint8_t i;
    uint16_t spiTransferSize = 4;
    int16_t spi_status = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t)((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t)(address & 0xFF);

    // Split word into 2 bytes and add them to buffer
    for (i = 0; i < 2; i++)
    {
        spiTransmitBuffer[i + 2] = (uint8_t)((txd >> (i * 8)) & 0xFF);
    }

    spi_status = SPI_Send_Receive(spiTransferSize);

    return spi_status;
}
int16_t SPI_ReadByteArray(uint16_t address, uint8_t *rxd, uint16_t nBytes)
{
    uint16_t i;
    uint16_t spiTransferSize = nBytes + 2;
    int16_t spi_status;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t)((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t)(address & 0xFF);
    memset(&spiTransmitBuffer[2],0xFF,spiTransferSize-2);

    // Clear data
    for (i = 2; i < spiTransferSize; i++)
    {
        spiTransmitBuffer[i] = 0;
    }

    spi_status = SPI_Send_Receive(spiTransferSize);

    // Update data
    for (i = 0; i < nBytes; i++)
    {
        rxd[i] = spiReceiveBuffer[i + 2];
    }

    return spi_status;
}

int16_t SPI_ReadWordArray(uint16_t address, uint32_t *rxd, uint16_t nWords)
{
    uint16_t i, j, n;
    REG_t w;
    uint16_t spiTransferSize = nWords * 4 + 2;
    int16_t spi_status = 0;

    // Compose command
    spiTransmitBuffer[0] = (cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF);
    spiTransmitBuffer[1] = address & 0xFF;
    memset(&spiTransmitBuffer[2],0xFF,spiTransferSize-2);

    // Clear data
    for (i = 2; i < spiTransferSize; i++)
    {
        spiTransmitBuffer[i] = 0;
    }

    spi_status = SPI_Send_Receive(spiTransferSize);
    if (spi_status < 0)
    {
        return spi_status;
    }

    // Convert Byte array to Word array
    n = 2;
    for (i = 0; i < nWords; i++)
    {
        w.word = 0;
        for (j = 0; j < 4; j++, n++)
        {
            w.byte[j] = spiReceiveBuffer[n];
        }
        rxd[i] = w.word;
    }

    return spi_status;
}

int16_t SPI_WriteWordArray(uint16_t address, uint32_t *txd, uint16_t nWords)
{
    uint16_t i, j, n;
    REG_t w;
    uint16_t spiTransferSize = nWords * 4 + 2;
    int16_t spi_status = 0;

    // Compose command
    spiTransmitBuffer[0] = (cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF);
    spiTransmitBuffer[1] = address & 0xFF;

    // Convert ByteArray to word array
    n = 2;
    for (i = 0; i < nWords; i++)
    {
        w.word = txd[i];
        for (j = 0; j < 4; j++, n++)
        {
            spiTransmitBuffer[n] = w.byte[j];
        }
    }

    spi_status = SPI_Send_Receive(spiTransferSize);

    return spi_status;
}

int16_t SPI_WriteByteArray(uint16_t address, uint8_t *txd, uint16_t nBytes)
{
    uint16_t i;
    uint16_t spiTransferSize = nBytes + 2;
    int16_t spi_status = 0;

    // Compose command
    spiTransmitBuffer[0] = (uint8_t)((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
    spiTransmitBuffer[1] = (uint8_t)(address & 0xFF);

    // Add data
    for (i = 2; i < spiTransferSize; i++)
    {
        spiTransmitBuffer[i] = txd[i - 2];
    }

    spi_status = SPI_Send_Receive(spiTransferSize);

    return spi_status;
}


