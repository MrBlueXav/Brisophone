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


/*============================================================================*/
/* ========================  MAIN  ===========================================*/
/*============================================================================*/

int
main(void)
{

	/* Expansion board Configuration------------------------------------------*/
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

	audio_init();

	bargraphWrite(1);

	/* Infinite loop */
	while (1)
	{
		wait(2000);  // SysTick_Handler() in file stm32f4xx_it.c is doing his job
					// + DMA access for audio DAC...
	}
}
/* END OF MAIN ======================================================================*/
/*====================================================================================*/
