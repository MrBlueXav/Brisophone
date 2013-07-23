/**
 ******************************************************************************
 * File Name          : bargraph.h
 * Author				: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BARGRAPH_H
#define __BARGRAPH_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_gpio.h"
#include "mx_gpio.h"
#include <stdint.h>

/* Defines for the LED bargraph ---------------------------------------------------*/

#define BARGRAPH0	GPIO_Pin_6
#define BARGRAPH1	GPIO_Pin_7
#define BARGRAPH2	GPIO_Pin_8
#define BARGRAPH3	GPIO_Pin_9
#define BARGRAPH4	GPIO_Pin_10
#define BARGRAPH5	GPIO_Pin_11
#define BARGRAPH6	GPIO_Pin_12
#define BARGRAPH7	GPIO_Pin_13
#define BARGRAPH8	GPIO_Pin_14
#define BARGRAPH9	GPIO_Pin_15


#define ALL_LEDS	BARGRAPH0|BARGRAPH1|BARGRAPH2|BARGRAPH3|BARGRAPH4|BARGRAPH5|BARGRAPH6|BARGRAPH7|BARGRAPH8|BARGRAPH9


struct bargraph
{
	uint16_t 		state;
	uint16_t 		mask;
	uint8_t 		current;
	const uint16_t	ledPin[10];
};


/* Exported functions ------------------------------------------------------- */
void bargraphInit(void);
void bargraphUpdate(void);
void bargraphWrite(uint16_t data);




#endif /* __BARGRAPH_H */
