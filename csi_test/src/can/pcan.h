/**
 * @file   pcan.h
 * @author Michel Catudal
 * @date   16 June 2022
 * @version 0.1
 * @brief  CAN interface for bootloader
 *
*/
#ifndef PCAN_H
#define PCAN_H

#include "system.h"

#define DucPCAN_iINACTIVE        0
#define DucPCAN_iRESET_REQUESTED 1    // Reception of Reset ECU

/******************************************************************************
**                          GLOBAL DEFINITIONS
******************************************************************************/
#define DucPCAN_eOK_NOFAILURE            0
#define DucPCAN_eCANRX_OVERRUN          -1 // CAN RECEPTION OVERRUN
#define DucPCAN_eRX_BADCANLENGTH        -2 // NOT 8 BYTES RECEIVED ON CAN FRAME
#define DucPCAN_eRX_BADDoCLENGTH        -3 // MORE THAN MAXIMUM BYTES DEFINED ON FIRST FRAME
#define DucPCAN_eBR_TIMEOUT             -4 // Transmission buffer not ready
#define DucPCAN_eCR_TIMEOUT             -5 // Not received consecutive frame in time
#define DucPCAN_eAS_TIMEOUT             -6 // No Transmission confirmation till As timeout
#define DucPCAN_eUNEXPECTED_FRAME       -7 // SEQUENCE NUMBER OF FRAME NOT RESPECTED
#define DucPCAN_eRX_BAD_SEQUENCE_NUMBER -8 // Bad sequence SN

extern uint8_t ucPCAN_iWait_Reset;   // Wait for sending response after request
extern uint8_t  ucPCAN_iDataTx[8];   // transmission Buffer for answer
extern uint16_t uiPCAN_iLengthTx_Data;    // Size of the data to be transmitted
extern uint8_t ucPCAN_eChangeOfDiagAddrPermitted;

//           EXPORT
/******************************************************************************
**                         PROTO
******************************************************************************/
void vPCAN_eInit(void);
void vPCAN_eOnCanRxComplete(uint8_t *rxd, const uint8_t rxdNumBytes);
void vPCAN_ePollCanRx(void);

#endif


