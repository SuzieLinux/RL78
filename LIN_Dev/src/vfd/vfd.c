/**
 * @file   vfd.c
 * @author Michel Catudal
 * @date   16 April 2022
 * @version 0.1
 * @brief  Futaba VFD display support
 *
*/

#include <stdio.h>
#include "vfd.h"
#include "r_cg_sau.h"

/*****************************************************************************
*      @brief Change the cursor blink rate in 30 msecs increment
*             default is 0x14
*
*      @param[in] Blink rate
*
*      @return None
******************************************************************************/
void VFD_CursorBlink(uint8_t blink_rate)
{
	uint8_t buf[3];
	buf[0] = VFD_ESC;
	buf[1] = VFD_BlinkSpeed;
	buf[2] = blink_rate;
	SPI_Send(buf,3);
}

/*****************************************************************************
*      @brief Move cursor
*             data values of 0x50 to 0xFF are invalid
*
*      @param[in] Position
*
*      @return None
******************************************************************************/
void VFD_MoveCursor(uint8_t position)
{
	uint8_t buf[3];
	buf[0] = VFD_ESC;
	buf[1] = VFD_CursorPosition;
	buf[2] = position;
	SPI_Send(buf,3);
}

/*****************************************************************************
*      @brief  Set the Luminance
*
*      @param[in] Luminance value
*
*      @return None
******************************************************************************/
void VFD_SetLuminance(uint8_t luminance)
{
	uint8_t buf[3];
	buf[0] = VFD_ESC;
	buf[1] = VFD_Luminance;
	buf[2] = luminance;
	SPI_Send(buf,3);
}

/*****************************************************************************
*      @brief Clear Screen
*
*      @param[in] None
*
*      @return None
******************************************************************************/
void VFD_ClearScreen(void)
{
	uint8_t c = VFD_CLR;
	SPI_Send(&c,1);
}

/*****************************************************************************
*      @brief Change Line
*
*      @param[in] None
*
*      @return None
******************************************************************************/
void VFD_ChangeLine(void)
{
	uint8_t buf[2];
	buf[0] = VFD_CR;
	buf[1] = VFD_LF;
	SPI_Send(buf,2);
}

