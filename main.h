/**
 ******************************************************************************
 * @file    main.h
 * Author: 	Xavier Halgand
 * @author
 * @version
 * @date
 * @brief   Header for main.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Includes ------------------------------------------------------------------*/
#include "mx_gpio.h"
#include "soundGen.h"
#include "stm32f4_discovery.h"
#include "bargraph.h"
#include "debounce.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio_codec.h"
#include "stm32f4xx_it.h"
#include "delay.h"
#include "chorusFD.h"
#include "random.h"
#include "CONSTANTS.h"
#include "drifter.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


#define BUFF_LEN_DIV4           160 // number of samples <==> XX ms latency at 48kHz
#define BUFF_LEN_DIV2           (2*BUFF_LEN_DIV4)
#define BUFF_LEN                (4*BUFF_LEN_DIV4)  // Audio buffer length : count in 16bits half-words
#define VOL                     70
#define MAXVOL                  100

#define SYSTICK_FREQ			500 // system tick interruption frequency in Hz
#define PARAM_MAX				20 // maximal parameter index, starting at 0

/* Exported functions ------------------------------------------------------- */
void action_ButtonPressed1(void);
void action_ButtonReleased1(void);
void action_ButtonPressed2(void);
void action_ButtonReleased2(void);
void action_ButtonPressed3(void);
void action_ButtonReleased3(void);
void action_ButtonPressed4(void);
void action_ButtonReleased4(void);

void TimingDelay_Decrement(void);

/*-------------------------------------------------------*/
extern uint16_t audiobuff[];

/*-------------------------------------------------------*/
#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
