/**
 * @file   main.c
 * @author Michel Catudal
 * @date   26 June 2022
 *
*/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "system.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "main.h"
#include "spi.h"
#include "pcan.h"
#include "dcan.h"
#include "Scheduler.h"
#include "Tasks.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte0 = 0xEFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte1 = 0xFFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte2 = 0xE8U;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte3 = 0x85U;

/* Set security ID */
#pragma location = "SECUID"
__root const uint8_t secuid[10] =
    {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

/*****************************************************************************
 *      @brief main function
 *
 *      @param[in] None
 *
 *      @return None
******************************************************************************/
void main(void)
{
    InitScheduler();
    DRV_CANFDSPI_CS = 1;
    R_CSI20_Start();
    vPCAN_eInit();

    EI();
    Scheduler();
}



