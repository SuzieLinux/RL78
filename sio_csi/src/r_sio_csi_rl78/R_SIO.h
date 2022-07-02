/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2010-2020 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file     R_SIO.h
 * @brief    SIO driver SIO I/F module common definitions.
 **********************************************************************************************************************/


/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
#ifndef SIO_H
#define SIO_H

/*----------- Definitions of return value ------------*/
#define SIO_OK                ((error_t)( 0))         /* Successful operation                     */
#define SIO_ERR_PARAM         ((error_t)(-1))         /* Parameter error                          */
#define SIO_ERR_HARD          ((error_t)(-2))         /* Hardware error                           */
#define SIO_ERR_OTHER         ((error_t)(-7))         /* Other error                              */


/*------------- Definitions of log type --------------*/
#define SIO_LOG_ERR           ((uint8_t)(0x01))       /* Log type : Error                         */


/*--------------- Definitions of flag ----------------*/
#define SIO_TRUE              ((uint8_t)(0x01))       /* Flag "ON"                                */
#define SIO_FALSE             ((uint8_t)(0x00))       /* Flag "OFF"                               */


/*----------- Definitions of port control ------------*/
#define SIO_HI                ((uint8_t)(0x01))       /* Port "H"                                 */
#define SIO_LOW               ((uint8_t)(0x00))       /* Port "L"                                 */
#define SIO_OUT               ((uint8_t)(0x01))       /* Port output setting                      */
#define SIO_IN                ((uint8_t)(0x00))       /* Port input setting                       */


/*------- Definitions of software timer value --------*/
/* Transmit&receive waiting time */
#define SIO_TX_WAIT           ((uint16_t)(50000))     /* SIO transmission completion waiting time 50000* 1us =  50ms  */
#define SIO_RX_WAIT           ((uint16_t)(50000))     /* SIO reception completion waiting time    50000* 1us =  50ms  */
#define SIO_DMA_TX_WAIT       ((uint16_t)(50000))     /* DMA transmission completion waiting time 50000* 1us =  50ms  */
#define SIO_DMA_RX_WAIT       ((uint16_t)(50000))     /* DMA reception completion waiting time    50000* 1us =  50ms  */

/* Access time */
#define SIO_T_SIO_WAIT        ((uint16_t)MTL_T_1US)   /* SIO transmission&reception completion waiting polling time   */
#define SIO_T_DMA_WAIT        ((uint16_t)MTL_T_1US)   /* DMA transmission&reception completion waiting polling time   */

#define SIO_T_BRR_WAIT        ((uint16_t)MTL_T_10US)  /* BRR setting wait time                    */         /** SET **/

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/
/* uint32_t <-> uint8_t conversion */
typedef union {
    uint32_t    ul;
    uint8_t uc[4];
} SIO_EXCHG_LONG;                                       /* total 4bytes                                 */


/* uint16_t <-> uint8_t conversion */
typedef union {
    uint16_t    us;
    uint8_t uc[2];
} SIO_EXCHG_SHORT;                                      /* total 2bytes                                 */

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/



/*******************************************************************************************************************//**
 * Function Name: R_SIO_Init_Driver
 * @brief     Driver initialization processing.
 * @param     none
 * @retval    SIO_OK Successful operation.
 * @details   Initializes the driver. Disables the serial I/O function and set the pin in the port. \n
 * @note      Performs the following processing, considering the previous use conditions. \n
***********************************************************************************************************************/
error_t R_SIO_Init_Driver(void);

/**********************************************************************************************************************
 * End of function R_SIO_Init_Driver
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Disable
 * @brief     Serial I/O disable setting processing.
 * @param     none
 * @retval    SIO_OK Successful operation.
 * @details   Disables the serial I/O function and sets the pins to function as ports. \n
 * @note      Performs the following processing, considering the previous use conditions. \n
***********************************************************************************************************************/
error_t R_SIO_Disable(void);

/**********************************************************************************************************************
 * End of function R_SIO_Disable
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Enable
 * @brief     Serial I/O enable setting processing.
 * @param     BrgData Bit rate setting value.
 * @retval    SIO_OK Successful operation.
 * @details   Enables the serial I/O function and sets the baud rate. \n
 * @note      Perform processing according to the initial setting procedure of the master send \n
 * and master send / receive as described in the hardware manual.
***********************************************************************************************************************/
error_t R_SIO_Enable(uint8_t BrgData);

/**********************************************************************************************************************
 * End of function R_SIO_Enable
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Open_Port
 * @brief     Serial I/O open setting processing.
 * @param     none
 * @retval    SIO_OK Successful operation.
 * @details   Sets the pin used for serial I/O to "open" (input state). \n
 * @note      Prepared to connect and disconnect removable media. Use this function before \n
***********************************************************************************************************************/
error_t R_SIO_Open_Port(void);

/**********************************************************************************************************************
 * End of function R_SIO_Open_Port
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Tx_Data
 * @brief     Serial I/O data transmit processing.
 * @param     TxCnt Number of transmitted bytes.
 * @param     pData Transmit data storage buffer pointer.
 * @retval    SIO_OK       Successful operation.
 * @retval    SIO_ERR_HARD Hardware error.
 * @details   Serial I/O data transmit processing. \n
 * @note      Makes the initialization settings, following serial I/O enable setting processing, according to \n
 * the initial setting procedure for master transmission described in the hardware manual. \n
***********************************************************************************************************************/
error_t R_SIO_Tx_Data(uint16_t TxCnt, uint8_t FAR* pData);

