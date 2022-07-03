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
static void R_CSI20_Create(void);

/***********************************************************************************************************************
* Function Name: R_SAU1_Create
* Description  : This function initializes the SAU1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU1_Create(void)
{
    SAU1EN = 1U;    /* supply SAU1 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS1 = _0000_SAU_CK00_FCLK_0 | _0000_SAU_CK01_FCLK_0;
    R_CSI20_Create();
}

/***********************************************************************************************************************
* Function Name: R_CSI20_Create
* Description  : This function initializes the CSI20 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void R_CSI20_Create(void)
{
    ST1 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable CSI20 */
    CSIMK20 = 1U;    /* disable INTCSI20 interrupt */
    CSIIF20 = 0U;    /* clear INTCSI20 interrupt flag */
    /* Set INTCSI20 low priority */
    CSIPR120 = 1U;
    CSIPR020 = 1U;
    SIR10 = _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR10 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
    SCR10 = _C000_SAU_RECEPTION_TRANSMISSION | _3000_SAU_TIMING_4 | _0000_SAU_MSB | _0007_SAU_LENGTH_8;
    SDR10 = _1E00_CSI20_DIVISOR;
    SO1 &= ~_0100_SAU_CH0_CLOCK_OUTPUT_1;    /* CSI20 clock initial level */
    SO1 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;    /* CSI20 SO initial level */
    SOE1 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable CSI20 output */
    /* Set SI20 pin */
    PM1 |= 0x10U;
    /* Set SO20 pin */
    PMC1 &= 0xF7U;
    P1 |= 0x08U;
    PM1 &= 0xF7U;
    /* Set SCK20 pin */
    P1 |= 0x20U;
    PM1 &= 0xDFU;
}

/***********************************************************************************************************************
* Function Name: R_CSI20_Start
* Description  : This function starts the CSI20 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI20_Start(void)
{
    SO1 &= ~_0100_SAU_CH0_CLOCK_OUTPUT_1;   /* CSI20 clock initial level */
    SO1 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;           /* CSI20 SO initial level */
    SOE1 |= _0001_SAU_CH0_OUTPUT_ENABLE;           /* enable CSI20 output */
    SS1 |= _0001_SAU_CH0_START_TRG_ON;             /* enable CSI20 */
}

/***********************************************************************************************************************
* Function Name: R_CSI20_Stop
* Description  : This function stops the CSI20 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI20_Stop(void)
{
    CSIMK20 = 1U;    /* disable INTCSI20 interrupt */
    ST1 |= _0001_SAU_CH0_STOP_TRG_ON;        /* disable CSI20 */
    SOE1 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable CSI20 output */
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
        CSIIF20 = SIO_FALSE;

        TxBuf = spiTransmitBuffer[i];

        SIO20 = TxBuf;

        /* Transmit&receive completion check */
        TRxWait = SPI_RX_WAIT;

        while (CSIIF20 == SIO_FALSE)   /* Loop for receive completion */
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
        RxBuf = SIO20;        /* Receive data */
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


