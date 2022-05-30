/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016, 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sau.c
* Version      : Applilet4 for RL78/G11 V1.02.05.01 [25 Nov 2020]
* Device(s)    : R5F1056A
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for SAU module.
* Creation Date: 5/28/2022
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
uint8_t * gp_uart0_tx_address;         /* uart0 transmit buffer address */
uint16_t  g_uart0_tx_count;            /* uart0 transmit data number */
uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
uint8_t * gp_csi11_rx_address;         /* csi11 receive buffer address */
uint16_t  g_csi11_rx_length;           /* csi11 receive data length */
uint16_t  g_csi11_rx_count;            /* csi11 receive data count */
uint8_t * gp_csi11_tx_address;         /* csi11 send buffer address */
uint16_t  g_csi11_send_length;         /* csi11 send data length */
uint16_t  g_csi11_tx_count;            /* csi11 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0RES = 1U;   /* reset serial array unit 0 */
    SAU0RES = 0U;   /* reset release of serial array unit 0 */
    SAU0EN = 1U;    /* enables input clock supply */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0040_SAU_CK01_fCLK_4 | _0004_SAU_CK00_fCLK_4;
    R_UART0_Create();
    R_CSI11_Create();
}
/***********************************************************************************************************************
* Function Name: R_UART0_Create
* Description  : This function initializes the UART0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Create(void)
{
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;
    STMK0 = 1U;     /* disable INTST0 interrupt */
    STIF0 = 0U;     /* clear INTST0 interrupt flag */
    SRMK0 = 1U;     /* disable INTSR0 interrupt */
    SRIF0 = 0U;     /* clear INTSR0 interrupt flag */
    SREMK0 = 1U;    /* disable INTSRE0 interrupt */
    SREIF0 = 0U;    /* clear INTSRE0 interrupt flag */
    /* Set INTSR0 low priority */
    SRPR10 = 1U;
    SRPR00 = 1U;
    /* Set INTST0 low priority */
    STPR10 = 1U;
    STPR00 = 1U;
    SMR00 = _0020_SMR00_DEFAULT_VALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR00 = _0004_SCR00_DEFAULT_VALUE | _8000_SAU_TRANSMISSION | _0000_SAU_TIMING_1 | _0000_SAU_INTSRE_MASK | 
            _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | _0003_SAU_LENGTH_8;
    SDR00 = _9A00_SAU_CH0_BAUDRATE_DIVISOR;
    NFEN0 |= _01_SAU_RXD0_FILTER_ON;
    SIR01 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;
    SMR01 = _0020_SMR01_DEFAULT_VALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS | 
            _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR01 = _0004_SCR01_DEFAULT_VALUE | _4000_SAU_RECEPTION | _0000_SAU_TIMING_1 | _0000_SAU_INTSRE_MASK | 
            _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | _0003_SAU_LENGTH_8;
    SDR01 = _9A00_SAU_CH1_BAUDRATE_DIVISOR;
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;
    SOL0 &= (uint16_t)~_0001_SAU_CHANNEL0_INVERTED;
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;
    /* Set RxD0 pin */
    PM5 |= 0x20U;      
    /* Set TxD0 pin */
    P5 |= 0x10U;        
    PM5 &= 0xEFU;      
}
/***********************************************************************************************************************
* Function Name: R_UART0_Start
* Description  : This function starts the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Start(void)
{
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;
    SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;
    STIF0 = 0U;     /* clear INTST0 interrupt flag */
    SRIF0 = 0U;     /* clear INTSR0 interrupt flag */
    STMK0 = 0U;     /* enable INTST0 interrupt */
    SRMK0 = 0U;     /* enable INTSR0 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_UART0_Stop
* Description  : This function stops the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Stop(void)
{
    STMK0 = 1U;     /* disable INTST0 interrupt */
    SRMK0 = 1U;     /* disable INTSR0 interrupt */
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;
    SOE0 &= (uint16_t)~_0001_SAU_CH0_OUTPUT_ENABLE;
    STIF0 = 0U;     /* clear INTST0 interrupt flag */
    SRIF0 = 0U;     /* clear INTSR0 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_UART0_Receive
* Description  : This function receives UART0 data.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_UART0_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_uart0_rx_count = 0U;
        g_uart0_rx_length = rx_num;
        gp_uart0_rx_address = rx_buf;
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_UART0_Send
* Description  : This function sends UART0 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_UART0_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_uart0_tx_address = tx_buf;
        g_uart0_tx_count = tx_num;
        STMK0 = 1U;    /* disable INTST0 interrupt */
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
        STMK0 = 0U;    /* enable INTST0 interrupt */
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_CSI11_Create
* Description  : This function initializes the CSI11 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Create(void)
{
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON;
    CSIMK11 = 1U;   /* disable INTCSI11 interrupt */
    CSIIF11 = 0U;   /* clear INTCSI11 interrupt flag */
    /* Set INTCSI11 low priority */
    CSIPR111 = 1U;
    CSIPR011 = 1U;
    SIR03 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;
    SMR03 = _0020_SMR03_DEFAULT_VALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS | 
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
    SCR03 = _0004_SCR03_DEFAULT_VALUE | _8000_SAU_TRANSMISSION | _0000_SAU_TIMING_1 | _0000_SAU_INTSRE_MASK | 
            _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0000_SAU_STOP_NONE | _0003_SAU_LENGTH_8;
    SDR03 = _9A00_SAU_CH3_BAUDRATE_DIVISOR;
    SO0 |= _0800_SAU_CH3_CLOCK_OUTPUT_1;
    SO0 &= (uint16_t)~_0008_SAU_CH3_DATA_OUTPUT_1;
    SOE0 |= _0008_SAU_CH3_OUTPUT_ENABLE;
    /* Set SO11 pin */
    PMC5 &= 0xBFU;      
    P5 |= 0x40U;        
    PM5 &= 0xBFU;       
    /* Set SCK11 pin */
    PMC3 &= 0xFEU;      
    P3 |= 0x01U;        
    PM3 &= 0xFEU;       
}
/***********************************************************************************************************************
* Function Name: R_CSI11_Start
* Description  : This function starts the CSI11 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Start(void)
{
    SO0 |= _0800_SAU_CH3_CLOCK_OUTPUT_1;
    SO0 &= (uint16_t)~_0008_SAU_CH3_DATA_OUTPUT_1;
    SOE0 |= _0008_SAU_CH3_OUTPUT_ENABLE;
    SS0 |= _0008_SAU_CH3_START_TRG_ON;
    CSIIF11 = 0U;   /* clear INTCSI11 interrupt flag */
    CSIMK11 = 0U;   /* enable INTCSI11 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_CSI11_Stop
* Description  : This function stops the CSI11 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Stop(void)
{
    CSIMK11 = 1U;   /* disable INTCSI11 interrupt */
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON;
    SOE0 &= (uint16_t)~_0008_SAU_CH3_OUTPUT_ENABLE;
    CSIIF11 = 0U;   /* clear INTCSI11 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_CSI11_Send
* Description  : This function sends CSI11 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_CSI11_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_csi11_tx_count = tx_num;        /* send data count */
        gp_csi11_tx_address = tx_buf;     /* send buffer pointer */
        CSIMK11 = 1U;                     /* disable INTCSI11 interrupt */
        SIO11 = *gp_csi11_tx_address;    /* started by writing data to SDR[7:0] */
        gp_csi11_tx_address++;
        g_csi11_tx_count--;
        CSIMK11 = 0U;                     /* enable INTCSI11 interrupt */
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
