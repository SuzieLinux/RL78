/**
 * @file   Scheduler.c
 * @author Michel Catudal
 * @date   2 July 2022
 * @brief  Real Time Scheduler
 *
*/

#include "system.h"
#include "r_cg_it.h"
#include "Tasks.h"
#include "Scheduler.h"

static uint8_t Count10msecs;
static uint8_t Count50msecs;
static uint8_t Count100msecs;
static uint16_t Count1sec;

/*****************************************************************************
 *      @brief Initialize variables
 *
 *      @param[in] None
 *
 *      @return None
 */
void InitScheduler(void)
{
	Count10msecs = 0;
	Count50msecs = 0;
	Count100msecs = 0;
	Count1sec = 0;

	R_IT_Start();
}

/*****************************************************************************
 *      @brief Set up which task is going to be ran
 *
 *      @param[in] None
 *
 *      @return None
 */
void Scheduler(void)
{
	if (Count10msecs)
	{
		Count10msecs = 0;
		Task10msecs();
	}
	if (Count50msecs)
	{
		Count50msecs = 0;
		Task50msecs();
	}
	if (Count100msecs)
	{
		Count100msecs = 0;
		Task100msecs();
	}
	if (Count1sec)
	{
		Count1sec = 0;
		Task1sec();
	}
}

/*****************************************************************************
 *      @brief Timer function
 *
 *      @param[in] None
 *
 *      @return None
 */
#pragma vector = INTIT_vect
__interrupt static void vRtk_eRealTimeFunction(void)
{
	Count10msecs++;
	if (!Count10msecs) Count10msecs--;
	Count50msecs++;
	if (!Count50msecs) Count50msecs--;
	Count100msecs++;
	if (!Count100msecs) Count100msecs--;
	Count1sec++;
	if (!Count1sec) Count1sec--;
}



