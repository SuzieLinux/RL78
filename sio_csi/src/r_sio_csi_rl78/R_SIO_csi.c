#define SIO_FID                         (6UL << 6 | 1UL)            /* Definition for mtl_Log() */
/************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*************************************************************************************************/
/* Copyright (C) 2011 (2012-2016) Renesas Electronics Corporation. All rights reserved.         */
/************************************************************************************************
* File Name     : R_SIO_csi.c
* Version       : 2.05
* Device(s)     : 78K0R/KE3-L,RL78/G14
* Tool-Chain    : -
*               : 
* OS            : -
* H/W Platform  : -
* Description   : SIO driver SAU I/F module(CSI mode)
* Operation     : -
* Limitations   : -
*************************************************************************************************
* History       : Date          No.             Comment
*               : 2011.02.21                    Initial version
*               : 2011.02.21    Ver1.00 ---------------------------------------------------------
*               : 2011.08.29    Ver2.00 ---------------------------------------------------------
*               : 2012.01.18    Ver2.01 Modified Notices of APIs.
*               : 2012.01.18    Ver2.01 ---------------------------------------------------------
*               : 
*               : DD.MM.YYYY    Version Description
*               : 27.04.2012    2.02    Added RL78/G14.
*               : 07.05.2012    2.02    Changed format.
*               : 07.05.2012    2.02    Changed define the wait time for timeout.
*               : 07.05.2012    2.02    ---------------------------------------------------------
*               : 30.09.2013    2.03    Supported IAR Embedded Workbench for Renesas RL78.
*               : 30.01.2015    2.04    Added R_SIO_TRx_Data().
*               : 31.03.2016    2.05    Supported CC-RL.
*************************************************************************************************/
#ifdef __CA78K0R__                                      /* Renesas RL78 Compiler                */
    #pragma SFR
    #pragma NOP
    #pragma DI
    #pragma EI
#endif  /* __CA78K0R__ */
#ifdef __CCRL__                                         /* Renesas CC-RL Compiler               */
    #include "iodefine.h"                               /* for RL78                             */
#endif  /* __CCRL__ */
#ifdef __ICCRL78__                                      /* IAR RL78 Compiler                    */
    #include <ior5f104pj.h>                             /* for RL78/G14 100pin (R5F104PJ)       */  /** SET **/
    #include <ior5f104pj_ext.h>                         /* for RL78/G14 100pin (R5F104PJ)       */  /** SET **/
    #include "intrinsics.h"                             /* Header file for built in function    */
#endif  /* __ICCRL78__ */


/************************************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
*************************************************************************************************/
#include "mtl_com.h"                            /* Renesas tool common definitions              */
#include "R_SIO.h"                              /* SIO driver common definitions                */
#include "R_SIO_csi.h"              /* SIO driver SAU I/F module(CSI mode) common definitions   */


/************************************************************************************************
Macro definitions
*************************************************************************************************/


/************************************************************************************************
Typedef definitions
*************************************************************************************************/


/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/
#if ((defined(SIO_CRCCCITT_USED) && defined(SIO_CRC16_HARD) && !defined(SIO_CRC16_MSBF)) || defined(SIO_LSBF))
/*""VAL DEF""*/
const uint8_t       T_ExchgBit[] = {                /* bit upper lower exchange table           */
/*""VAL DEF END""*/
/*  0x*0  0x*1  0x*2  0x*3  0x*4  0x*5  0x*6  0x*7  0x*8  0x*9  0x*A  0x*B  0x*C  0x*D  0x*E  0x*F          */
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, /* 0x0* */
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, /* 0x1* */
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, /* 0x2* */
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, /* 0x3* */
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, /* 0x4* */
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, /* 0x5* */
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, /* 0x6* */
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, /* 0x7* */
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, /* 0x8* */
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, /* 0x9* */
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, /* 0xA* */
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, /* 0xB* */
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, /* 0xC* */
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, /* 0xD* */
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, /* 0xE* */
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF  /* 0xF* */
};
#endif  /* #if ((defined(SIO_CRCCCITT_USED) && defined(SIO_CRC16_HARD) && !defined(SIO_CRC16_MSBF)) || defined(SIO_LSBF)) */


/************************************************************************************************
Private global variables and functions
*************************************************************************************************/


/*----------------------------------------------------------------------------------------------*/
/* SIO initial setting processing                                                               */
/*----------------------------------------------------------------------------------------------*/
/************************************************************************************************
* Function Name : R_SIO_Init_Driver
* Description   : SIO is disabled.
*               : Initializes the ports related to the specified SIO.
*               : Execute once when the system starts up.
* Arguments     : None
* Return Value  : SIO_OK                                ;   Successful operation
*************************************************************************************************/
error_t R_SIO_Init_Driver(void)
{
    return R_SIO_Disable();
}   /* End of function R_SIO_Init_Driver() */


