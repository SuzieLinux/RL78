/**
 * @file   vfd.h
 * @author Michel Catudal
 * @date   16 April 2022
 * @version 0.1
 * @brief  Futaba VFD display support
 *
*/
#ifndef __VFD_H__
#define __VFD_H__

#include "r_cg_macrodriver.h"

#define VFD_CS   P2_bit.no3
#define VFD_WR   P2_bit.no2
#define VFD_Busy P3_bit.no3

#define VFD_BS  8
#define VFD_HS  9
#define VFD_LF  0x0A
#define VFD_FF  0x0C
#define VFD_CR  0x0D
#define VFD_CLR 0x0E
#define VFD_DC1 0x11
#define VFD_DC2 0x12
#define VFD_DC4 0x14
#define VFD_DC5 0x15
#define VFD_DC6 0x16
#define VFD_DC7 0x17
#define VFD_CT0 0x18
#define VFD_CT1 0x19
#define VFD_ESC 0x1B
#define VFD_CursorPosition 0x48
#define VFD_Luminance 0x4C
#define VFD_Flickerless 0x53
#define VFD_BlinkSpeed 0x54

void VFD_CursorBlink(uint8_t blink_rate);
void VFD_MoveCursor(uint8_t position);
void VFD_SetLuminance(uint8_t luminance);
void VFD_ClearScreen(void);
void VFD_ChangeLine(void);
void VFD_writeString(char *str);

#endif
