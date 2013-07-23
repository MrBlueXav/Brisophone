/**
 ******************************************************************************
 * File Name		: buttons.c
 * Date				:
 * Author			: Xavier Halgand
 * Description		:
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "buttons.h"

static uint8_t			param = 0;
static const uint16_t	display[] = {1, 3, 7, 15, 31, 63, 127, 255, 511, 1023,
							1022, 1020, 1016, 1008, 992, 960, 896, 768, 512 /* index 18 */,
							513, 771 };

//---------------------------------------------------------------------------
void action_ButtonPressed1(void) // change parameter to edit (forward)
{
	STM_EVAL_LEDOn(LED_Blue);
	if (param < PARAM_MAX)
	{
		param++;

	} else param = 0;
	bargraphWrite(display[param]);
}

//---------------------------------------------------------------------------
void action_ButtonReleased1(void)
{
	STM_EVAL_LEDOff(LED_Blue);
}

//*****************************************************************************
//---------------------------------------------------------------------------
void action_ButtonPressed2(void) // change parameter to edit (backward)
{
	STM_EVAL_LEDOn(LED_Blue);
	if (param > 0)
	{
		param--;

	} else param = PARAM_MAX;
	bargraphWrite(display[param]);
}

//---------------------------------------------------------------------------
void action_ButtonReleased2(void)
{
	STM_EVAL_LEDOff(LED_Blue);
}


//****************************************************************************
//---------------------------------------------------------------------------
void action_ButtonPressed3(void)
{
	STM_EVAL_LEDOn(LED_Blue);

	switch (param)
	{
	case 0 : 	pitchGenRequestChangePoints(); 	break ;
	case 1 :  	toggleChorus();	break;
	case 2 : 	incMaxFreq(); 	break;
	case 3 : 	incTempo();		break;
	case 4 : 	doubleTempo();	break;
	case 5 : 	toggleGlide();	break;
	case 6 : 	nextSound();	break;
	case 7 : 	incDecay();		break;
	case 8 : 	muteSomeNotes();break; // toggle function !
	case 9 :	nextScale();	break;
	case 10 : 	incVol();		break;
	case 11 : 	incSynthOut();	break;
	case 12 :	transpUp();		break;
	case 13 :	AdditiveGen_newWaveform();	break;
	case 14 :	inc_delayfeedback(); 		break;
	case 15 :	inc_delayTime(); 			break;
	case 16 :	inc_chorusFeedback();		break;
	case 17 :	inc_chorusRate();			break;
	case 18 :	inc_chorusDelay();			break;
	case 19 :	inc_chorusSweep();			break;
	case 20 : 	toggleVibrato();			break;
	}
}

//---------------------------------------------------------------------------
void action_ButtonReleased3(void)
{
	STM_EVAL_LEDOff(LED_Blue);
}
//****************************************************************************
//---------------------------------------------------------------------------
void action_ButtonPressed4(void)
{
	STM_EVAL_LEDOn(LED_Blue);

	switch (param)
	{
	case 0 : 	automatic_or_manual();	break ;
	case 1 : 	toggleFilter();	break;
	case 2 : 	decMaxFreq(); 	break;
	case 3 : 	decTempo();		break;
	case 4 : 	halfTempo();	break;
	case 5 : 	pitchGenRequestChangePoints();	break;
	case 6 : 	prevSound();	break;
	case 7 : 	decDecay();		break;
	case 8 : 	toggleGlide();	break;
	case 9 :	prevScale();	break;
	case 10 : 	decVol();		break;
	case 11 : 	decSynthOut();	break;
	case 12 :	transpDown();	break;
	case 13 :	toggleDelay();	break;
	case 14 :	dec_delayfeedback(); 	break;
	case 15 :	dec_delayTime(); 		break;
	case 16 :	dec_chorusFeedback();	break;
	case 17 :	dec_chorusRate();		break;
	case 18 :	dec_chorusDelay();		break;
	case 19 :	dec_chorusSweep();		break;
	case 20 : 	toggleSynthOut();		break;
	}
}

//---------------------------------------------------------------------------
void action_ButtonReleased4(void)
{
	STM_EVAL_LEDOff(LED_Blue);
}


//******************************************************************************
