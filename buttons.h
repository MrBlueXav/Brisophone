/**
 ******************************************************************************
 * File Name		: buttons.h
 * Author			: Xavier Halgand
 * Date				:
 * Description		:
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTONS_H
#define __BUTTONS_H


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
void action_ButtonPressed1(void);
void action_ButtonReleased1(void);
void action_ButtonPressed2(void);
void action_ButtonReleased2(void);
void action_ButtonPressed3(void);
void action_ButtonReleased3(void);
void action_ButtonPressed4(void);
void action_ButtonReleased4(void);


#endif /* __BUTTONS_H */