/************************************************************************************************
* Function Name : R_SIO_Disable
* Description   : SIO is disabled.
*               : Initializes the ports related to the specified SIO.
* Arguments     : None
* Return Value  : SIO_OK                                ;   Successful operation
*************************************************************************************************/
error_t R_SIO_Disable(void)
{
    uint16_t            usTmp;  
    
    SIO_SAUEN = 1;                                      /* CSI enable                           */
#ifdef __CA78K0R__
    NOP();                                              /* Wait                                 */
    NOP();                                              /* Wait                                 */
    NOP();                                              /* Wait                                 */
    NOP();                                              /* Wait                                 */
#endif /* __CA78K0R__ */
#ifdef __CCRL__
    __nop();                                            /* Wait                                 */
    __nop();                                            /* Wait                                 */
    __nop();                                            /* Wait                                 */
    __nop();                                            /* Wait                                 */
#endif /* __CCRL__ */
#ifdef __ICCRL78__
    __no_operation();                                   /* Wait                                 */
    __no_operation();                                   /* Wait                                 */
    __no_operation();                                   /* Wait                                 */
    __no_operation();                                   /* Wait                                 */
#endif /* __ICCRL78__ */
    SIO_DISABLE(usTmp);                                 /* SIO disable setting                  */
    SIO_IO_INIT();                                      /* Driver use port initial setting      */
    
    return SIO_OK;
}   /* End of function R_SIO_Disable() */


/************************************************************************************************
* Function Name : R_SIO_Enable
* Description   : Initializes the SIO register.
*               : Initializes the ports related to the specified SIO.
* Arguments     : uint8_t           BrgData             ;   SIO bit rate register setting data
* Return Value  : SIO_OK                                ;   Successful operation
*************************************************************************************************/
error_t R_SIO_Enable(uint8_t BrgData)
{
    uint16_t            usTmp;
    
    SIO_SAUEN = 1;                                      /* CSI enable                           */
#ifdef __CA78K0R__
    NOP();                                              /* Wait                                 */
    NOP();                                              /* Wait                                 */
    NOP();                                              /* Wait                                 */
    NOP();                                              /* Wait                                 */
#endif /* __CA78K0R__ */
#ifdef __CCRL__
    __nop();                                            /* Wait                                 */
    __nop();                                            /* Wait                                 */
    __nop();                                            /* Wait                                 */
    __nop();                                            /* Wait                                 */
#endif /* __CCRL__ */
#ifdef __ICCRL78__
    __no_operation();                                   /* Wait                                 */
    __no_operation();                                   /* Wait                                 */
    __no_operation();                                   /* Wait                                 */
    __no_operation();                                   /* Wait                                 */
#endif /* __ICCRL78__ */
    SIO_IO_INIT();                                      /* Driver use port initial setting      */
    SIO_ENABLE(BrgData,usTmp);                          /* SIO enable setting                   */
    
    return SIO_OK;
}   /* End of function R_SIO_Enable() */


/*----------------------------------------------------------------------------------------------*/
/* Port control processing                                                                      */
/*----------------------------------------------------------------------------------------------*/
/************************************************************************************************
* Function Name : R_SIO_Open_Port
* Description   : Open the ports related to SIO.
* Arguments     : None
* Return Value  : SIO_OK                                ;   Successful operation
*************************************************************************************************/
error_t R_SIO_Open_Port(void)
{
    SIO_IO_OPEN();
    
    return SIO_OK;
}   /* End of function R_SIO_Open_Port() */


/*----------------------------------------------------------------------------------------------*/
/* SIO transmit & receive control processing                                                    */
/*----------------------------------------------------------------------------------------------*/
/************************************************************************************************
* Function Name : R_SIO_Tx_Data
* Description   : Consecutively transmits data for the specified number of bytes.
* Arguments     : uint16_t          TxCnt               ;   Transmission byte count
*               : uint8_t FAR*      pData               ;   Transmission data buffer pointer
* Return Value  : SIO_OK                                ;   Successful operation
*               : SIO_ERR_HARD                          ;   Hardware error
*************************************************************************************************/
error_t R_SIO_Tx_Data(uint16_t TxCnt, uint8_t FAR* pData)
{
    uint16_t            TxWait;                         /* Transmission waiting counter         */
    uint16_t            usTmp;

    SIO_TX_ENABLE(usTmp);                               /* SIO transmit enable                  */

     do {
        /* Clear IR bit of IR register */
        SIO_TXEND = SIO_FALSE;                          /*                                      */
        /* Writes data to SDR in order to transmit. */
        SIO_TXBUF  = SIO_BIT_EXCHG(*pData);             /* Transmission data setting            */

        /* Transmission preparation completion check */
        TxWait = SIO_TX_WAIT;
        while (SIO_TXEND == SIO_FALSE) {
            TxWait--;
            if (TxWait == 0) {                          /* Transmission waiting time over       */
                SIO_TX_DISABLE();                       /* SIO transmit disable                 */
                mtl_Log(SIO_LOG_ERR, 0, SIO_FID, __LINE__);
                return SIO_ERR_HARD;
            }
            mtl_wait_lp(SIO_T_SAU_WAIT);
        }
        pData++;
        TxCnt--;
    } while (TxCnt != 0);

    SIO_TX_DISABLE();                                   /* SIO transmit disable                 */

    return SIO_OK;
}   /* End of function R_SIO_Tx_Data() */


