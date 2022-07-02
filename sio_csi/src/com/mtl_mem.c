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
 * @file     mtl_mem.c
 * @brief    Common file
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#ifndef MTL_USER_LIB
#include <string.h>
#endif
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
Standard Functions
 ************************************************************************************************/
/*******************************************************************************************************************//**
 * Function Name: mtl_memcmp2
 * @brief     Comparison of two memory areas (n bytes) (substitute standard function).
 * @param     s1 Pointer to first memory area to be compared.
 * @param     s2 Pointer to second memory area to be compared.
 * @param     n  Number of bytes to be compared.
 * @retval    == 0 The two memory areas are equal.
 * @retval    > 0  First memory area (s1) is larger.
 * @retval    < 0  Number of bytes to be compared.
 * @details   Compares two memory areas for n bytes in byte units.
 * @note      none.
***********************************************************************************************************************/
int mtl_memcmp2 (void FAR *s1, void FAR *s2, size_t16 n)
{
#ifdef MTL_USER_LIB /* ---------------------------------|* original processing is used          */
    char FAR*           ss1 = s1;
    char FAR*           ss2 = s2;

    while ( n > 0 ) 
    {
        if ( *ss1 != *ss2 ) 
        {
            return ( (int )*ss1 - (int )*ss2 );         /* The two memory areas are different   */
        }
        ss1++;
        ss2++;
        n--;
    }

    return 0;                                           /* The two memory areas are equal       */

#else   /* ---------------------------------------------|* Standard library is used             */
    return memcmp(s1, s2, (size_t ) n);
#endif /* MTL_USER_LIB */
}
/**********************************************************************************************************************
 * End of function mtl_memcmp2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_memmove2
 * @brief     Data is moved from a area to other area which may overlap (substitute standard function).
 * @param     s1 Pointer to memory area to which data is moved.
 * @param     s2 Pointer to memory area from which data is moved.
 * @param     n  Number of bytes to be moved.
 * @return    Pointer to memory area to which data is moved.
 * @details   Data is moved from a area to other area which may overlap.
 * @note      none.
***********************************************************************************************************************/
void FAR* mtl_memmove2 (void FAR *s1, void FAR *s2, size_t16 n)
{
#ifdef MTL_USER_LIB /* ---------------------------------|* original processing is used          */
    char FAR*           pp1;
    char FAR*           pp2;

    pp1 = (char FAR* )s1;
    pp2 = (char FAR* )s2;
    if ( pp2 >= pp1 ) 
    {     /* Data are copied from the top when left shift copy is performed.  */
        for ( ; n > 0 ; n-- ) 
        {
            *pp1++ = *pp2++;
        }

    } 
    else 
    {                /* Data are copied from the end when right shift copy is performed. */
        pp1 += (n - 1);
        pp2 += (n - 1);
        for ( ; n > 0 ; n-- )
        {
            *pp1-- = *pp2--;
        }
    }

    return s1;

#else   /* ---------------------------------------------|* Standard library is used             */
    return memmove(s1, s2, (size_t ) n);
#endif /* MTL_USER_LIB */
}
/**********************************************************************************************************************
 * End of function mtl_memmove2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_memcpy2
 * @brief     Copy of memory area (n bytes) (substitute standard function).
 * @param     s1 Pointer to memory area to which data is copied.
 * @param     s2 Pointer to memory area from which data is copied.
 * @param     n  Number of bytes to be copied.
 * @return    Pointer to memory area to which data is copied.
 * @details   Copies data for the number of bytes specified by "n" from the memory area \n
 * indicated by "s2" to the memory area indicated by "s1".
 * @note      none.
***********************************************************************************************************************/
void FAR* mtl_memcpy2 (void FAR *s1, void FAR *s2, size_t16 n)
{
#ifdef MTL_USER_LIB /* ---------------------------------|* original processing is used          */
    char FAR*           ss1 = s1;
    char FAR*           ss2 = s2;

    while ( n > 0 ) 
    {
        *ss1++ = *ss2++;
        n--;
    }

    return s1;                                          /*  Pointer to memory area to which data is copied  */

#else   /* ---------------------------------------------|* Standard library is used             */
    return memcpy(s1, s2, (size_t ) n);
#endif /* MTL_USER_LIB */
}
/**********************************************************************************************************************
 * End of function mtl_memcpy2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_memcpy_w2
 * @brief     Copy of memory area (n word) (substitute standard function).
 * @param     s1 Pointer to memory area to which data is copied.
 * @param     s2 Pointer to memory area from which data is copied.
 * @param     n  Number of words to be copied.
 * @return    Pointer to memory area to which data is copied.
 * @details   Copies data for the number of words specified by "n" from the memory area \n
 * indicated by "s2" to the memory area indicated by "s1".
 * @note      Copies data in word(2 bytes) unit.
***********************************************************************************************************************/
void FAR* mtl_memcpy_w2 (void FAR *s1, void FAR *s2, size_t16 n)
{
    ushort FAR *ss1 = s1;
    ushort FAR *ss2 = s2;

    while (n > 0)
    {
        *ss1++ = *ss2++;
        n--;
    }

    return s1; /*  Pointer to memory area to which data is copied  */
}
/**********************************************************************************************************************
 * End of function mtl_memcpy_w2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_memset2
 * @brief     Memory area setting (substitute standard function).
 * @param     s Pointer to memory area to which data is set.
 * @param     c Data to be set.
 * @param     n Number of bytes to be set.
 * @return    Pointer to memory area to which data is set.
 * @details   Sets the data specified by "c" for the number of bytes specified by "n" \n
 * to the memory area indicated by "s".
 * @note      none.
***********************************************************************************************************************/
void FAR* mtl_memset2(void FAR* s, int c, size_t16 n)
{
#ifdef MTL_USER_LIB /* ---------------------------------|* original processing is used          */
    char FAR*           ss = s;

    while ( n > 0 ) {
        *ss++ = (char )c;
        n--;
    }

    return s;                                   /* Pointer to memory area to which data is set  */

#else   /* ---------------------------------------------|* Standard library is used             */
    return memset(s, c, (size_t )n);
#endif /* MTL_USER_LIB */
}
/**********************************************************************************************************************
 * End of function mtl_memset2
 *********************************************************************************************************************/

