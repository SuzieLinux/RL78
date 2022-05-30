/**
 * @file   lin.h
 * @author Michel Catudal
 * @date   6 March 2022
 * @version 0.1
 * @brief  Support for LIN gauges
 *
*/

#ifndef LIN_H
#define LIN_H

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#define RightAir_LIN_ID  0xD8
#define LeftAir_LIN_ID   0x99
#define SpareAir_LIN_ID  0x9C
#define LIN_CommTimeoutValue 63 /* 2.5 secs using 40 msecs task */

extern uint8_t TxBuffer[20];
extern uint8_t ucLIN_CommTimeout;

void vLin_Init(void);
uint8_t CalculateChecksum(uint8_t *buffer, uint8_t ByteCount);
void Init_LIN_HeaderRx(void);
void LIN_Transmit(uint8_t *msg, uint8_t Len);

#endif

