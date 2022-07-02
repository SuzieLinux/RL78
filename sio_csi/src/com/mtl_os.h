/************************************************************************************************************************
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
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file     mtl_os.h
 * @brief    Common file
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
#ifndef MTL_OS_H
#define MTL_OS_H

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/



/*----------------------------------------------------------------------------------------------*
    Definitions
 *----------------------------------------------------------------------------------------------*/
#if 1

    #if 1
/* for microITRON 3.00                                                                          */
#define MTL_STARTCODE                                       (0)                                         /** SET **/
#define MTL_E_OK                                            ( E_OK                                   )  /** SET **/
#define MTL_WM_AND_CLR                                      ( (uint)((uint)TWF_ANDW | (uint)TWF_CLR) )  /** SET **/
#define MTL_WM_AND                                          ( TWF_ANDW                               )  /** SET **/

    #else

/* for microITRON 4.00                                                                          */
#define MTL_STARTCODE                                       (0)                                         /** SET **/
#define MTL_E_OK                                            ( E_OK      )                               /** SET **/
#define MTL_WM_AND_CLR                                      ( TWF_ANDW  )                               /** SET **/
#define MTL_WM_AND                                          ( TWF_ANDW  )                               /** SET **/

    #endif /* 1 */

#else

/* without RTOS                                                                                 */
#define MTL_STARTCODE                                       (0)
#define MTL_E_OK                                            (0)
#define MTL_WM_AND_CLR                                      (0)
#define MTL_WM_AND                                          (0)

#endif /* 1 */


#endif  /* __MTL_OS_H__ */
