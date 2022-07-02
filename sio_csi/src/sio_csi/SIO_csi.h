#ifndef __SIO_CSI_H__
#define __SIO_CSI_H__

/************************************************************************************************
Macro definitions
*************************************************************************************************/

/*----------------------------------------------------------------------------------------------*/
/*   Define the control port.                                                                   */
/*   Delete comment of a related macrodefinition, and please validate setting.                  */
/*----------------------------------------------------------------------------------------------*/
#define SIO_PM_DATAO        PM14_bit.no4    /* SIO DataOut                                  */  /** SET **/
#define SIO_PM_DATAI        PM14_bit.no3    /* SIO DataIn                                   */  /** SET **/
#define SIO_PM_CLK          PM14_bit.no2    /* SIO CLK                                      */  /** SET **/
#define SIO_P_DATAO         P14_bit.no4     /* SIO DataOut                                  */  /** SET **/
#define SIO_P_DATAI         P14_bit.no3     /* SIO DataIn                                   */  /** SET **/
#define SIO_P_CLK           P14_bit.no2     /* SIO CLK                                      */  /** SET **/
#define SIO_PIM_DATAI       PIM14_bit.no3   /* SIO DataIn                                   */  /** SET **/
#define SIO_PIM_CLK         PIM14_bit.no2   /* SIO CLK                                      */  /** SET **/
#define SIO_POM_DATAO       POM14_bit.no4   /* SIO DataOut                                  */  /** SET **/
#define SIO_POM_CLK         POM14_bit.no2   /* SIO CLK                                      */  /** SET **/

#define SIO_SAUEN           SAU1EN          /* Control of CSI30 input clock supply          */  /** SET **/

/*----------------- SIO definitions -----------------*/

/* CSI30 setting example - Set the following for the system. */
#define SIO_SPS             SPS1            /* Serial clock select register                 */  /** SET **/
#define SIO_SMR             SMR12           /* Serial mode register                         */  /** SET **/
#define SIO_SCR             SCR12           /* Serial communication operation setting register  */  /** SET **/
#define SIO_SDR             SDR12           /* Serial data register                         */  /** SET **/
#define SIO_TXBUF           SIO30           /* SIOp data register                           */  /** SET **/
#define SIO_RXBUF           SIO30           /* SIOp data register                           */  /** SET **/
#define SIO_SIR             SIR12           /* Serial flag clear trigger register           */  /** SET **/
#define SIO_SSR             SSR12           /* Serial status register                       */  /** SET **/
#define SIO_SS              SS1L_bit.no2    /* Serial channel start register SSmn           */  /** SET **/
#define SIO_ST              ST1L_bit.no2    /* Serial channel stop register STmn            */  /** SET **/
#define SIO_SE              SE1L_bit.no2    /* Serial channel enable status register SEmn   */  /** SET **/
#define SIO_SOE             SOE1L_bit.no2   /* Serial output enable register SOEmn          */  /** SET **/
#define SIO_SO              SO1             /* Serial output register SOmn                  */  /** SET **/
#define SIO_SOL             SOL1            /* Serial output level register                 */  /** SET **/
#define SIO_SNFEN           NFEN0_bit.no6   /* Use of noise filter of RXD pin SNFEN         */  /** SET **/

#define SIO_TXNEXT          (SSR12L & 0x20) /* CSI Transmit data empty                      */  /** SET **/
#define SIO_RXNEXT          IF1H_bit.no4    /* CSI Receive completion                       */  /** SET **/
#define SIO_TXEND           IF1H_bit.no4    /* CSI Transmit completion                      */  /** SET **/

/*----------- Definitions of resource used -----------*/

/* SIO setting */

#define SIO_USCR_NOUSE          (uint16_t)0x0087
/*  0000000010000111B   */  /* SCR CSI unused setting               */
/*  ++++++++++++++++-- Same as after reset setting                  */

#define SIO_USPS_INIT           (uint16_t)0x0000                                                    /** SET **/
/*  0000000000000000B   */  /* SPS CSI initial setting              */  /** SET **/
/*  ||||||||||||++++-- CKm0:No division of fclk                     */  /** SET **/
/*  ||||||||++++------ CKm1:No division of fclk                     */  /** SET **/
/*  ++++++++---------- Reserved     : 0     Fixed                   */

