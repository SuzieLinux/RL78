/**
 * @file   Tasks.c
 * @author Michel Catudal
 * @date   2 July 2022
 * @brief  Real Time Tasks
 *
*/
#include "system.h"
#include "Scheduler.h"
#include "Tasks.h"


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
}

