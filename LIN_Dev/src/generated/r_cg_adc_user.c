/**
 * @file   r_cg_adc_user.c
 * @author Michel Catudal
 * @date   29 May 2022
 * @version 0.1
 * @brief  ADC Support
 *
*/
/**********************************************************************************
* Code based on code generated by Applilet4 for RL78/G11 and IAR Systems iccrl78
* Copyright (C) 2016, 2021 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************/
/**********************************************************************************
Includes
**********************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
#include "r_cg_userdefine.h"

// Measure results for each analog input
TstADCData stADCData[2];
uint8_t SampleNumber[2] = { 0,0 };
uint8_t 1AcquiMade = 0;
uint8_t ADC_Done = 0;
uint8_t AcquiMade = 0;

/*****************************************************************************
 *      @brief   Treating analog acquisition
 *
 *      @param[in] None
 *
 *      @return 0 None
******************************************************************************/
void TreatAcquisition(void)
{
    uint8_t  uci;
    uint32_t ulcalcul;

    if (ADC_Done == 1)
    {
        if (!AcquiMade)   /* Likely coming out of reset */
        {
            AcquiMade = 1;
        }

        for (uci = 0; uci < 2; uci++)
        {
            ulcalcul = stADCData[uci].uiValueRead;   // Read previous measure (t)
            stADCData[uci].uiValueFiltered = ulcalcul;
            stADCData[uci].uiFilteringRest = 0;
        }
        ADC_Done = 0;
		ADS = 0;
		ADIF = 0U;  /* clear INTAD interrupt flag */
		ADMK = 0U;  /* enable INTAD interrupt */
		ADCS = 1U;  /* enable AD conversion */
    }
}

/*****************************************************************************
 *      @brief   End of conversion
 *
 *      @param[in] None
 *
 *      @return 0 None
******************************************************************************/
#pragma vector = INTAD_vect
__interrupt static void r_adc_interrupt(void)
{
    uint8_t CurrentSample;
    uint16_t ADC_Data = (uint16_t)(ADCR >> 6U);
    uint8_t CurrentADC = ADS;
    uint32_t ulValConversion;

    if (CurrentADC > 1)
    {
        ADS = 0;
        ADIF = 0U;  /* clear INTAD interrupt flag */
        ADMK = 0U;  /* enable INTAD interrupt */
        ADCS = 1U;  /* enable AD conversion */
        return;
    }

    CurrentSample = SampleNumber[CurrentADC];

    if (CurrentSample > 3)
    {
        CurrentSample = 0;  /* Garbage protection */
        SampleNumber[CurrentADC] = 0;
    }

    stADCData[CurrentADC].uiSample[CurrentSample++] = ADC_Data;
    if (CurrentSample == 4)
    {
         ulValConversion = stADCData[CurrentADC].uiSample[0] +
                           stADCData[CurrentADC].uiSample[1] +
                           stADCData[CurrentADC].uiSample[2] +
                           stADCData[CurrentADC].uiSample[3];
         ulValConversion >>= 2; /* Divide by 4 */
         stADCData[CurrentADC].uiValueRead = (uint16_t) ulValConversion;

         if (CurrentADC == 1)
         {
             ADC_Done = 1;
             SampleNumber[0] = 0;
             SampleNumber[1] = 0;

             return;    /* Second channel is done */
         }
         else ADS = 1U;/* Convert next channel*/
    }

    SampleNumber[CurrentADC] = CurrentSample;

    ADIF = 0U;  /* clear INTAD interrupt flag */
    ADMK = 0U;  /* enable INTAD interrupt */
    ADCS = 1U;  /* enable AD conversion */
}



