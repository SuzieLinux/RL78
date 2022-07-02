/**
 * @file   R_SIO_csi.c
 * @author Michel Catudal
 * @date   2 July 2022
 * @brief  CAN interface
 *
*/

#include "ior5f104bg.h"
#include "ior5f104bg_ext.h"
#include "intrinsics.h"         /* Header file for built in function    */
#include "R_SIO.h"                              /* SIO driver common definitions                */
#include "R_SIO_csi.h"              /* SIO driver SAU I/F module(CSI mode) common definitions   */

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
int16_t R_SIO_Init_Driver(void)
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
int16_t R_SIO_Disable(void)
{
	SIO_SAUEN = 1;                             /* CSI enable                      */
	__no_operation();                          /* Wait                            */
	__no_operation();                          /* Wait                            */
	__no_operation();                          /* Wait                            */
	__no_operation();                          /* Wait                            */
    SIO_SO    = SIO_SO | SIO_USO_INIT;         /* setting of initial output level /
    SIO_ST    = 1;                             /* Start disable                   */
    SIO_SOE   = 0;                             /* channel output disable          */
    usTmp     = SIO_SO;                        /* setting of initial output level */
    SIO_SO    = usTmp | SIO_USO_INIT;          /*                                 */
    SIO_SCR   = SIO_USCR_NOUSE;                /* Same as after reset setting     */
    SIO_SMR   = SIO_USMR_INIT;                 /* Same as after reset setting     */
    SIO_SOL   = usTmpusTmp & (~SIO_USOL_INIT); /* SOL setting(CSI mode setting)   */
	/* Driver use port initial setting */
    SIO_SNFEN       = 0; /* Noise filter OFF    */
    SIO_PIM_DATAI   = 0; /* Normal input buffer */
    SIO_PM_DATAI    = 1; /* DataIn  Input */
    SIO_POM_DATAO   = 0;      /* Normal output mode  */
    SIO_P_DATAO     = 1; /* DataOut "H"         */
    SIO_PM_DATAO    = 0;      /* DataOut Output      */
    SIO_P_DATAO     = 1; /* DataOut "H"         */
    SIO_POM_CLK     = 0;         /* Normal output mode */
    SIO_P_CLK       = 1;    /* CLK "H"     */
    SIO_PM_CLK      = 0;         /* CLK Output  */
    SIO_P_CLK       = 1;    /* CLK "H"     */

	return SIO_OK;
}

/************************************************************************************************
* Function Name : R_SIO_Enable
* Description   : Initializes the SIO register.
*               : Initializes the ports related to the specified SIO.
* Arguments     : uint8_t           BrgData             ;   SIO bit rate register setting data
* Return Value  : SIO_OK                                ;   Successful operation
*************************************************************************************************/
int16_t R_SIO_Enable(uint8_t BrgData)
{
	SIO_SAUEN = 1;     /* CSI enable */
	__no_operation();  /* Wait       */
	__no_operation();  /* Wait       */
	__no_operation();  /* Wait       */
	__no_operation();  /* Wait       */
	/* Driver use port initial setting */
    SIO_SNFEN       = 0; /* Noise filter OFF     */
    SIO_PIM_DATAI   = 0; /* Normal input buffer  */
    SIO_PM_DATAI    = 1; /* DataIn Input         */
    SIO_POM_DATAO   = 0;      /* Normal output mode */
    SIO_P_DATAO     = 1; /* DataOut "H"     */
    SIO_PM_DATAO    = 0;      /* DataOut Output  */
    SIO_P_DATAO     = 1; /* DataOut "H"     */
    SIO_POM_CLK     = 0;      /* Normal output mode */
    SIO_P_CLK       = 1; /* CLK "H"     */
    SIO_PM_CLK      = 0;      /* CLK Output  */
    SIO_P_CLK       = 1; /* CLK "H"     */

    SIO_SPS   = SIO_SPS | SIO_USPS_INIT;    /* Division setting of CKmn frequency */
    SIO_SMR   = SIO_USMR_INIT;              /* CSI mode setting                   */
    SIO_SCR   = SIO_USCR_INIT;              /* Transmit&receive disable           */
    SIO_SDR   = ((uint16_t)BrgData << 9);   /* Bit rate setting                   */
    SIO_SIR   = SIO_USIR_CLEAR;             /* Error flag clear setting           */
    SIO_SOL   = SIO_SOL & (~SIO_USOL_INIT); /* SOL setting(CSI mode setting)      */

	return SIO_OK;
}

