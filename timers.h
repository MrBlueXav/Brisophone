/**
 ******************************************************************************
 * File Name		: timers.h
 * Author			: Xavier Halgand
 * Date				:
 * Description		:
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMERS_H
#define __TIMERS_H


/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx_gpio.h"
//#include "mx_gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include "CONSTANTS.h"
#include "audio.h"
#include "stm32f4_discovery_audio_codec.h"
#include "stm32f4_discovery.h"
#include "soundGen.h"
#include "bargraph.h"

/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void wait(uint32_t nTime);
void setSysTick(void);

#endif /* __TIMERS_H */
