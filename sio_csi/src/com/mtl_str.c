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
 * @file     mtl_str.c
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
 * Function Name: mtl_strcat2
 * @brief     Character string linkage (substitute standard function).
 * @param     s1 Pointer to character string to which linkage is performed.
 * @param     s2 Pointer to character string which is to be linked.
 * @return    Pointer to character string after linkage.
 * @details   Links the character string indicated by "s2" to the end of the character string indicated by "s1". \n
 * The character string after linkage ends with NULL.
 * @note      none.
***********************************************************************************************************************/
char FAR* mtl_strcat2 (char FAR *s1, char FAR *s2)
{
#ifdef MTL_USER_LIB /* ---------------------------------|* Original processing is used          */
    char FAR*           ptr = s1;

    while ( *s1 != 0 )
    {
        s1++;                                           /* Search for end of "s1"               */
    }
    while ( *s2 != 0 )
    {
        *s1++ = *s2++;                                  /* Link "s2" to "s1"                    */
    }
    *s1 = *s2;                                          /* Add NULL                             */

    return ptr;                 /* Pointer to character string to which linkage is performed    */

#else   /* ---------------------------------------------|* Standard library is used             */
    return strcat(s1, s2);
#endif /* MTL_USER_LIB */
}
/**********************************************************************************************************************
 * End of function mtl_strcat2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_strcmp2
 * @brief     Comparison of two character strings (substitute standard function).
 * @param     s1 Pointer to first character string to be compared.
 * @param     s2 Pointer to second character string to be compared.
 * @retval    == 0 The two character strings are equal
 * @retval    > 0  First character string (s1) is longer.
 * @retval    < 0  Second character string (s2) is longer.
 * @details   Compares two character strings which end with NULL in byte units.
 * @note      none.
***********************************************************************************************************************/
int mtl_strcmp2(char FAR* s1, char FAR* s2)
{
#ifdef MTL_USER_LIB /* ---------------------------------|* Original processing is used          */
    for ( ; *s1 == *s2 ; s1++, s2++ )
    {
        if ( *s1 == '\0' )
        {
            return 0;                                   /*The two character strings are equal   */
        }
    }

    return ((int )*s1 - (int )*s2);                     /*The two character strings are different*/

#else   /* ---------------------------------------------|* Standard library is used             */
    return strcmp(s1, s2);
#endif /* MTL_USER_LIB */
}
/**********************************************************************************************************************
 * End of function mtl_strcmp2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_strcpy2
 * @brief     Copy of character string (substitute standard function).
 * @param     s1 Pointer to character string to which copy is performed.
 * @param     s2 Pointer to character string which is copied.
 * @return    Pointer to character string to which copy is performed.
 * @details   Copies the character string (ends with NULL) indicated by "s2" to the area indicated by "s1". \n
 * The character string to which copy is performed ends with NULL.
 * @note      none.
***********************************************************************************************************************/
char FAR* mtl_strcpy2(char FAR* s1, char FAR* s2)
{
#ifdef MTL_USER_LIB /* ---------------------------------|* Original processing is used          */
    char FAR*           ptr = s1;

    while ( *s2 != 0 )
    {
        *s1++ = *s2++;                                  /* Copy s2 to s1                        */
    }
    *s1 = *s2;                                          /* Add Null                             */

    return ptr;                     /*Pointer to character string to which copy is performed    */

#else   /* ---------------------------------------------|* Standard library is used             */
    return strcpy(s1, s2);
#endif /* MTL_USER_LIB */
}
/**********************************************************************************************************************
 * End of function mtl_strcpy2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_strlen2
 * @brief     Acquisition of character string length (substitute standard function).
 * @param     s Pointer to character string whose length is to be obtained.
 * @return    Character string length.
 * @details   Obtains the length of the character string (NULL included) indicated by "s".
 * @note      none.
***********************************************************************************************************************/
size_t16 mtl_strlen2(char FAR* s)
{
#ifdef MTL_USER_LIB /* ---------------------------------|* Original processing is used          */
    char FAR*            ptr = s;

    while ( *s != 0 )
    {
        s++;
    }

    return (size_t16 )((ulong )s - (ulong )ptr);        /* Character string length              */

#else   /* ---------------------------------------------|* Standard library is used             */
    return ((size_t16 )strlen(s));
#endif /* MTL_USER_LIB */
}
/**********************************************************************************************************************
 * End of function mtl_strlen2
 *********************************************************************************************************************/

