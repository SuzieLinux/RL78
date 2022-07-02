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
 * @file     mtl_com.c
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
/*""VAL DEF""*/
ulong FAR       MtlLogTbl[MTL_LOG_MAX];     /* Log buffer               */  /*   4*n bytes  */  /* Due to MTL_LOG_MAX */
ushort FAR      MtlLogIdx;                  /* Log buffer index counter */  /*   2 bytes    */

/*""VAL DEF END""*/



/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/



/***********************************************************************************************************************
 Debugging sub-modules
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * Function Name: mtl_Log2
 * @brief      Debugging function.
 * @param      data Debugging information.
 * @retval     0 Successful operation.
 * @retval     1 Failed operation.
 * @details    Debugging informations are stored.
 * @note
 * Usage of mtl_Log() macro is recommended. (mtl_Log(FLG, VAL, FID, LINE))\n
 * This function can be used in any way but basically the following purpose is intended.\n
 * \n
 * Write this function where there is information to be saved.\n
 * Write this function also at a location determined to cause an error.\n
 * By internally setting breakpoints in this function at debugging,\n
 * breakpoints do not have to be set at various locations.\n
 * On exiting this function, execution returns to the calling function.\n
 * \n
 * Since the file ID and error occurrence line are saved when an error occurs,\n
 * if an I/F for referencing the data is generated,information can be acquired without even using a debugger.\n
 * Arbitrary debugging information can be saved in a ulong type variable.When not used, 0 should be set.\n
 * \n
 * 0xFF is used as the last mark.\n
 * \n
 * To generate a break under particular conditions based on the debugging information,\n
 * create the conditions after the breakpoint processing,and set the breakpoint at debugging.\n
***********************************************************************************************************************/
ulong mtl_Log2(ulong data)
{

    /* -------------------------------------------------|* LOG save processing                  */
    if ( MtlLogIdx >= MTL_LOG_MAX )
    {
        MtlLogIdx = 0;                                  /* The range is exceeded                */
    }

    MtlLogTbl[MtlLogIdx] = data;                        /* Save the debugging information       */

    ++MtlLogIdx;
    if ( MtlLogIdx >= MTL_LOG_MAX )
    {
        MtlLogIdx = 0;                                  /* Maximum buffer value exceeded?       */
    }

    MtlLogTbl[MtlLogIdx] = 0xFFFFFFFF;                  /* Write the last mark                  */


    /* -------------------------------------------------|* Breakpoint processing                */
    if ( (data & 0x80000000) == 0x80000000 )
    {
        return 1;                                       /* Breakpoint setting location          */
    }

    return 0;
}
/**********************************************************************************************************************
 * End of function mtl_Log2
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: mtl_LogClear
 * @brief   Debugging function 2.
 * @param   none.
 * @retval  none.
 * @details Debugging informations are cleared.
 * @note    All logs are cleared.
***********************************************************************************************************************/
void mtl_LogClear(void)
{
    ushort              i;

    MtlLogIdx = 0;                                      /* Clear the counter                    */
    for ( i = 0 ; i < MTL_LOG_MAX ; ++i )
    {
        MtlLogTbl[i] = 0;                               /* Clear the log buffer                 */
    }
}
/**********************************************************************************************************************
 * End of function mtl_LogClear
 *********************************************************************************************************************/

