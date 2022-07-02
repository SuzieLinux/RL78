/**
 * @file   pcan.c
 * @author Michel Catudal
 * @date   26 June 2022
 * @version 0.1
 * @brief  CAN interface for bootloader
 *
*/
#include "system.h"
#include "r_cg_port.h"
#include "dcan.h"
#include "pcan.h"

//static void vPCAN_memcpy(uint8_t  *s1, uint8_t  *s2, uint16_t n);
//static void vPCAN_memset(uint8_t *s, uint8_t c, uint16_t n);

/*****************************************************************************
 *      @brief   CAN Initialization
 *
 *      @param[in] None
 *
 *      @return None
******************************************************************************/
void vPCAN_eInit(void)
{
    iDCAN_eInit();        /* Speed to used is read from data flash */
}

/*****************************************************************************
 *      @brief  Handle received data if any
 *
 *      @param[in] ACK SID
 *      @param[in] ADD SID
 *      @param[in] Status
 *
 *      @return None
******************************************************************************/
void vPCAN_ePollCanRx(void)
{
    uint8_t rxdNumBytes;
    int8_t spiTransferError;
    // Receive Message Object
    CAN_RX_MSGOBJ rxObj;
    uint8_t rxd[MAX_DATA_BYTES];
    CAN_RX_FIFO_EVENT rxFlags;

    if (DRV_CANFDSPI_INT1) return;  /* no interrupt yet */

    spiTransferError = iDCAN_iReceiveChannelEventGet(CAN_FIFO_CH2, &rxFlags);

    if (spiTransferError < 0) return;

    // Check that FIFO is not empty
    if (rxFlags & CAN_RX_FIFO_NOT_EMPTY_EVENT)
    {
        // Read message and UINC
        spiTransferError = iDCAN_iReceiveMessageGet(CAN_FIFO_CH2, &rxObj, rxd, MAX_DATA_BYTES);
        if (spiTransferError < 0) return;   /* no response or no valid data */
    }
    else return; /* No Data yet */

    if (rxObj.bF.ctrl.DLC > 8)
    {
        rxdNumBytes = 8;
    }
    else
    {
        rxdNumBytes = rxObj.bF.ctrl.DLC;
    }

    if (rxObj.bF.ctrl.IDE)
    {
        iDCAN_iSendExtCANMsg(rxObj.bF.id.EID, rxd, rxdNumBytes);
    }
    else
    {
        iDCAN_iSendStdCANMsg(rxObj.bF.id.SID, rxd, rxdNumBytes);
    }
}

#if 0

static void vPCAN_memcpy(uint8_t  *s1, uint8_t  *s2, uint16_t n)
{
    uint8_t *ss1 = s1;
    uint8_t *ss2 = s2;

    while (n > 0)
    {
        *ss1++ = *ss2++;
        n--;
    }
}

static void vPCAN_memset(uint8_t *s, uint8_t c, uint16_t n)
{
    uint8_t *ss = s;

    while (n > 0)
    {
        *ss++ = c;
        n--;
    }
}

#endif

