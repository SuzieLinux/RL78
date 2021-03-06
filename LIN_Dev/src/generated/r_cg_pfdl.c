/**
 * @file   r_cg_pfdl.h
 * @author Michel Catudal
 * @date   29 May 2022
 * @version 0.1
 * @brief  pfdl driver
 *
*/

/***********************************************************************************************************************
* Code based on code generated by Applilet3 for RL78/G14 and IAR Systems iccrl78
* Copyright (C) 2011, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_pfdl.h"
#include "r_cg_userdefine.h"
#include <string.h>

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
pfdl_request_t gFdlReq;      /* Control variable for PFDL */
pfdl_descriptor_t gFdlDesc;
uint8_t gFdlStatus;	         /* This indicates status of FDL library that is "close" or "open". (open=1, close=0) */

/*****************************************************************************
 *      @brief This function initializes the flash data library.
 *
 *      @param[in] None
 *
 *      @return None
******************************************************************************/
void R_FDL_Create(void)
{
    gFdlDesc.fx_MHz_u08 = _48_HOCO_CLOCK_MHz;   /* Set an integer of the range from 1 to 48 according to GUI setting of HOCO. */
    gFdlDesc.wide_voltage_mode_u08 = _FULL_SPEED_MODE; /* Voltage mode */
}

/*****************************************************************************
 *      @brief This function writes a data to flash memory.
 *
 *      @param[in] It is destination address of flash memory for reading a data. The address range is from 0x0000 to 0x0FFF
 *      @param[in] Pointer to buffer
 *      @param[in] The size of data to write (Unit is byte)
 *
 *      @return status of write command
******************************************************************************/
pfdl_status_t R_FDL_Write(pfdl_u16 index, pfdl_u08 *buffer, pfdl_u16 bytecount)
{
    pfdl_status_t PFDL_status;

    if (gFdlStatus == 1)
    {
        gFdlReq.index_u16     = index;
        gFdlReq.data_pu08     = buffer;
        gFdlReq.bytecount_u16 = bytecount;
        gFdlReq.command_enu   = PFDL_CMD_WRITE_BYTES;
        PFDL_status = PFDL_Execute(&gFdlReq);
        /* Wait for completing command */
        while (PFDL_status == PFDL_BUSY)
        {
            NOP();
            NOP();
            PFDL_status = PFDL_Handler();     /* The process for confirming end */
        }
    }
    else
    {
        PFDL_status = PFDL_ERR_PROTECTION;
    }
    return PFDL_status;
}

/*****************************************************************************
 *      @brief This function reads a data flash memory.
 *
 *      @param[in] It is destination address of flash memory for reading a data. The address range is from 0x0000 to 0x0FFF
 *      @param[in] Pointer to buffer
 *      @param[in] The size of data to read (Unit is byte)
 *
 *      @return status of read command
******************************************************************************/
pfdl_status_t R_FDL_Read(pfdl_u16 index, pfdl_u08 *buffer, pfdl_u16 bytecount)
{
    pfdl_status_t PFDL_status;

    if (gFdlStatus == 1)
    {
        gFdlReq.index_u16     = index;
        gFdlReq.data_pu08     = buffer;
        gFdlReq.bytecount_u16 = bytecount;
        gFdlReq.command_enu   = PFDL_CMD_READ_BYTES;
        PFDL_status = PFDL_Execute(&gFdlReq);
    }
    else
    {
        PFDL_status = PFDL_ERR_PROTECTION;
    }

    return PFDL_status;
}

/*****************************************************************************
 *      @brief This function erases a block of data flash.
 *
 *      @param[in] The block number to erase data flash. The range of block number is from 0 to 3
 *
 *      @return status of erase command
******************************************************************************/
pfdl_status_t R_FDL_Erase(pfdl_u16 blockno)
{
    pfdl_status_t PFDL_status;

    if (gFdlStatus == 1)
    {
        gFdlReq.index_u16   = blockno;
        gFdlReq.command_enu = PFDL_CMD_ERASE_BLOCK;
        PFDL_status = PFDL_Execute(&gFdlReq);
        /* Wait for completing command  */
        while (PFDL_status == PFDL_BUSY)
        {
            NOP();
            NOP();
            PFDL_status = PFDL_Handler();     /* The process for confirming end */
        }
    }
    else
    {
        PFDL_status = PFDL_ERR_PROTECTION;
    }

    return PFDL_status;
}

/*****************************************************************************
 *      @brief This function opens the RL78 data flash library.
 *
 *      @param[in] None
 *
 *      @return None
***********************************************************************************************************************/
void R_FDL_Open(void)
{
    gFdlDesc.fx_MHz_u08 = _48_HOCO_CLOCK_MHz;   /* Set an integer of the range from 1 to 48 according to GUI setting of HOCO. */
    gFdlDesc.wide_voltage_mode_u08 = _FULL_SPEED_MODE; /* Voltage mode */
    PFDL_Open(&gFdlDesc);
    gFdlStatus = 1;
}

/*****************************************************************************
 *      @brief This function closes the RL78 data flash library
 *
 *      @param[in] None
 *
 *      @return None
******************************************************************************/
void R_FDL_Close(void)
{
    PFDL_Close();
    gFdlStatus = 0;
}

/*****************************************************************************
 *      @brief Verify data from data flash
 *
 *      @param[in] It is destination address of flash memory for blank check. The address range is from 0x0000 to 0x0FFF
 *      @param[in] The size of data to blank check (Unit is byte)
 *
 *      @return status of blank check command
******************************************************************************/
pfdl_status_t R_FDL_BlankCheck(pfdl_u16 index, pfdl_u16 bytecount)
{
    pfdl_status_t PFDL_status;

    if (gFdlStatus == 1)
    {
        gFdlReq.index_u16 = index;
        gFdlReq.bytecount_u16 = bytecount;
        gFdlReq.command_enu = PFDL_CMD_BLANKCHECK_BYTES;
        PFDL_status = PFDL_Execute(&gFdlReq);
        /* Wait for completing command */
        while (PFDL_status == PFDL_BUSY)
        {
            NOP();
            NOP();
            PFDL_status = PFDL_Handler();     /* The process for confirming end */
        }
    }
    else
    {
        PFDL_status = PFDL_ERR_PROTECTION;
    }

    return PFDL_status;
}

/*****************************************************************************
 *      @brief Verify data from data flash
 *
 *      @param[in] It is destination address of flash memory for iverify a data. The address range is from 0x0000 to 0x0FFF
 *      @param[in] The size of data to iverify (Unit is byte)
 *
 *      @return status of iverify command
******************************************************************************/
pfdl_status_t R_FDL_IVerify(pfdl_u16 index, pfdl_u16 bytecount)
{
    pfdl_status_t PFDL_status;

    if (gFdlStatus == 1)
    {
        gFdlReq.index_u16 = index;
        gFdlReq.bytecount_u16 = bytecount;
        gFdlReq.command_enu = PFDL_CMD_IVERIFY_BYTES;
        PFDL_status = PFDL_Execute(&gFdlReq);

        /* Wait for completing command */
        while (PFDL_status == PFDL_BUSY)
        {
            NOP();
            NOP();
            PFDL_status = PFDL_Handler();     /* The process for confirming end */
        }
    }
    else
    {
        PFDL_status = PFDL_ERR_PROTECTION;
    }

    return PFDL_status;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
