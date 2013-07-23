/**
 ******************************************************************************
 * File Name			: main.c
 * Author				: Xavier Halgand
 * Date					: 24/05/2013
 * Description			: Brisophone, main program body
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/*----------------------------------------------------------------------------*/

uint16_t audiobuff[BUFF_LEN]; // The circular audio buffer

static uint32_t TimingDelay;
uint16_t 		bg_data = 1;
uint8_t			param = 0;
uint8_t			volume = VOL;
bool			sound = true;
const uint16_t	display[] = {1, 3, 7, 15, 31, 63, 127, 255, 511, 1023,
							1022, 1020, 1016, 1008, 992, 960, 896, 768, 512 /* index 18 */,
							513, 771 };


//---------------------------------------------------------------------------
void toggleSound (void)
{
	if ( ! sound )
	{
		pitchGenResetPhase();
		EVAL_AUDIO_VolumeCtl(volume);
		sound = true;
	}
	else
	{
		EVAL_AUDIO_VolumeCtl(0);
		sound = false;
	}
}
//---------------------------------------------------------------------------
void incVol(void)
{
	if (volume < MAXVOL)
	{
		volume++;
		EVAL_AUDIO_VolumeCtl(volume);
	}
}
//---------------------------------------------------------------------------
void decVol(void)
{
	if (volume > 0)
	{
		volume--;
		EVAL_AUDIO_VolumeCtl(volume);
	}
}
//---------------------------------------------------------------------------
void action_ButtonPressed1(void)
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
void action_ButtonPressed2(void)
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
//---------------------------------------------------------------------------
void setSysTick(void)
{
	if (SysTick_Config(SystemCoreClock / SYSTICK_FREQ))
	{
		/* Capture error */
		while (1)
			;
	}
	NVIC_SetPriority(SysTick_IRQn,0);
}
//---------------------------------------------------------------------------
/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void Delay(uint32_t nTime)
{
	TimingDelay = nTime;

	while (TimingDelay != 0)
		;
}
//---------------------------------------------------------------------------
/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0)
	{
		TimingDelay--;
	}
}

//---------------------------------------------------------------------------
/**
 * @brief  Basic management of the timeout situation.
 * @param  None
 * @retval None
 */
uint32_t
Codec_TIMEOUT_UserCallback(void)
{
	//STM_EVAL_LEDOn(LED5); /*  alert : red LED !  */
	return (0);
}
//---------------------------------------------------------------------------
/**
 * @brief  Manages the DMA Half Transfer complete interrupt.
 * @param  None
 * @retval None
 */
void
EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size)
{
	/* Generally this interrupt routine is used to load the buffer when
   a streaming scheme is used: When first Half buffer is already transferred load
   the new data to the first half of buffer while DMA is transferring data from
   the second half. And when Transfer complete occurs, load the second half of
   the buffer while the DMA is transferring from the first half ... */

	STM_EVAL_LEDOff(LED_Orange);
	make_sound(0, BUFF_LEN_DIV4);
	STM_EVAL_LEDOn(LED_Orange);


}
//---------------------------------------------------------------------------
/**
 * @brief  Manages the DMA Complete Transfer complete interrupt.
 * @param  None
 * @retval None
 */
void
EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{

	STM_EVAL_LEDOff(LED_Orange);
	make_sound(BUFF_LEN_DIV2, BUFF_LEN_DIV4);
	STM_EVAL_LEDOn(LED_Orange);

}

//---------------------------------------------------------------------------
/** NOT USED !
 * @brief  Get next data sample callback
 * @param  None
 * @retval Next data sample to be sent
 */
uint16_t
EVAL_AUDIO_GetSampleCallBack(void)
{
	return 0;
}

/*============================================================================*/
/* ========================  MAIN  ===========================================*/

int
main(void)
{

	/* EXpansion board Configuration------------------------------------------*/
	pinout_config();
	bargraphInit();

	/* Initialize Discovery board LEDS */
	STM_EVAL_LEDInit(LED3); // orange LED
	STM_EVAL_LEDInit(LED4); // green LED
	STM_EVAL_LEDInit(LED5); // red LED
	STM_EVAL_LEDInit(LED6); // blue LED

	/* Setup SysTick Timer for x msec interrupts.----------------------------*/
	setSysTick();


	randomGen_init();/* Initialize the on-board random number generator ! */
	delay_Init();
	Synth_Init();
	drifter_init();
	pitchGenInit();
	chorusFD_init();

	//soundGenInit();

	EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, VOL, SAMPLERATE);
	EVAL_AUDIO_Play((uint16_t*) audiobuff, BUFF_LEN); // start sound

	bargraphWrite(1);

	/* Infinite loop */
	while (1)
	{

		Delay(2000);

	}
}
/* END OF MAIN ======================================================================*/
/*====================================================================================*/
