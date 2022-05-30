/************************************************************************/
/* Header file generated from device file:                              */
/*    DR5F10RGA.DVF                                                     */
/*    V1.03 (2012/08/20)                                                */
/*    Copyright(C) 2012 Renesas                                         */
/* Tool Version: 4.0.0                                                  */
/* Date Generated: 2020/01/17                                           */
/************************************************************************/
#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

/*
 * INT_WDTI (0x4)
 */
void INT_WDTI(void) __attribute__ ((interrupt));

/*
 * INT_LVI (0x6)
 */
void INT_LVI(void) __attribute__ ((interrupt));

/*
 * INT_P0 (0x8)
 */
void INT_P0(void) __attribute__ ((interrupt));

/*
 * INT_P1 (0xA)
 */
void INT_P1(void) __attribute__ ((interrupt));

/*
 * INT_P2 (0xC)
 */
void INT_P2(void) __attribute__ ((interrupt));

/*
 * INT_P3 (0xE)
 */
void INT_P3(void) __attribute__ ((interrupt));

/*
 * INT_P4 (0x10)
 */
void INT_P4(void) __attribute__ ((interrupt));

/*
 * INT_P5 (0x12)
 */
void INT_P5(void) __attribute__ ((interrupt));

/*
 * INT_DMA0 (0x14)
 */
void INT_DMA0(void) __attribute__ ((interrupt));

/*
 * INT_DMA1 (0x16)
 */
void INT_DMA1(void) __attribute__ ((interrupt));

/*
 * INT_CSI00/INT_ST0 (0x18)
 */
void INT_ST0(void) __attribute__ ((interrupt));
//void INT_CSI00(void) __attribute__ ((interrupt));

/*
 * INT_CSI01/INT_SR0 (0x1A)
 */
void INT_SR0(void) __attribute__ ((interrupt));
//void INT_CSI01(void) __attribute__ ((interrupt));

/*
 * INT_SRE0/INT_TM01H (0x1C)
 */
void INT_TM01H(void) __attribute__ ((interrupt));
//void INT_SRE0(void) __attribute__ ((interrupt));

/*
 * INT_TM00 (0x20)
 */
void INT_TM00(void) __attribute__ ((interrupt));

/*
 * INT_TM03H (0x24)
 */
void INT_TM03H(void) __attribute__ ((interrupt));

/*
 * INT_IICA0 (0x26)
 */
void INT_IICA0(void) __attribute__ ((interrupt));

/*
 * INT_TM01 (0x28)
 */
void INT_TM01(void) __attribute__ ((interrupt));

/*
 * INT_TM02 (0x2A)
 */
void INT_TM02(void) __attribute__ ((interrupt));

/*
 * INT_TM03 (0x2C)
 */
void INT_TM03(void) __attribute__ ((interrupt));

/*
 * INT_AD (0x2E)
 */
void INT_AD(void) __attribute__ ((interrupt));

/*
 * INT_RTC (0x30)
 */
void INT_RTC(void) __attribute__ ((interrupt));

/*
 * INT_IT (0x32)
 */
void INT_IT(void) __attribute__ ((interrupt));

/*
 * INT_KR (0x34)
 */
void INT_KR(void) __attribute__ ((interrupt));

/*
 * INT_TM04 (0x3C)
 */
void INT_TM04(void) __attribute__ ((interrupt));

/*
 * INT_TM05 (0x3E)
 */
void INT_TM05(void) __attribute__ ((interrupt));

/*
 * INT_TM06 (0x40)
 */
void INT_TM06(void) __attribute__ ((interrupt));

/*
 * INT_TM07 (0x42)
 */
void INT_TM07(void) __attribute__ ((interrupt));

/*
 * INT_LCD0 (0x44)
 */
void INT_LCD0(void) __attribute__ ((interrupt));

/*
 * INT_MD (0x4A)
 */
void INT_MD(void) __attribute__ ((interrupt));

/*
 * INT_FL (0x4C)
 */
void INT_FL(void) __attribute__ ((interrupt));

/*
 * INT_BRK_I (0x7E)
 */
void INT_BRK_I(void) __attribute__ ((interrupt));

//Hardware Vectors
//PowerON_Reset (0x0)
void PowerON_Reset(void) __attribute__ ((interrupt));
#endif
