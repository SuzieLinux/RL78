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
* Copyright (C) 2011, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial.c
* Version      : Applilet3 for RL78/G14 V2.05.06.02 [08 Nov 2021]
* Device(s)    : R5F104BG
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for Serial module.
* Creation Date: 7/2/2022
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
uint8_t * gp_csi11_rx_address;         /* csi11 receive buffer address */
uint16_t  g_csi11_rx_length;           /* csi11 receive data length */
uint16_t  g_csi11_rx_count;            /* csi11 receive data count */
uint8_t * gp_csi11_tx_address;         /* csi11 send buffer address */
uint16_t  g_csi11_send_length;         /* csi11 send data length */
uint16_t  g_csi11_tx_count;            /* csi11 send data count */
uint8_t * gp_csi20_rx_address;         /* csi20 receive buffer address */
uint16_t  g_csi20_rx_length;           /* csi20 receive data length */
uint16_t  g_csi20_rx_count;            /* csi20 receive data count */
uint8_t * gp_csi20_tx_address;         /* csi20 send buffer address */
uint16_t  g_csi20_send_length;         /* csi20 send data length */
uint16_t  g_csi20_tx_count;            /* csi20 send data count */
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
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0000_SAU_CK00_FCLK_0 | _0000_SAU_CK01_FCLK_0;
    R_CSI11_Create();
}

/***********************************************************************************************************************
* Function Name: R_CSI11_Create
* Description  : This function initializes the CSI11 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Create(void)
{
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON;    /* disable CSI11 */
    CSIMK11 = 1U;    /* disable INTCSI11 interrupt */
    CSIIF11 = 0U;    /* clear INTCSI11 interrupt flag */
    /* Set INTCSI11 low priority */
    CSIPR111 = 1U;
    CSIPR011 = 1U;
    SIR03 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR03 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
    SCR03 = _C000_SAU_RECEPTION_TRANSMISSION | _3000_SAU_TIMING_4 | _0000_SAU_MSB | _0007_SAU_LENGTH_8;
    SDR03 = _1E00_CSI11_DIVISOR;
    SO0 &= ~_0800_SAU_CH3_CLOCK_OUTPUT_1;    /* CSI11 clock initial level */
    SO0 &= ~_0008_SAU_CH3_DATA_OUTPUT_1;    /* CSI11 SO initial level */
    SOE0 |= _0008_SAU_CH3_OUTPUT_ENABLE;    /* enable CSI11 output */
    /* Set SI11 pin */
    PM1 |= 0x02U;
    /* Set SO11 pin */
    PMC1 &= 0xFBU;
    P1 |= 0x04U;
    PM1 &= 0xFBU;
    /* Set SCK11 pin */
    P1 |= 0x01U;
    PM1 &= 0xFEU;
}

/***********************************************************************************************************************
* Function Name: R_CSI11_Start
* Description  : This function starts the CSI11 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Start(void)
{
    SO0 &= ~_0800_SAU_CH3_CLOCK_OUTPUT_1;   /* CSI11 clock initial level */
    SO0 &= ~_0008_SAU_CH3_DATA_OUTPUT_1;           /* CSI11 SO initial level */
    SOE0 |= _0008_SAU_CH3_OUTPUT_ENABLE;           /* enable CSI11 output */
    SS0 |= _0008_SAU_CH3_START_TRG_ON;             /* enable CSI11 */
    CSIIF11 = 0U;    /* clear INTCSI11 interrupt flag */
    CSIMK11 = 0U;    /* enable INTCSI11 */
}

/***********************************************************************************************************************
* Function Name: R_CSI11_Stop
* Description  : This function stops the CSI11 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Stop(void)
{
    CSIMK11 = 1U;    /* disable INTCSI11 interrupt */
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON;        /* disable CSI11 */
    SOE0 &= ~_0008_SAU_CH3_OUTPUT_ENABLE;    /* disable CSI11 output */
    CSIIF11 = 0U;    /* clear INTCSI11 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_CSI11_Send_Receive
* Description  : This function sends and receives CSI11 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_CSI11_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf)
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
        gp_csi11_rx_address = rx_buf;     /* receive buffer pointer */
        CSIMK11 = 1U;                     /* disable INTCSI11 interrupt */

        if (0U != gp_csi11_tx_address)
        {
            SIO11 = *gp_csi11_tx_address;    /* started by writing data to SDR[7:0] */
            gp_csi11_tx_address++;
        }
        else
        {
            SIO11 = 0xFFU;
        }

        g_csi11_tx_count--;
        CSIMK11 = 0U;                     /* enable INTCSI11 interrupt */
    }

    return (status);
}

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
void R_CSI20_Create(void)
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
    CSIIF20 = 0U;    /* clear INTCSI20 interrupt flag */
    CSIMK20 = 0U;    /* enable INTCSI20 */
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
    CSIIF20 = 0U;    /* clear INTCSI20 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_CSI20_Send_Receive
* Description  : This function sends and receives CSI20 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_CSI20_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_csi20_tx_count = tx_num;        /* send data count */
        gp_csi20_tx_address = tx_buf;     /* send buffer pointer */
        gp_csi20_rx_address = rx_buf;     /* receive buffer pointer */
        CSIMK20 = 1U;                     /* disable INTCSI20 interrupt */

        if (0U != gp_csi20_tx_address)
        {
            SIO20 = *gp_csi20_tx_address;    /* started by writing data to SDR[7:0] */
            gp_csi20_tx_address++;
        }
        else
        {
            SIO20 = 0xFFU;
        }

        g_csi20_tx_count--;
        CSIMK20 = 0U;                     /* enable INTCSI20 interrupt */
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
