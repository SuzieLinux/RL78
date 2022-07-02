/**
 * @file   spi.h
 * @author Michel Catudal
 * @date   6 June 2022
 * @version 0.1
 * @brief  SPI Support for CANFD driver
 *
*/
#ifndef _SPI_H
#define _SPI_H

#include "system.h"
#include "r_cg_port.h"

#define SPI_OK 0
#define SPI_ERR_HARD -1

#define SPI_DEFAULT_BUFFER_LENGTH 96

//! SPI Transmit buffer
extern uint8_t spiTransmitBuffer[SPI_DEFAULT_BUFFER_LENGTH];

//! SPI Receive buffer
extern uint8_t spiReceiveBuffer[SPI_DEFAULT_BUFFER_LENGTH];

void SPI_wait(uint16_t time);
int16_t SPI_Send_Receive(uint16_t ByteCount);
int16_t SPI_ReadByte(uint16_t address, uint8_t *rxd);
int16_t SPI_WriteByte(uint16_t address, uint8_t txd);
int16_t SPI_ReadWord(uint16_t address, uint32_t *rxd);
int16_t SPI_WriteWord(uint16_t address, uint32_t txd);
int16_t SPI_ReadHalfWord(uint16_t address, uint16_t *rxd);
int16_t SPI_WriteHalfWord(uint16_t address, uint16_t txd);
int16_t SPI_ReadByteArray(uint16_t address, uint8_t *rxd, uint16_t nBytes);
int16_t SPI_WriteByteArray(uint16_t address, uint8_t *txd, uint16_t nBytes);
int16_t SPI_ReadWordArray(uint16_t address, uint32_t *rxd, uint16_t nWords);
int16_t SPI_WriteWordArray(uint16_t address, uint32_t *txd, uint16_t nWords);
void R_SAU0_Create(void);
void R_CSI11_Start(void);
void R_CSI11_Stop(void);

extern uint8_t g_csi11_data_received;

/* Setting for 32.0MHz no wait */
#define Time_1US        (4)  /* loop Number of   1us    */ /** SET **/
#define Time_2US        (9)  /* loop Number of   2us    */ /** SET **/
#define Time_4US       (20)  /* loop Number of   4us    */ /** SET **/
#define Time_5US       (25)  /* loop Number of   5us    */ /** SET **/
#define Time_10US      (51)  /* loop Number of  10us    */ /** SET **/
#define Time_20US     (104)  /* loop Number of  20us    */ /** SET **/
#define Time_30US     (157)  /* loop Number of  30us    */ /** SET **/
#define Time_50US     (263)  /* loop Number of  50us    */ /** SET **/
#define Time_100US    (528)  /* loop Number of 100us    */ /** SET **/
#define Time_200US   (1059)  /* loop Number of 200us    */ /** SET **/
#define Time_300US   (1588)  /* loop Number of 300us    */ /** SET **/
#define Time_400US   (2118)  /* loop Number of 400us    */ /** SET **/
#define Time_1MS     (5298)  /* loop Number of   1ms    */ /** SET **/

/*----------------------------------------------------------------------------------------------*/
/*   Define the wait time for timeout.                                                          */
/*----------------------------------------------------------------------------------------------*/
#define SPI_T_SAU_WAIT  (uint16_t)(1)       /* 0.25us wait When CPU clock = 32MHz       */

/*------- Definitions of software timer value --------*/
/* Transmit&receive waiting time */
#define SPI_TX_WAIT  ((uint16_t)(5000))     /* SIO transmission completion waiting time 5000* 1us =  5ms  */
#define SPI_RX_WAIT  ((uint16_t)(5000))     /* SIO reception completion waiting time    5000* 1us =  5ms  */

/* Access time */
#define SPI_T_SIO_WAIT  ((uint16_t)Time_1US)   /* SIO transmission&reception completion waiting polling time  */

#define SIO_RXNEXT IF0H_bit.no5    /* CSI Receive completion  */
#define SIO_TXEND  IF0H_bit.no5    /* CSI Transmit completion */

/*--------------- Definitions of flag ----------------*/
#define SIO_TRUE  ((uint8_t)(0x01))       /* Flag "ON"   */
#define SIO_FALSE ((uint8_t)(0x00))       /* Flag "OFF"  */

#endif

