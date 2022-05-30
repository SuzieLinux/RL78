/************************************************************************/
/* Header file generated from device file:                              */
/*    DR5F10RGA.DVF                                                     */
/*    V1.03 (2012/08/20)                                                */
/*    Copyright(C) 2012 Renesas                                         */
/* Tool Version: 4.0.0                                                  */
/* Date Generated: 2020/01/17                                           */
/************************************************************************/
#include "interrupt_handlers.h"

extern void PowerON_Reset (void);

const unsigned char Option_Bytes[]  __attribute__ ((section (".option_bytes"))) = {
	0xef, 0xff, 0xe0, 0x85
};

const unsigned char Security_Id[]  __attribute__ ((section (".security_id"))) = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define VEC          __attribute__ ((section (".vec")))
const void *HardwareVectors[] VEC = {
	// Address 0x0
	PowerON_Reset,
	// Secure for Debugging
	(void*)0xFFFF
};

#define VECT_SECT          __attribute__ ((section (".vects")))
const void *Vectors[] VECT_SECT = {
	//INT_WDTI (0x4)
	INT_WDTI,
	//INT_LVI (0x6)
	INT_LVI,
	//INT_P0 (0x8)
	INT_P0,
	//INT_P1 (0xA)
	INT_P1,
	//INT_P2 (0xC)
	INT_P2,
	//INT_P3 (0xE)
	INT_P3,
	//INT_P4 (0x10)
	INT_P4,
	//INT_P5 (0x12)
	INT_P5,
	//INT_DMA0 (0x14)
	INT_DMA0,
	//INT_DMA1 (0x16)
	INT_DMA1,
	//INT_CSI00/INT_ST0 (0x18)
	INT_ST0,
	//INT_CSI01/INT_SR0 (0x1A)
	INT_SR0,
	//INT_SRE0/INT_TM01H (0x1C)
	INT_TM01H,
	// Padding
	(void*)0xFFFF,
	//INT_TM00 (0x20)
	INT_TM00,
	// Padding
	(void*)0xFFFF,
	//INT_TM03H (0x24)
	INT_TM03H,
	//INT_IICA0 (0x26)
	INT_IICA0,
	//INT_TM01 (0x28)
	INT_TM01,
	//INT_TM02 (0x2A)
	INT_TM02,
	//INT_TM03 (0x2C)
	INT_TM03,
	//INT_AD (0x2E)
	INT_AD,
	//INT_RTC (0x30)
	INT_RTC,
	//INT_IT (0x32)
	INT_IT,
	//INT_KR (0x34)
	INT_KR,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	//INT_TM04 (0x3C)
	INT_TM04,
	//INT_TM05 (0x3E)
	INT_TM05,
	//INT_TM06 (0x40)
	INT_TM06,
	//INT_TM07 (0x42)
	INT_TM07,
	//INT_LCD0 (0x44)
	INT_LCD0,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	//INT_MD (0x4A)
	INT_MD,
	//INT_FL (0x4C)
	INT_FL,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	// Padding
	(void*)0xFFFF,
	//INT_BRK_I (0x7E)
	INT_BRK_I,
};