#define SIO_USMR_INIT           (uint16_t)0x0020                                                    /** SET **/
/*  0000000000100000B   */  /* SMR CSI initial setting              */
/*  |||||||||||||||+-- Interrupt source : Transfer end interrupt    */
/*  |||||||||||||++--- Operation mode : CSI mode                    */
/*  |||||||||||++----- Reserved     : 0     Fixed                   */
/*  ||||||||||+------- Reserved     : 1     Fixed                   */
/*  |||||||||+-------- Reserved (Controls in UART mode)             */
/*  ||||||||+--------- Reserved     : 0     Fixed                   */
/*  |||||||+---------- Start trigger source : Software trigger      */
/*  ||+++++----------- Reserved     : 0     Fixed                   */
/*  |+---------------- ftclk clock channel setting  : Divided fmck  */
/*  +----------------- fMCK clock channel setting   : CKm0 set      */  /** SET **/

#define SIO_USCR_INIT           (uint16_t)0x0007
/*  0000000000000111B   */  /* SCR CSI initial setting              */
/*  ||||||||||||||++-- Data length          : 8 bit                 */
/*  |||||||||||||+---- Reserved     : 1     Fixed                   */
/*  ||||||||||||+----- Reserved     : 0     Fixed                   */
/*  ||||||||||++------ Stop bit : No Stop bit(CSI mode setting)     */
/*  |||||||||+-------- Reserved     : 0     Fixed                   */
/*  ||||||||+--------- data transfer sequence : MSB first           */
/*  ||||||++---------- Parity bit : No Parity(CSI mode setting)     */
/*  |||||+------------ Error interrupt : Disable(CSI mode setting)  */
/*  ||||+------------- Reserved     : 0     Fixed                   */
/*  ||++-------------- Data and clock phase : Type 1(SPI mode 3)    */
/*  |+---------------- RXE setting          : Disable               */
/*  +----------------- TXE setting          : Disable               */

#define SIO_USO_INIT            (uint16_t)0x0404    /* Set Unit/Channel No. to use to 1.        */  /** SET **/
/*  0000010000000100B   */  /* SOm initial setting                  */  /** SET **/
/*  |||||||||||||||+-- SOm0 output                                  */  /** SET **/
/*  ||||||||||||||+--- SOm1 output                                  */  /** SET **/
/*  |||||||||||||+---- SOm2 output                                  */  /** SET **/
/*  ||||||||||||+----- SOm3 output                                  */  /** SET **/
/*  ||||||||++++------ Reserved     : 0     Fixed                   */
/*  |||||||+---------- CKOm0 output                                 */  /** SET **/
/*  ||||||+----------- CKOm1 output                                 */  /** SET **/
/*  |||||+------------ CKOm2 output                                 */  /** SET **/
/*  ||||+------------- CKOm3 output                                 */  /** SET **/
/*  ++++-------------- Reserved     : 0     Fixed                   */

#define SIO_USIR_CLEAR          (uint16_t)0x0007
/*  0000000000000111B   */  /* SIR error flag clear setting         */
/*  |||||||||||||||+-- Overrun error        : Clear                 */
/*  ||||||||||||||+--- Parity error         : Clear                 */
/*  |||||||||||||+---- Framing error        : Clear                 */
/*  +++++++++++++----- Reserved     : 0     Fixed                   */

#define SIO_USCR_TX             (uint16_t)0x8007
/*  1000000000000111B   */  /* SCR CSI TX setting                   */
/*  ||||||||||||||++-- Data length          : 8 bit                 */
/*  |||||||||||||+---- Reserved     : 1     Fixed                   */
/*  ||||||||||||+----- Reserved     : 0     Fixed                   */
/*  ||||||||||++------ Stop bit : No Stop bit(CSI mode setting)     */
/*  |||||||||+-------- Reserved     : 0     Fixed                   */
/*  ||||||||+--------- data transfer sequence : MSB first           */
/*  ||||||++---------- Parity bit : No Parity(CSI mode setting)     */
/*  |||||+------------ Error interrupt : Disable(CSI mode setting)  */
/*  ||||+------------- Reserved     : 0     Fixed                   */
/*  ||++-------------- Data and clock phase : Type 1(SPI mode 3)    */
/*  |+---------------- RXE setting          : Disable               */
/*  +----------------- TXE setting          : Enable                */

