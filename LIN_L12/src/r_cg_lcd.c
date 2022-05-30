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
* Copyright (C) 2012, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_lcd.c
* Version      : CodeGenerator for RL78/L12 V2.04.05.02 [08 Nov 2021]
* Device(s)    : R5F10RGA
* Tool-Chain   : GCCRL78
* Description  : This file implements device driver for LCD module.
* Creation Date: 5/26/2022
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_lcd.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_LCD_Create
* Description  : This function initializes the LCD module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Create(void)
{
    RTCEN = 1U;    /* supply LCD clock */
    LCDON = 0U;    /* disable LCD clock operation */
    LCDM0 = _00_LCD_DISPLAY_WAVEFORM_A | _0D_LCD_DISPLAY_MODE1;
    LCDM0 |= _00_LCD_VOLTAGE_MODE_EXTERNAL;
    /* Set segment pins */
    PFSEG0 |= 0x70U;
    PFSEG2 |= 0x0FU;
    PFSEG3 |= 0x01U;
    PFSEG4 |= 0x00U;
    POM1 &= 0x5FU;
    PIM1 &= 0x9FU;
    PU1 &= 0x1FU;
    P1 &= 0x1FU;
    PM1 &= 0x1FU;
    PU3 &= 0xF8U;
    P3 &= 0xF8U;
    PM3 &= 0xF8U;
    PU4 &= 0xFDU;
    PMC4 &= 0xFDU;
    P4 &= 0xFDU;
    PM4 &= 0xFDU;
    PU7 &= 0xFEU;
    P7 &= 0xFEU;
    PM7 &= 0xFEU;
    LCDM1 |= _00_LCD_DISPLAY_PATTERN_A;
    LCDC0 = _04_LCD_CLOCK_FSUB_FIL_5;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Start
* Description  : This function enables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Start(void)
{
    LCDON = 1U;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Stop
* Description  : This function disables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Stop(void)
{
    LCDON = 0U;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Set_VoltageOn
* Description  : This function enables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Set_VoltageOn(void)
{
    SCOC = 1U;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Set_VoltageOff
* Description  : This function disables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Set_VoltageOff(void)
{
    SCOC = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
