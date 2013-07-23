/**
  ******************************************************************************
  * File Name          : mx_gpio.h
  * Date               : 09/03/2013 01:51:31
  * Description        : This file contains all the functions prototypes for 
  *                      the mx_gpio  
  ******************************************************************************

  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __mx_pinoutConfig_H
#define __mx_pinoutConfig_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"




/* Prototypes ----------------------------------------------------------------*/
void pinout_config(void);
uint8_t PBGetState(uint16_t button);
void BG_Write(uint16_t Leds);
//void BG_LEDsOff(uint16_t Leds);
void BG_LEDsToggle(uint16_t Leds);


#ifdef __cplusplus
}
#endif
#endif /*__ mx_pinoutConfig_H */
