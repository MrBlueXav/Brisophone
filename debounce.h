 /******************************************************************************
 * File Name          : debounce.h
 * Author				: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */





/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBOUNCE_H
#define __DEBOUNCE_H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_gpio.h"

//#include "main.h"

#define CHECK_MSEC 2 // Read hardware every x msec
#define PRESS_MSEC 20 // Stable time before registering pressed
#define RELEASE_MSEC 20 // Stable time before registering released


/* Exported functions ------------------------------------------------------- */

void DebounceSwitch1(void);
void DebounceSwitch2(void);
void DebounceSwitch3(void);
void DebounceSwitch4(void);

#endif /* __BARGRAPH_H */