#define SIO_USCR_TRX            (uint16_t)0xC007
/*  1100000000000111B   */  /* SCR CSI TRX setting                  */
/*  ||||||||||||||++-- Data length          : 8 bit                 */
/*  |||||||||||||+---- Reserved     : 1     Fixed                   */
/*  ||||||||||||+----- Reserved     : 0     Fixed                   */
/*  ||||||||||++------ Stop bit : No Stop bit(CSI mode setting)     */
/*  |||||||||+-------- Reserved     : 0     Fixed                   */
/*  ||||||||+--------- data transfer sequence : MSB first           */
/*  ||||||++---------- Parity bit : No Parity(CSI mode setting)     */
/*  |||||+------------ Error interrupt  : Disable(CSI mode setting) */
/*  ||||+------------- Reserved     : 0     Fixed                   */
/*  ||++-------------- Data and clock phase : Type 1(SPI mode 3)    */
/*  |+---------------- RXE setting          : Enable                */
/*  +----------------- TXE setting          : Enable                */

#define SIO_USOL_INIT           (uint16_t)0xFFFE
/*  1111111111111110B   */  /* SOLm initial setting(CSI mode setting)  */  /** SET **/
/*  |||||||||||||||+-- SOLm0 Communication data is output :            */  /** SET **/
/*  ||||||||||||||+--- Reserved     : 1     Fixed                      */
/*  |||||||||||||+---- SOLm2 Communication data is output :            */  /** SET **/
/*  +++++++++++++----- Reserved     : 1     Fixed                      */
/*                                                                     */
/* Caution: Refer to the application note for setting method.          */  /** SET **/
/*          Set Unit/Channel No. and reserved bit to use to 1.         */  /** SET **/
/*          Because 0 is written to a register by setting 1.           */  /** SET **/

#define SIO_DUMMY_DATA  (uint8_t)0xFF       /* Dummy data for receiving data            */

/*----------------------------------------------------------------------------------------------*/
/*   Define the wait time for timeout.                                                          */
/*   Time out is occurred after 50000 times loop process of wait time.                          */
/*----------------------------------------------------------------------------------------------*/
#define SIO_T_SAU_WAIT  (uint16_t)(1)       /* 0.25us wait When CPU clock = 32MHz       */

/*==============================================================================================*/
#ifdef MTL_TIM_RL78__32MHz_noWait_IAR
/* Setting for 32.0MHz no wait */
#define MTL_T_1US                (4)  /* loop Number of   1us    */ /** SET **/
#define MTL_T_2US                (9)  /* loop Number of   2us    */ /** SET **/
#define MTL_T_4US               (20)  /* loop Number of   4us    */ /** SET **/
#define MTL_T_5US               (25)  /* loop Number of   5us    */ /** SET **/
#define MTL_T_10US              (51)  /* loop Number of  10us    */ /** SET **/
#define MTL_T_20US             (104)  /* loop Number of  20us    */ /** SET **/
#define MTL_T_30US             (157)  /* loop Number of  30us    */ /** SET **/
#define MTL_T_50US             (263)  /* loop Number of  50us    */ /** SET **/
#define MTL_T_100US            (528)  /* loop Number of 100us    */ /** SET **/
#define MTL_T_200US           (1059)  /* loop Number of 200us    */ /** SET **/
#define MTL_T_300US           (1588)  /* loop Number of 300us    */ /** SET **/
#define MTL_T_400US           (2118)  /* loop Number of 400us    */ /** SET **/
#define MTL_T_1MS             (5298)  /* loop Number of   1ms    */ /** SET **/
#endif

/*******************************************************************************************************************//**
 * Function Name: wait_loop
 * @brief     loop timer processing.
 * @param     time Timer value.
 * @return    none.
 * @details   loop timer processing.
***********************************************************************************************************************/
void wait_loop(uint time);

#endif /* __SIO_CSI_H__ */

