/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
#ifndef SIO_H
#define SIO_H

/*----------- Definitions of return value ------------*/
#define SIO_OK         ((int16_t)( 0))         /* Successful operation                     */
#define SIO_ERR_PARAM  ((int16_t)(-1))         /* Parameter error                          */
#define SIO_ERR_HARD   ((int16_t)(-2))         /* Hardware error                           */
#define SIO_ERR_OTHER  ((int16_t)(-7))         /* Other error                              */

/*------- Definitions of software timer value --------*/
/* Transmit&receive waiting time */
#define SIO_TX_WAIT     ((uint16_t)(50000))     /* SIO transmission completion waiting time 50000* 1us =  50ms  */
#define SIO_RX_WAIT     ((uint16_t)(50000))     /* SIO reception completion waiting time    50000* 1us =  50ms  */
#define SIO_DMA_TX_WAIT ((uint16_t)(50000))     /* DMA transmission completion waiting time 50000* 1us =  50ms  */
#define SIO_DMA_RX_WAIT ((uint16_t)(50000))     /* DMA reception completion waiting time    50000* 1us =  50ms  */

/* Access time */
#define SIO_T_SIO_WAIT  ((uint16_t)MTL_T_1US)   /* SIO transmission&reception completion waiting polling time   */

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Init_Driver
 * @brief     Driver initialization processing.
 * @param     none
 * @retval    SIO_OK Successful operation.
 * @details   Initializes the driver. Disables the serial I/O function and set the pin in the port.
 * @note      Performs the following processing, considering the previous use conditions.
***********************************************************************************************************************/
int16_t R_SIO_Init_Driver(void);

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Disable
 * @brief     Serial I/O disable setting processing.
 * @param     none
 * @retval    SIO_OK Successful operation.
 * @details   Disables the serial I/O function and sets the pins to function as ports.
 * @note      Performs the following processing, considering the previous use conditions.
***********************************************************************************************************************/
int16_t R_SIO_Disable(void);

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Enable
 * @brief     Serial I/O enable setting processing.
 * @param     BrgData Bit rate setting value.
 * @retval    SIO_OK Successful operation.
 * @details   Enables the serial I/O function and sets the baud rate.
 * @note      Perform processing according to the initial setting procedure of the master send
 * and master send / receive as described in the hardware manual.
***********************************************************************************************************************/
int16_t R_SIO_Enable(uint8_t BrgData);

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Open_Port
 * @brief     Serial I/O open setting processing.
 * @param     none
 * @retval    SIO_OK Successful operation.
 * @details   Sets the pin used for serial I/O to "open" (input state).
 * @note      Prepared to connect and disconnect removable media. Use this function before
***********************************************************************************************************************/
int16_t R_SIO_Open_Port(void);


/*******************************************************************************************************************//**
 * Function Name: R_SIO_Tx_Data
 * @brief     Serial I/O data transmit processing.
 * @param     TxCnt Number of transmitted bytes.
 * @param     pData Transmit data storage buffer pointer.
 * @retval    SIO_OK       Successful operation.
 * @retval    SIO_ERR_HARD Hardware error.
 * @details   Serial I/O data transmit processing.
 * @note      Makes the initialization settings, following serial I/O enable setting processing, according to
 * the initial setting procedure for master transmission described in the hardware manual.
***********************************************************************************************************************/
int16_t R_SIO_Tx_Data(uint16_t TxCnt, uint8_t FAR* pData);

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Rx_Data
 * @brief     Serial I/O data receive processing.
 * @param     RxCnt Number of received bytes.
 * @param     pData Receive data storage buffer pointer.
 * @retval    SIO_OK       Successful operation.
 * @retval    SIO_ERR_HARD Hardware error.
 * @details   Receives a specified number of data and stores it in pData.
 * @note      Makes the initialization settings, following serial I/O enable setting processing,
 * according to the initial setting procedure for master transmission/reception described in the hardware manual.
***********************************************************************************************************************/
int16_t R_SIO_Rx_Data(uint16_t RxCnt, uint8_t FAR* pData);

/*******************************************************************************************************************//**
 * Function Name: R_SIO_TRx_Data
 * @brief     Serial I/O data transmit/receive processing.
 * @param     TRxCnt  Number of transmit/received byte.
 * @param     pTxData Transmit data storage buffer pointer.
 * @param     pRxData Receive data storage buffer pointer.
 * @retval    SIO_OK       Successful operation.
 * @retval    SIO_ERR_HARD Hardware error.
 * @details   Transmits a specified number of bytes of pTxData.
 * @note      Makes the initialization settings, following serial I/O enable setting processing,
 * according to the initial setting procedure for master transmission/reception described in the hardware manual.
***********************************************************************************************************************/
int16_t R_SIO_TRx_Data(uint16_t TRxCnt, uint8_t FAR* pTxData, uint8_t FAR* pRxData);

/**************************************************************
 * Function Name: R_SIO_Tx_SameData
 * @brief     Designated data consecutive transmission.
 * @param
 * @retval
 * @details
 * @note
**************************************************************/
int16_t R_SIO_Tx_SameData(uint16_t TxCnt, uint8_t Data);

/************************************************************
 * Function Name: R_SIO_Chk_RxData
 * @brief     SIO recpetion data check.
 * @param
 * @retval
 * @details
 * @note
*************************************************************/
int16_t R_SIO_Chk_RxData(uint32_t RtyCnt, uint8_t ChkData);


#endif /* __SIO_H__ */