/*----------------------------------------------------------------------------------------------*/
/* Port control processing                                                                      */
/*----------------------------------------------------------------------------------------------*/
/************************************************************************************************
* Function Name : R_SIO_Open_Port
* Description   : Open the ports related to SIO.
* Arguments     : None
* Return Value  : SIO_OK      ;   Successful operation
*************************************************************************************************/
int16_t R_SIO_Open_Port(void)
{
    SIO_SNFEN       = 0;  /* Noise filter OFF    */
    SIO_PIM_DATAI   = 0;  /* Normal input buffer */
    SIO_PM_DATAI    = 1;  /* DataIn Input        */
    SIO_PM_DATAO    = 1;  /* DataOut Input       */
    SIO_PIM_CLK     = 0;  /* Normal input buffer */
    SIO_PM_CLK      = 1;  /* CLK Input           */

	return SIO_OK;
}

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
int16_t R_SIO_Tx_Data(uint16_t TxCnt, uint8_t FAR *pData)
{
	uint16_t  TxWait;                  /* Transmission waiting counter  */

	SIO_SCR   = SIO_USCR_TX;           /* Transmit enable               */
	SIO_SO    = SIO_SO | SIO_USO_INIT; /*                               */
	SIO_SOE   = 1;                     /* channel output enable         */
	SIO_SS    = 1;                     /* Start enable, BFF clear       */

	do
	{
		/* Clear IR bit of IR register */
		SIO_TXEND = 0;
		/* Writes data to SDR in order to transmit. */
		SIO_TXBUF  = *pData;             /* Transmission data setting */

		/* Transmission preparation completion check */
		TxWait = SIO_TX_WAIT;
		while (SIO_TXEND == 0)
		{
			TxWait--;
			if (TxWait == 0)             /* Transmission waiting time over */
			{
				SIO_ST  = 1;             /* Start disable          */
				SIO_SOE = 0;             /* channel output disable */
				SIO_SCR = SIO_USCR_INIT; /* Transmit disable       */
				return SIO_ERR_HARD;
			}
			wait_loop(SIO_T_SAU_WAIT);
		}
		pData++;
		TxCnt--;
	}
	while (TxCnt != 0);

	SIO_ST  = 1;             /* Start disable           */
	SIO_SOE = 0;             /* channel output disable  */
	SIO_SCR = SIO_USCR_INIT; /* Transmit disable        */

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
int16_t R_SIO_Rx_Data(uint16_t RxCnt, uint8_t FAR *pData)
{
	uint16_t  RxWait;  /* Receive waiting counter */
	uint8_t   RxBuf;   /* Receive temp buffer     */

	SIO_SCR   = SIO_USCR_TRX;            /* Transmit&receive enable */
	SIO_SO    = SIO_SO | SIO_USO_INIT;   /*                         */
	SIO_SOE   = 1;                       /* channel output enable   */
	SIO_SS    = 1;                       /* Start enable, BFF clear */

	do                                   /* Loop for receive byte   */
	{
		/* Clear IR bit of IR register */
		SIO_RXNEXT = 0;
		/* Writes dummy data to SDR in order to receive data. */
		SIO_TXBUF  = SIO_DUMMY_DATA;

		/* Receive completion check */
		RxWait = SIO_RX_WAIT;
		while (SIO_RXNEXT == 0)        /* Loop for receive completion */
		{
			RxWait--;
			if (RxWait == 0)                   /* Receive waiting time over   */
			{
				SIO_ST    = 1;                 /* Start disable               */
					SIO_SOE   = 0;             /* channel output disable      */
					SIO_SCR   = SIO_USCR_INIT; /* Transmit&receive disable    */
					return SIO_ERR_HARD;
			}
			wait_loop(SIO_T_SAU_WAIT);
		}
		/* Stores the received data from SDR to pData buffer. */
		RxBuf      = SIO_RXBUF;                         /* Receive data storage                 */
		*pData     = RxBuf;
		pData++;
		RxCnt--;
	}
	while (RxCnt != 0);

	SIO_ST    = 1;                /* Start disable            */
	SIO_SOE   = 0;                /* channel output disable   */
	SIO_SCR   = SIO_USCR_INIT;    /* Transmit&receive disable */

	return SIO_OK;
}


/************************************************************************************************
* Function Name : R_SIO_TRx_Data
* Description   : Consecutively receives data for the specified number of bytes.
* Arguments     : uint16_t          TRxCnt              ;   Reception byte count
*               : uint8_t FAR*      pTxData             ;   Transmission data buffer pointer
*               : uint8_t FAR*      pRxData             ;   Reception data buffer pointer
* Return Value  : SIO_OK                                ;   Successful operation
*               : SIO_ERR_HARD                          ;   Hardware error
*************************************************************************************************/
int16_t R_SIO_TRx_Data(uint16_t TRxCnt, uint8_t FAR *pTxData, uint8_t FAR *pRxData)
{
	uint16_t TRxWait;  /* Receive waiting counter              */
	uint8_t  RxBuf;    /* Receive temp buffer                  */

	SIO_SCR = SIO_USCR_TRX;           /* Transmit&receive enable */
	SIO_SO  = SIO_SO | SIO_USO_INIT;
	SIO_SOE = 1;                      /* channel output enable   */
	SIO_SS  = 1;                      /* Start enable, BFF clear */

	do                                /* Loop for receive byte   */
	{
		/* Clear IR bit of IR register */
		SIO_RXNEXT = 0;
		/* Writes data to SDR in order to transmit. */
		SIO_TXBUF  = *pTxData;           /* Transmission data setting */

		/* Transmit&receive completion check */
		TRxWait = SIO_RX_WAIT;
		while (SIO_RXNEXT == 0)        /* Loop for receive completion */
		{
			TRxWait--;
			if (TRxWait == 0)                  /* Transmit&receive waiting time over */
			{
				SIO_ST    = 1;                 /* Start disable             */
					SIO_SOE   = 0;             /* channel output disable    */
					SIO_SCR   = SIO_USCR_INIT; /* Transmit&receive disable  */
					return SIO_ERR_HARD;
			}
			wait_loop(SIO_T_SAU_WAIT);
		}
		/* Stores the received data from SDR to pData buffer. */
		RxBuf      = SIO_RXBUF;                         /* Receive data storage */
		*pRxData   = RxBuf;
		pTxData++;
		pRxData++;
		TRxCnt--;
	}
	while (TRxCnt != 0);

	SIO_ST    = 1;                  /* Start disable            */
		SIO_SOE   = 0;              /* channel output disable   */
		SIO_SCR   = SIO_USCR_INIT;  /* Transmit&receive disable */

	return SIO_OK;
}   /* End of function R_SIO_TRx_Data() */

/*******************************************************************************************************************//**
 * Function Name: wait_loop2
 * @brief     loop timer processing.
 * @param     time Timer value.
 * @return    none.
 * @details   loop timer processing.
 * @note      Refer the mtl_com.h about timer value.
***********************************************************************************************************************/
void wait_loop(uint time)
{
    do
    {
        --time;
    } while ( 0 != time );
}

