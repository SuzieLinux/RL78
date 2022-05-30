/**
 * @file   lin.c
 * @author Michel Catudal
 * @date   16 April 2022
 * @version 0.1
 * @brief  Support for LIN gauges
 *
*/

#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
#include "r_cg_userdefine.h"
#include "r_cg_tau.h"
#include "lin.h"
#include "RtkTasks.h"
#include <string.h>

uint8_t byte_count = 0;
uint8_t TxBuffer[20] = { 0 };

/*  Timeout counters */
uint8_t ucLIN_CommTimeout = LIN_CommTimeoutValue;

uint8_t ucLIN_iChecksum = 0;

static uint8_t CalculateChecksum(uint8_t *buffer, uint8_t ByteCount);

/*****************************************************************************
 *      @brief  Calculate checksum
 *
 *      @param[in] None
 *
 *      @return None
*/
uint8_t CalculateChecksum(uint8_t *buffer, uint8_t ByteCount)
{
    uint16_t checksum = 0;
    uint8_t uci;

    for (uci = 0; uci < ByteCount; uci++)
    {
        checksum += buffer[uci]; // go on adding the byte
        if (checksum > 0xff)     // if more than one byte add the carry
        {
            checksum &= 0xff;
            checksum++;
        }
    }
    return (uint8_t)checksum;
}

/*****************************************************************************
*      @brief  LIN Slave transmit
*
*      @param[in] Pointer to message
*      @param[in] Number of bytes to send
*
*      @return None
*****************************************************************************/
void LIN_Transmit(uint8_t *msg, uint8_t Len)
{
    uint8_t buffer[9];
    uint8_t uci;


    if (Len > 8)
    {
        return;
    }

    memcpy(buffer, msg, Len);

    buffer[Len] = ~CalculateChecksum(msg, Len);
    Len++;

    STIF0 = 0;

    for (uci = 0; uci < Len; uci++)
    {
        TXD0 = buffer[uci];
        while (!STIF0);
        STIF0 = 0;
    }
    Init_LIN_HeaderRx();    // Restart Lin reception
}


/*****************************************************************************
*      @brief  Initiate reception of LIN Slave header
*
*      @param[in] None
*
*      @return None
*****************************************************************************/
void Init_LIN_HeaderRx(void)
{
    /* Make sure that UART0 reception is not enabled */
    SCR01 &= ~_4000_SAU_RECEPTION;

    sync_field_received = 0;
    /* Clear timer TM03 break received flag */
    break_received = 0;

    /* Start timer TM03 operations */
    R_TAU0_Channel3_Start();
}

