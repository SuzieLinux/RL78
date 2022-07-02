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
 * @file     mtl_tim.h
 * @brief    Common file
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
#ifndef MTL_TIM_H
#define MTL_TIM_H

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/



/*----------------------------------------------------------------------------------------------*/
/*  Define the counter value for the timer.                                     */  /** SET **/
/*  Specify according to the user MCU, clock and wait requirements.             */  /** SET **/
#if 1
/*  Setting for xxxxxx                                  */
#define MTL_T_1US                      (1)      /* loop Number of   1us         */  /** SET **/
#define MTL_T_2US                      (2)      /* loop Number of   2us         */  /** SET **/
#define MTL_T_4US                      (4)      /* loop Number of   4us         */  /** SET **/
#define MTL_T_5US                      (5)      /* loop Number of   5us         */  /** SET **/
#define MTL_T_10US                    (11)      /* loop Number of  10us         */  /** SET **/
#define MTL_T_20US                    (22)      /* loop Number of  20us         */  /** SET **/
#define MTL_T_30US                    (33)      /* loop Number of  30us         */  /** SET **/
#define MTL_T_50US                    (55)      /* loop Number of  50us         */  /** SET **/
#define MTL_T_100US                  (110)      /* loop Number of 100us         */  /** SET **/
#define MTL_T_200US                  (220)      /* loop Number of 200us         */  /** SET **/
#define MTL_T_300US                  (330)      /* loop Number of 300us         */  /** SET **/
#define MTL_T_400US      (MTL_T_200US * 2)      /* loop Number of 400us         */  /** SET **/
#define MTL_T_1MS                   (1100)      /* loop Number of   1ms         */  /** SET **/
#endif



#endif  /* __MTL_TIM_H__ */
