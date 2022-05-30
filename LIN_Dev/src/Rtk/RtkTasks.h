/**
 * @file   RtkTasks.h
 * @author Michel Catudal
 * @date   6 March 2022
 * @version 0.1
 * @brief  Real Time Tasks
 *
*/

#ifndef RTKTASKS_H
#define RTKTASKS_H

#include "r_cg_macrodriver.h"
#include "r_cg_tau.h"
#include "r_cg_userdefine.h"
#include <string.h>
#include "r_cg_port.h"

#define DucRtk_eTASKNBTASK  9
#define DucRtk_eTASKNBRECUR 8

#define DuiRtk_itask08 0x0100
#define DuiRtk_itask07 0x0080
#define DuiRtk_itask06 0x0040
#define DuiRtk_itask05 0x0020
#define DuiRtk_itask04 0x0010
#define DuiRtk_itask03 0x0008
#define DuiRtk_itask02 0x0004
#define DuiRtk_itask01 0x0002
#define DuiRtk_itask00 0x0001
#define DRtk_inotask   0x0000

#define DuiRtk_itask_1280 DuiRtk_itask00
#define DuiRtk_itask_640  DuiRtk_itask01
#define DuiRtk_itask_320  DuiRtk_itask02
#define DuiRtk_itask_160  DuiRtk_itask03
#define DuiRtk_itask_80   DuiRtk_itask04
#define DuiRtk_itask_40   DuiRtk_itask05
#define DuiRtk_itask_20B  DuiRtk_itask06
#define DuiRtk_itask_20   DuiRtk_itask07
#define DuiRtk_itask_10   DuiRtk_itask08

#define DuiRtk_iREC10   DuiRtk_itask_10
#define DuiRtk_iREC20   DuiRtk_itask_20
#define DuiRtk_iREC20B  DuiRtk_itask_20B
#define DuiRtk_iREC40   DuiRtk_itask_40
#define DuiRtk_iREC80   DuiRtk_itask_80
#define DuiRtk_iREC160  DuiRtk_itask_160
#define DuiRtk_iREC320  DuiRtk_itask_320
#define DuiRtk_iREC640  DuiRtk_itask_640
#define DuiRtk_iREC1280 DuiRtk_itask_1280


#define DuiRtk_iARM0 DuiRtk_iREC10 | DuiRtk_iREC20B
#define DuiRtk_iARM1 DuiRtk_iREC10 | DuiRtk_iREC20
#define DuiRtk_iARM2 DuiRtk_iREC10 | DuiRtk_iREC20B | DuiRtk_iREC40
#define DuiRtk_iARM3 DuiRtk_iREC10 | DuiRtk_iREC20B | DuiRtk_iREC80
#define DuiRtk_iARM4 DuiRtk_iREC10 | DuiRtk_iREC20B | DuiRtk_iREC160
#define DuiRtk_iARM5 DuiRtk_iREC10 | DuiRtk_iREC20B | DuiRtk_iREC320
#define DuiRtk_iARM6 DuiRtk_iREC10 | DuiRtk_iREC20B | DuiRtk_iREC640
#define DuiRtk_iARM7 DuiRtk_iREC10 | DuiRtk_iREC20B | DuiRtk_iREC1280

/*---------------------------------------------------------------------------*/
/* TYPEDEF                                                                   */

typedef union T_SEMAPH
{
    uint16_t uiSemaph;
    uint8_t ucSemaph[2];
} TnRtk_eSemaph;


/*---------------------------------------------------------------------------*/
/* EXPORT                                                                    */
extern const uint16_t CuiRtk_eTabActiv [DucRtk_eTASKNBRECUR];

/*---------------------------------------------------------------------------*/
/* PROTO                                                                     */

extern void vRtk_eTask10_Tsk(void);
extern void vRtk_eTask20_Tsk(void);
extern void vRtk_eTask20B_Tsk(void);
extern void vRtk_eTask40_Tsk(void);
extern void vRtk_eTask80_Tsk(void);
extern void vRtk_eTask160_Tsk(void);
extern void vRtk_eTask320_Tsk(void);
extern void vRtk_eTask640_Tsk(void);
extern void vRtk_eTask1280_Tsk(void);

#endif


