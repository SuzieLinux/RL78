/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2010-2020 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file     mtl_os.c
 * @brief    Common file
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "mtl_com.h"

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/



/*----------------------------------------------------------------------------------------------*
    Macros
 *----------------------------------------------------------------------------------------------*/
#ifdef MTL_OS_USE
    #if 1
/* for microITRON 3.00                                                                          */
#define MTL_WAI_SEM(semid)                                  (wai_sem((ID )(semid)))                                                           /** SET **/
#define MTL_SIG_SEM(semid)                                  (sig_sem((ID )(semid)))                                                           /** SET **/
#define MTL_DLY_TSK(dlytim)                                 (dly_tsk((DLYTIME )(dlytim)))                                                     /** SET **/
#define MTL_STA_TSK(tskid, stacd)                           (sta_tsk((ID )(tskid), (INT )(stacd)))                                            /** SET **/
#define MTL_SET_FLG(flgid, setptn)                          (set_flg((ID )(flgid), (UINT )(setptn)))                                          /** SET **/
#define MTL_CLR_FLG(flgid, clrptn)                          (clr_flg((ID )(flgid), (UINT )(clrptn)))                                          /** SET **/
#define MTL_WAI_FLG(p_flgptn, flgid, waiptn, wfmode)        (wai_flg((UINT* )(p_flgptn), (ID )(flgid), (UINT )(waiptn), (UINT )(wfmode)))     /** SET **/
#define MTL_POL_FLG(p_flgptn, flgid, waiptn, wfmode)        (pol_flg((UINT* )(p_flgptn), (ID )(flgid), (UINT )(waiptn), (UINT )(wfmode)))     /** SET **/

    #else
/* for microITRON 4.00                                                                          */
#define MTL_WAI_SEM(semid)                                  (wai_sem((ID )(semid)))                                                           /** SET **/
#define MTL_SIG_SEM(semid)                                  (sig_sem((ID )(semid)))                                                           /** SET **/
#define MTL_DLY_TSK(dlytim)                                 (dly_tsk((RELTIM )(dlytim)))                                                      /** SET **/
#define MTL_STA_TSK(tskid, stacd)                           (sta_tsk((ID )(tskid), (VP_INT )(stacd)))                                         /** SET **/
#define MTL_SET_FLG(flgid, setptn)                          (set_flg((ID )(flgid), (FLGPTN )(setptn)))                                        /** SET **/
#define MTL_CLR_FLG(flgid, clrptn)                          (clr_flg((ID )(flgid), (FLGPTN )(clrptn)))                                        /** SET **/
#define MTL_WAI_FLG(p_flgptn, flgid, waiptn, wfmode)        (wai_flg((ID )(flgid), (FLGPTN )(waiptn), (MODE )(wfmode), (FLGPTN* )(p_flgptn))) /** SET **/
#define MTL_POL_FLG(p_flgptn, flgid, waiptn, wfmode)        (pol_flg((ID )(flgid), (FLGPTN )(waiptn), (MODE )(wfmode), (FLGPTN* )(p_flgptn))) /** SET **/

    #endif /* 1 */

#else
#define MTL_WAI_SEM(semid)                                  (0)
#define MTL_SIG_SEM(semid)                                  (0)
#define MTL_DLY_TSK(dlytim)                                 (0)
#define MTL_STA_TSK(tskid, stacd)                           (0)
#define MTL_SET_FLG(flgid, setptn)                          (0)
#define MTL_CLR_FLG(flgid, clrptn)                          (0)
#define MTL_WAI_FLG(p_flgptn, flgid, waiptn, wfmode)        (0)
#define MTL_POL_FLG(p_flgptn, flgid, waiptn, wfmode)        (0)

#endif /* MTL_OS_USE */




/*----------------------------------------------------------------------------------------------*
    Programs
 *----------------------------------------------------------------------------------------------*/
/************************************************************************************************
 System calls
 ************************************************************************************************/