/**********************************************************************************************************************
 * End of function R_SIO_Tx_Data
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Rx_Data
 * @brief     Serial I/O data receive processing.
 * @param     RxCnt Number of received bytes.
 * @param     pData Receive data storage buffer pointer.
 * @retval    SIO_OK       Successful operation.
 * @retval    SIO_ERR_HARD Hardware error.
 * @details   Receives a specified number of data and stores it in pData. \n
 * @note      Makes the initialization settings, following serial I/O enable setting processing, \n
 * according to the initial setting procedure for master transmission/reception described in the hardware manual. \n
***********************************************************************************************************************/
error_t R_SIO_Rx_Data(uint16_t RxCnt, uint8_t FAR* pData);

/**********************************************************************************************************************
 * End of function R_SIO_Rx_Data
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_TRx_Data
 * @brief     Serial I/O data transmit/receive processing.
 * @param     TRxCnt  Number of transmit/received byte.
 * @param     pTxData Transmit data storage buffer pointer.
 * @param     pRxData Receive data storage buffer pointer.
 * @retval    SIO_OK       Successful operation.
 * @retval    SIO_ERR_HARD Hardware error.
 * @details   Transmits a specified number of bytes of pTxData. \n
 * @note      Makes the initialization settings, following serial I/O enable setting processing, \n
 * according to the initial setting procedure for master transmission/reception described in the hardware manual. \n
***********************************************************************************************************************/
error_t R_SIO_TRx_Data(uint16_t TRxCnt, uint8_t FAR* pTxData, uint8_t FAR* pRxData);

/**********************************************************************************************************************
 * End of function R_SIO_TRx_Data
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Tx_Data_CRC16
 * @brief     SIO transmission & CRC-CCITT calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Tx_Data_CRC16(uint16_t TxCnt, uint8_t FAR* pData, uint16_t* pCrc16);

/**********************************************************************************************************************
 * End of function R_SIO_Tx_Data_CRC16
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Rx_Data_CRC16
 * @brief     SIO reception & CRC-CCITT calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Rx_Data_CRC16(uint16_t RxCnt, uint8_t FAR* pData, uint16_t* pCrc16);

/**********************************************************************************************************************
 * End of function R_SIO_Rx_Data_CRC16
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Tx_Data_CRC7
 * @brief     SIO transmission & CRC7 calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Tx_Data_CRC7(uint16_t TxCnt, uint8_t FAR* pData, uint8_t* pCrc7);

/**********************************************************************************************************************
 * End of function R_SIO_Tx_Data_CRC7
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Rx_Data_CRC7
 * @brief     SIO reception & CRC7 calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Rx_Data_CRC7(uint16_t RxCnt, uint8_t FAR* pData, uint8_t* pCrc7);

/**********************************************************************************************************************
 * End of function R_SIO_Rx_Data_CRC7
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Tx_SameData
 * @brief     Designated data consecutive transmission.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Tx_SameData(uint16_t TxCnt, uint8_t Data);

/**********************************************************************************************************************
 * End of function R_SIO_Tx_SameData
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Chk_RxData
 * @brief     SIO recpetion data check.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Chk_RxData(uint32_t RtyCnt, uint8_t ChkData);

/**********************************************************************************************************************
 * End of function R_SIO_Chk_RxData
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Tx_Data_DMA
 * @brief     SIO DMA transmission.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Tx_Data_DMA(uint16_t TxCnt, uint8_t FAR* pData);

/**********************************************************************************************************************
 * End of function R_SIO_Tx_Data_DMA
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Tx_Data_DMA_CRC16
 * @brief     SIO DMA transmission & CRC-CCITT calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Tx_Data_DMA_CRC16(uint16_t TxCnt, uint8_t FAR* pData, uint16_t* pCrc16);

/**********************************************************************************************************************
 * End of function R_SIO_Tx_Data_DMA_CRC16
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Tx_Data_DMA_CRC7
 * @brief     SIO DMA transmission & CRC7 calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Tx_Data_DMA_CRC7(uint16_t TxCnt, uint8_t FAR* pData, uint8_t* pCrc7);

/**********************************************************************************************************************
 * End of function R_SIO_Tx_Data_DMA_CRC7
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Rx_Data_DMA
 * @brief     SIO DMA reception.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Rx_Data_DMA(uint16_t RxCnt, uint8_t FAR* pData);

/**********************************************************************************************************************
 * End of function R_SIO_Rx_Data_DMA
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Rx_Data_DMA_CRC16
 * @brief     SIO DMA reception & CRC-CCITT calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Rx_Data_DMA_CRC16(uint16_t RxCnt, uint8_t FAR* pData, uint16_t* pCrc16);

/**********************************************************************************************************************
 * End of function R_SIO_Rx_Data_DMA_CRC16
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_Rx_Data_DMA_CRC7
 * @brief     SIO DMA reception & CRC7 calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
error_t R_SIO_Rx_Data_DMA_CRC7(uint16_t RxCnt, uint8_t FAR* pData, uint8_t* pCrc7);

/**********************************************************************************************************************
 * End of function R_SIO_Rx_Data_DMA_CRC7
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_CRC16
 * @brief     CRC-CCITT calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
uint16_t R_SIO_CRC16(uint8_t FAR* pData, uint16_t CrcCnt);

/**********************************************************************************************************************
 * End of function R_SIO_CRC16
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: R_SIO_CRC7
 * @brief     CRC7 calculation.
 * @param     
 * @retval    
 * @details   
 * @note      
***********************************************************************************************************************/
uint8_t R_SIO_CRC7(uint8_t FAR* pData, uint16_t CrcCnt);

/**********************************************************************************************************************
 * End of function R_SIO_CRC7
 *********************************************************************************************************************/


#endif /* __SIO_H__ */

/* End of File */