/************************************************************************************************
* Function Name : R_SIO_Rx_Data
* Description   : Consecutively receives data for the specified number of bytes.
* Arguments     : uint16_t          RxCnt               ;   Reception byte count
*               : uint8_t FAR*      pData               ;   Reception data buffer pointer
* Return Value  : SIO_OK                                ;   Successful operation
*               : SIO_ERR_HARD                          ;   Hardware error
*************************************************************************************************/
error_t R_SIO_Rx_Data(uint16_t RxCnt, uint8_t FAR* pData)
{
    uint16_t            RxWait;                         /* Receive waiting counter              */
    uint8_t             RxBuf;                          /* Receive temp buffer                  */
    uint16_t            usTmp;

    SIO_TRX_ENABLE(usTmp);                              /* SIO transmit&receive enable          */

    do {                                                /* Loop for receive byte                */
        /* Clear IR bit of IR register */
        SIO_RXNEXT = SIO_FALSE;
        /* Writes dummy data to SDR in order to receive data. */
        SIO_TXBUF  = SIO_DUMMY_DATA;

        /* Receive completion check */
        RxWait = SIO_RX_WAIT;
        while (SIO_RXNEXT == SIO_FALSE) {               /* Loop for receive completion          */
            RxWait--;
            if (RxWait == 0) {                          /* Receive waiting time over            */
                SIO_TRX_DISABLE();                      /* SIO transmit&receive disable         */
                mtl_Log(SIO_LOG_ERR, 0, SIO_FID, __LINE__);
                return SIO_ERR_HARD;
            }
            mtl_wait_lp(SIO_T_SAU_WAIT);
        }
        /* Stores the received data from SDR to pData buffer. */
        RxBuf      = SIO_RXBUF;                         /* Receive data storage                 */
        *pData     = SIO_BIT_EXCHG(RxBuf);
        pData++;
        RxCnt--;
    } while (RxCnt != 0);
    
    SIO_TRX_DISABLE();                                  /* SIO transmit&receive disable         */

    return SIO_OK;
}   /* End of function R_SIO_Rx_Data() */


/************************************************************************************************
* Function Name : R_SIO_TRx_Data
* Description   : Consecutively receives data for the specified number of bytes.
* Arguments     : uint16_t          TRxCnt              ;   Reception byte count
*               : uint8_t FAR*      pTxData             ;   Transmission data buffer pointer
*               : uint8_t FAR*      pRxData             ;   Reception data buffer pointer
* Return Value  : SIO_OK                                ;   Successful operation
*               : SIO_ERR_HARD                          ;   Hardware error
*************************************************************************************************/
error_t R_SIO_TRx_Data(uint16_t TRxCnt, uint8_t FAR* pTxData, uint8_t FAR* pRxData)
{
    uint16_t            TRxWait;                        /* Receive waiting counter              */
    uint8_t             RxBuf;                          /* Receive temp buffer                  */
    uint16_t            usTmp;

    SIO_TRX_ENABLE(usTmp);                              /* SIO transmit&receive enable          */

    do {                                                /* Loop for receive byte                */
        /* Clear IR bit of IR register */
        SIO_RXNEXT = SIO_FALSE;
        /* Writes data to SDR in order to transmit. */
        SIO_TXBUF  = SIO_BIT_EXCHG(*pTxData);           /* Transmission data setting            */

        /* Transmit&receive completion check */
        TRxWait = SIO_RX_WAIT;
        while (SIO_RXNEXT == SIO_FALSE) {               /* Loop for receive completion          */
            TRxWait--;
            if (TRxWait == 0) {                         /* Transmit&receive waiting time over   */
                SIO_TRX_DISABLE();                      /* SIO transmit&receive disable         */
                mtl_Log(SIO_LOG_ERR, 0, SIO_FID, __LINE__);
                return SIO_ERR_HARD;
            }
            mtl_wait_lp(SIO_T_SAU_WAIT);
        }
        /* Stores the received data from SDR to pData buffer. */
        RxBuf      = SIO_RXBUF;                         /* Receive data storage                 */
        *pRxData   = SIO_BIT_EXCHG(RxBuf);
        pTxData++;
        pRxData++;
        TRxCnt--;
    } while (TRxCnt != 0);
    
    SIO_TRX_DISABLE();                                  /* SIO transmit&receive disable         */

    return SIO_OK;
}   /* End of function R_SIO_TRx_Data() */

/* End of File */
