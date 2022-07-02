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
 * @file     mtl_endi.c
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
    Programs
 *----------------------------------------------------------------------------------------------*/
/************************************************************************************************
Routines whose processing differs according to MCU and C compiler specifications
 ************************************************************************************************/
/*******************************************************************************************************************//**
 * Function Name: mtl_ShortPeek2
 * @brief     Reading of ushort type value in little endian.
 * @param     adr Address to be read from.
 * @return    Read value (ushort).
 * @details   Routine whose processing differs according to MCU and C compiler specifications.
 * @note      Switch MTL_MCU_LITTLE and MTL_ENDI_HISPEED to match the environment.
***********************************************************************************************************************/
ushort mtl_ShortPeek2(void FAR* adr)
{
#if ( defined(MTL_MCU_LITTLE) && defined(MTL_ENDI_HISPEED) )
    ushort FAR*         ptr;

    ptr = adr;                                          /* Convert the address into the ushort pointer  */
    return *ptr;                                        /* Read the value                       */

#else
    uchar FAR*          ptr;
    ushort              ret;

    ptr = adr;                                          /* Convert the address into the uchar pointer   */
    ret =                                 ptr[1];
    ret = (ushort )((ushort )(ret << 8) | ptr[0]);      /* Convert the address into the uchar pointer   */
    return ret;
#endif  /* MTL_MCU_LITTLE */
}
/**********************************************************************************************************************
 * End of function mtl_ShortPeek2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_LongPeek2
 * @brief     Reading of ulong type value in little endian.
 * @param     adr Address to be read from.
 * @return    Read value (ulong).
 * @details   Routine whose processing differs according to MCU and C compiler specifications.
 * @note      Switch MTL_MCU_LITTLE and MTL_ENDI_HISPEED to match the environment.
***********************************************************************************************************************/
ulong mtl_LongPeek2(void FAR* adr)
{
#if ( defined(MTL_MCU_LITTLE) && defined(MTL_ENDI_HISPEED) )
    ulong FAR*          ptr;

    ptr = adr;                                          /* Convert the address into the ulong pointer   */
    return *ptr;                                        /* Read the value                       */


#else
    uchar FAR*          ptr;
    ulong               ret;

    ptr = adr;                                          /* Convert the address into the uchar pointer   */
    ret =              ptr[3];
    ret = (ret << 8) | ptr[2];
    ret = (ret << 8) | ptr[1];
    ret = (ret << 8) | ptr[0];                          /* Read the value                       */

    return ret;
#endif  /* MTL_MCU_LITTLE */
}
/**********************************************************************************************************************
 * End of function mtl_LongPeek2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_ShortPoke2
 * @brief     Writing of ushort type value in little endian.
 * @param     adr Address to be written to.
 * @param     dat Write data
 * @return    none.
 * @details   Routine whose processing differs according to MCU and C compiler specifications.
 * @note      Switch MTL_MCU_LITTLE and MTL_ENDI_HISPEED to match the environment.
***********************************************************************************************************************/
void mtl_ShortPoke2(void FAR* adr, ushort dat)
{
#if ( defined(MTL_MCU_LITTLE) && defined(MTL_ENDI_HISPEED) )
    ushort FAR*         ptr;

    ptr = adr;                                          /* Convert the address into the ushort pointer  */
    *ptr = dat;                                         /* Write the value                      */

#else
    uchar FAR*          ptr;

    ptr = adr;                                          /* Convert the address into the uchar pointer   */
    ptr[0] = (uchar )(          dat & (ushort )0x00FF); /* Write the lower byte                 */
    ptr[1] = (uchar )((ushort )(dat & (ushort )0xFF00) >> 8);/* Write the upper byte            */
#endif  /* MTL_MCU_LITTLE */
}
/**********************************************************************************************************************
 * End of function mtl_ShortPoke2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_LongPoke2
 * @brief     Writing of ulong type value in little endian.
 * @param     adr Address to be written to.
 * @param     dat Write data
 * @return    none.
 * @details   Routine whose processing differs according to MCU and C compiler specifications.
 * @note      Switch MTL_MCU_LITTLE and MTL_ENDI_HISPEED to match the environment.
***********************************************************************************************************************/
void mtl_LongPoke2(void FAR* adr, ulong dat)
{
#if ( defined(MTL_MCU_LITTLE) && defined(MTL_ENDI_HISPEED) )
    ulong FAR*          ptr;

    ptr = adr;                                          /* Convert the address into the ulong pointer   */
    *ptr = dat;                                         /* Write the value                      */
#else
    uchar FAR*          ptr;

    ptr = adr;                                          /* Convert the address into the uchar pointer   */
    ptr[0] = (uchar )( dat & 0x000000FFUL       );      /* LSB Write                            */
    ptr[1] = (uchar )((dat & 0x0000FF00UL) >>  8);      /* Byte after LSB write                 */
    ptr[2] = (uchar )((dat & 0x00FF0000UL) >> 16);      /* Byte before MSB write                */
    ptr[3] = (uchar )((dat & 0xFF000000UL) >> 24);      /* MSB write                            */
#endif  /* MTL_MCU_LITTLE */
}
/**********************************************************************************************************************
 * End of function mtl_LongPoke2
 *********************************************************************************************************************/

