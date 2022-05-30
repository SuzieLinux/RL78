/**
 * @file   RtkSched.h
 * @author Michel Catudal
 * @date   6 March 2022
 * @version 0.1
 * @brief  Real Time Scheduler
 *
*/

#ifndef RtkSched_H
#define RtkSched_H

/* DEFINITION                                                                */
#define DuiRtk_e1HOUR_DEF       2812    // 1 Hour = 3600 s / 1.28 s
#define DucRtk_e1MINUTE_DEF     188     // 1 Minute = 60 s / 0.320 s
#define DucRtk_e1SEC_DEF        13      // 1 sec = 1000/80 = 12.5
#define DucRtk_e2SEC_DEF        25      // 2 sec = 2000/80 = 25
#define DucRtk_e500MSEC_DEF     6       //500 msec = 500/80 = 6.25

/*---------------------------------------------------------------------------*/
/* PROTO                                                                     */
extern void vRtk_eInitGlobVar_Set(void);
extern void vRtk_eScheduler_Tsk (void);

#endif