/*******************************************************************************************************************//**
 * Function Name: mtl_waisem2
 * @brief     Semaphore resource allocation (substitute system call).
 * @param     semid Semaphore ID number.
 * @return    End code.
 * @details   Allocates the semaphore resource.
 * @note      When the OS is not used, this function becomes a dummy processing.
***********************************************************************************************************************/
int mtl_waisem2(short semid)
{
    return MTL_WAI_SEM(semid);                  /* Semaphore resource allocation system call    */
}
/**********************************************************************************************************************
 * End of function mtl_waisem2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
* Function Name: mtl_sigsem2
* @brief     Semaphore resource deallocation (substitute system call).
* @param     semid Semaphore ID number.
* @return    End code.
* @details   Returns the semaphore resource.
* @note      When the OS is not used, this function becomes a dummy processing.
***********************************************************************************************************************/
int mtl_sigsem2(short semid)
{
    return MTL_SIG_SEM(semid);                  /* Semaphore resource deallocation system call  */
}
/**********************************************************************************************************************
 * End of function mtl_sigsem2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
* Function Name: mtl_dlytsk2
* @brief     Task execution delay (substitute system call).
* @param     dlytim Delay.
* @return    End code.
* @details   Delays execution of the task.
* @note      When the OS is not used, this function becomes a dummy processing.
***********************************************************************************************************************/
int mtl_dlytsk2(int dlytim)
{
    return MTL_DLY_TSK(dlytim);                         /* Task execution delay systm call      */
}
/**********************************************************************************************************************
 * End of function mtl_dlytsk2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
* Function Name: mtl_statsk2
* @brief     Activate  task (substitute system call).
* @param     tskid ID number of the task to be activated.
* @param     stacd Start code of the task.
* @return    End code.
* @details   Activate  task.
* @note      When the OS is not used, this function becomes a dummy processing.
***********************************************************************************************************************/
int mtl_statsk2(short tskid, int stacd)
{
    return MTL_STA_TSK(tskid, stacd);                   /* Activate task system call            */
}
/**********************************************************************************************************************
 * End of function mtl_statsk2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
* Function Name: mtl_setflg2
* @brief     Set event flag  (substitute system call).
* @param     flgid  ID number of event flag to be set.
* @param     setptn Bit pattern to set.
* @return    Event flag setting.
* @details   Activate  task.
* @note      When the OS is not used, this function becomes a dummy processing.
***********************************************************************************************************************/
int mtl_setflg2(short flgid, uint setptn)
{
    return MTL_SET_FLG(flgid, setptn);                  /* Set event flag system call           */
}
/**********************************************************************************************************************
 * End of function mtl_setflg2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
* Function Name: mtl_clrflg2
* @brief     Clear event flag (substitute system call).
* @param     flgid  ID number of event flag to be set.
* @param     clrptn Bits pattern to clear.
* @return    Event flag setting.
* @details   Clear of the event flag.
* @note      When the OS is not used, this function becomes a dummy processing.
***********************************************************************************************************************/
int mtl_clrflg2(short flgid, uint clrptn)
{
    return MTL_CLR_FLG(flgid, clrptn);              /* Clear event flag system call             */
}
/**********************************************************************************************************************
 * End of function mtl_clrflg2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
* Function Name: mtl_waiflg2
* @brief      Wait of the event flag (substitute system call).
* @param      p_flgptn  First address of the area of the bit pattern which is returned when the wait is released.
* @param      flgid     Event flag ID number.
* @param      waiptn    Wait bit pattern.
* @param      wfmode    Wait mode.
* @return     End code.
* @details    Wait the event flag.
* @note       When the OS is not used, this function becomes a dummy processing.
***********************************************************************************************************************/
int mtl_waiflg2(uint FAR* p_flgptn, short flgid, uint waiptn, uint wfmode)
{
    return MTL_WAI_FLG(p_flgptn, flgid, waiptn, wfmode);/* Event flag wait system call          */
}
/**********************************************************************************************************************
 * End of function mtl_waiflg2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
* Function Name: mtl_polflg2
* @brief      Get the event flag(substitute system call).
* @param      p_flgptn  First address of the area of the bit pattern which is returned when the wait is released.
* @param      flgid     Event flag ID number.
* @param      waiptn    Wait bit pattern.
* @param      wfmode    Wait mode.
* @return     End code.
* @details    Get the event flag. ( No wait).
* @note       When the OS is not used, this function becomes a dummy processing.
***********************************************************************************************************************/
int mtl_polflg2(uint FAR* p_flgptn, short flgid, uint waiptn, uint wfmode)
{
    return MTL_POL_FLG(p_flgptn, flgid, waiptn, wfmode);/* System call to get the event flag    */
}
/**********************************************************************************************************************
 * End of function mtl_polflg2
 *********************************************************************************************************************/

