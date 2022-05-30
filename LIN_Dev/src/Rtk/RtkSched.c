/**
 * @file   RtkSched.c
 * @author Michel Catudal
 * @date   5 March 2022
 * @version 0.1
 * @brief  Real Time Scheduler
 *
*/

#include "r_cg_macrodriver.h"
#include "r_cg_tau.h"
#include "r_cg_userdefine.h"
#include "RtkTasks.h"
#include "Rtksched.h"
#include "lin.h"
#include <string.h>

/*---------------------------------------------------------------------------*/
/* TYPEDEF                                                                   */

typedef struct
{
    void (*CpRtk_iTaskEntry)(void);

} TstRtk_iTASKENTRY;

/*---------------------------------------------------------------------------*/
/* CONST                                                                     */
const TstRtk_iTASKENTRY CstRtk_iTaskEntry[DucRtk_eTASKNBTASK] =
{
    vRtk_eTask1280_Tsk,
    vRtk_eTask640_Tsk,
    vRtk_eTask320_Tsk,
    vRtk_eTask160_Tsk,
    vRtk_eTask80_Tsk,
    vRtk_eTask40_Tsk,
    vRtk_eTask20B_Tsk,
    vRtk_eTask20_Tsk,
    vRtk_eTask10_Tsk
};

uint8_t   ucRtk_iCountRTC;
TnRtk_eSemaph   nRtk_eSemafor;

/*****************************************************************************
 *      @brief Initialize variables
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eInitGlobVar_Set(void)
{
    ucRtk_iCountRTC = 0xFF;
}

/*****************************************************************************
 *      @brief Set up which task is going to be ran
 *
 *      @param[in] None
 *
 *      @return None
 */
void vRtk_eScheduler_Tsk(void)
{
    uint16_t uiVarLoc1, uiVarLoc2;
    uint8_t ucIndex, ucIndexSem;

    // Waiting for a task to be activated (Semafor not empty)
    while (!nRtk_eSemafor.uiSemaph);

    // Continuous loop of the scheduler
    for (;;)
    {
        // Running all the tasks inside the semafor
        // The semafor is made of 16 bits, 1 bit for each task
        // This bit is set to one by the 10ms interruption if the task must be run
        // In total, we have 9 tasks so 9 affected bits (b8..0)
        // The tasks with higher priority are affected to the higher bits
        // (b8 has the higher priority, b0 the lower)
        // The 10ms task is affected to b8, the 20ms task to b7 ...
        while (nRtk_eSemafor.uiSemaph)
        {
            // Checking b15..8
            uiVarLoc1 = nRtk_eSemafor.ucSemaph[1];

            // if b15..8 = 0 : checking b7..0
            if (!uiVarLoc1)
            {
                uiVarLoc1   = nRtk_eSemafor.ucSemaph[0];
                ucIndexSem  = 1;
                ucIndex     = 0;
            }
            else
            {
                ucIndexSem  = 0;
                ucIndex     = 8;
            }

            // uiVarLoc1 contains the 8 bits for the group of tasks to be run first
            //   (b15..8 if the 10ms task must be run, b7..0 else)
            // ucIndexSem indicates if these 8 bits are the higher ones (0 for b15..8)
            //   or the lower ones (1 for b7..0)
            // ucIndex will give the index inside CstRtk_iTaskEntry of the task
            //   to be run first (always 8 for the higher bits)

            // If at least one task must be run
            if (uiVarLoc1)
            {
                // uiVarLoc1 will now contain only the bit associated to the task
                // to be run that has the highest priority
                // ucIndex will be set to the correct value
                uiVarLoc2 = uiVarLoc1 & 0xF0;
                if (uiVarLoc2)
                {
                    uiVarLoc1   = uiVarLoc2;
                    ucIndex     += 4;
                }

                uiVarLoc2 = uiVarLoc1 & 0xCC;
                if (uiVarLoc2)
                {
                    uiVarLoc1   = uiVarLoc2;
                    ucIndex     += 2;
                }

                uiVarLoc2 = uiVarLoc1 & 0xAA;
                if (uiVarLoc2)
                {
                    uiVarLoc1   = uiVarLoc2;
                    ucIndex     += 1;
                }


                // The semafor is updated to indicate that the task with higher priority was run
                // (his associated bit is reset)
                if (ucIndexSem)
                {
                    nRtk_eSemafor.ucSemaph[0] ^= uiVarLoc1;
                }
                else
                {
                    nRtk_eSemafor.ucSemaph[1] ^= uiVarLoc1;
                }

                // Running the task with higher priority
                CstRtk_iTaskEntry[ucIndex].CpRtk_iTaskEntry();
            }
        }

        // Waiting for a task to be activated
        while (!nRtk_eSemafor.uiSemaph);
;    }
}

/*****************************************************************************
 *      @brief Timer function
 *
 *      @param[in] None
 *
 *      @return None
 */
#pragma vector = INTTM00_vect
__interrupt static void vRtk_eRealTimeFunction(void)
{
    uint8_t ucIndex, ucMask;

    // Updating the semafor to set the tasks to be run
    ucRtk_iCountRTC++;

    if (ucRtk_iCountRTC == 0x80)
    {
        ucRtk_iCountRTC = 0;
    }

    if (ucRtk_iCountRTC)
    {
        ucMask      = 1;
        ucIndex     = 1;

        while (!(ucRtk_iCountRTC & ucMask))
        {
            ucMask <<= 1;
            ucIndex++;
        }
    }
    else
    {
        ucIndex = 0;
    }

    nRtk_eSemafor.uiSemaph |= CuiRtk_eTabActiv[ucIndex];
}



