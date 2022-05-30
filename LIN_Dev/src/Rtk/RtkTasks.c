/**
 * @file   RtkTasks.c
 * @author Michel Catudal
 * @date   6 March 2022
 * @version 0.1
 * @brief  Real Time Tasks
 *
*/
#include "macrodriver.h"
#include "userdefine.h"
#include "timers.h"
#include "RtkSched.h"
#include "RtkTasks.h"
#include "lin.h"
#include <string.h>
#include "port.h"

/*---------------------------------------------------------------------------*/
/* CONST                                                                     */

const uint16_t CuiRtk_eTabActiv[DucRtk_eTASKNBRECUR] =
{
    DuiRtk_iARM0,
    DuiRtk_iARM1,
    DuiRtk_iARM2,
    DuiRtk_iARM3,
    DuiRtk_iARM4,
    DuiRtk_iARM5,
    DuiRtk_iARM6,
    DuiRtk_iARM7
};

volatile uint8_t ucRtk_i100msecsCount = 0;

/*****************************************************************************
 *      @brief 10 msecs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask10_Tsk(void)
{


#if 0

    if (ucRtk_i100msecsCount)
    {
        ucRtk_i100msecsCount--;
    }
    else
    {
        ucRtk_i100msecsCount = 9;
    }

#endif
}

/*****************************************************************************
 *      @brief 20 msecs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask20_Tsk(void)
{
}

/*****************************************************************************
 *      @brief 20 msecs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask20B_Tsk(void)
{
}

/*****************************************************************************
 *      @brief 40 msecs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask40_Tsk(void)
{
    if (ucLIN_CommTimeout) ucLIN_CommTimeout--;
}

/*****************************************************************************
 *      @brief 80 msecs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask80_Tsk (void)
{
;
}

/*****************************************************************************
 *      @brief 60 msecs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask160_Tsk(void)
{
;
}

/*****************************************************************************
 *      @brief 320 msecs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask320_Tsk(void)
{
;
}

/*****************************************************************************
 *      @brief 640 msecs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask640_Tsk(void)
{}

/*****************************************************************************
 *      @brief 1.28 secs task
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eTask1280_Tsk(void)
{
#if 0
    uint8_t uci;
    stDSM_PwmStatus.msg.S1_PWM = 700 / 2;
    stDSM_PwmStatus.msg.S3_PWM = 700 / 4;
    stDSM_PwmStatus.msg.S2_status = 1;
    stDSM_PwmStatus.msg.S2_status = 0;

    stDSM_PwmStatus.msg.HeaderPart1 = 0x50;
    stDSM_PwmStatus.msg.HeaderPart2 = 0x50 ^ 0xFF;
    stDSM_PwmStatus.msg.checksum = stDSM_PwmStatus.Byte[2] + stDSM_PwmStatus.Byte[3] +
    stDSM_PwmStatus.Byte[4] + stDSM_PwmStatus.Byte[5];

    for (uci = 0; uci < 7; uci++)
    {
        uart_tx_bit_bang(stDSM_PwmStatus.Byte[uci]);
    }
#endif
}

