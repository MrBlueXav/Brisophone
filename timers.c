/**
 ******************************************************************************
 * File Name		: timers.c
 * Date				:
 * Author			: Xavier Halgand
 * Description		:
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "timers.h"

static uint32_t TimingDelay;

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
void wait(uint32_t nTime)
{
	TimingDelay = nTime;

	while (TimingDelay != 0)
		;
}
//---------------------------------------------------------------------------
/**
 * @brief  Decrements the TimingDelay variable.
 * 			Called by SysTick_Handler() in stm32f4xx_it.c
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
