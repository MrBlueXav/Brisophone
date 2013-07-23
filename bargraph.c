/**
 ******************************************************************************
 * File Name          : bargraph.c
 * Date               :
 * Author				: Xavier Halgand
 * Description        :
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

#include "bargraph.h"

static struct bargraph bg =
	{0, 1, 0, {BARGRAPH0, BARGRAPH1, BARGRAPH2, BARGRAPH3, BARGRAPH4, BARGRAPH5, BARGRAPH6, BARGRAPH7, BARGRAPH8, BARGRAPH9}};


void bargraphInit(void)
{
	bg.state = 0;
	bg.mask = 1;
	bg.current = 0;
}

void bargraphWrite(uint16_t data)
{
	bg.state = data;
}

/*  bargraphUpdate() must be called by the SysTick_Handler() function in file <stm32f4xx_it.c>   :   */
void bargraphUpdate(void)
{
	//BG_LEDsOff(ALL_LEDS);

	if ( bg.state & bg.mask ) BG_Write(bg.ledPin[bg.current]);
	else BG_Write(0);

	if (bg.current >= 9) bg.current = 0; else bg.current++;
	bg.mask = 1 << bg.current;

}
