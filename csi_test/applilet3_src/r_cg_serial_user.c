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
* File Name    : r_cg_serial_user.c
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
extern uint8_t * gp_csi11_rx_address;         /* csi11 receive buffer address */
extern uint16_t  g_csi11_rx_length;           /* csi11 receive data length */
extern uint16_t  g_csi11_rx_count;            /* csi11 receive data count */
extern uint8_t * gp_csi11_tx_address;         /* csi11 send buffer address */
extern uint16_t  g_csi11_send_length;         /* csi11 send data length */
extern uint16_t  g_csi11_tx_count;            /* csi11 send data count */
extern uint8_t * gp_csi20_rx_address;         /* csi20 receive buffer address */
extern uint16_t  g_csi20_rx_length;           /* csi20 receive data length */
extern uint16_t  g_csi20_rx_count;            /* csi20 receive data count */
extern uint8_t * gp_csi20_tx_address;         /* csi20 send buffer address */
extern uint16_t  g_csi20_send_length;         /* csi20 send data length */
extern uint16_t  g_csi20_tx_count;            /* csi20 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_csi11_interrupt
* Description  : This function is INTCSI11 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTCSI11_vect
__interrupt static void r_csi11_interrupt(void)
{
    volatile uint8_t err_type;
    volatile uint8_t sio_dummy;

    err_type = (uint8_t)(SSR03 & _0001_SAU_OVERRUN_ERROR);
    SIR03 = (uint16_t)err_type;

    if (err_type != 1U)
    {
        if (g_csi11_tx_count > 0U)
        {
            if (0U != gp_csi11_rx_address)
            {
                *gp_csi11_rx_address = SIO11;
                gp_csi11_rx_address++;
            }
            else
            {
                sio_dummy = SIO11;
            }

            if (0U != gp_csi11_tx_address)
            {
                SIO11 = *gp_csi11_tx_address;
                gp_csi11_tx_address++;
            }
            else
            {
                SIO11 = 0xFFU;
            }
            g_csi11_tx_count--;        
        }
        else
        {
            if (0U == g_csi11_tx_count)
            {
                if (0U != gp_csi11_rx_address)
                {
                    *gp_csi11_rx_address = SIO11;
                }
                else
                {
                    sio_dummy = SIO11;
                }
            }

            r_csi11_callback_receiveend();    /* complete receive */
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_csi11_callback_receiveend
* Description  : This function is a callback function when CSI11 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi11_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi20_interrupt
* Description  : This function is INTCSI20 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTCSI20_vect
__interrupt static void r_csi20_interrupt(void)
{
    volatile uint8_t err_type;
    volatile uint8_t sio_dummy;

    err_type = (uint8_t)(SSR10 & _0001_SAU_OVERRUN_ERROR);
    SIR10 = (uint16_t)err_type;

    if (err_type != 1U)
    {
        if (g_csi20_tx_count > 0U)
        {
            if (0U != gp_csi20_rx_address)
            {
                *gp_csi20_rx_address = SIO20;
                gp_csi20_rx_address++;
            }
            else
            {
                sio_dummy = SIO20;
            }

            if (0U != gp_csi20_tx_address)
            {
                SIO20 = *gp_csi20_tx_address;
                gp_csi20_tx_address++;
            }
            else
            {
                SIO20 = 0xFFU;
            }
            g_csi20_tx_count--;        
        }
        else
        {
            if (0U == g_csi20_tx_count)
            {
                if (0U != gp_csi20_rx_address)
                {
                    *gp_csi20_rx_address = SIO20;
                }
                else
                {
                    sio_dummy = SIO20;
                }
            }
        }
    }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
