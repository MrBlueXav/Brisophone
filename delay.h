/**
 ******************************************************************************
 * File Name          : delay.h
 * Author				: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H
/*--------------------- Global Defines ------------------------ */
#define DELAYLINE_LEN           20000  // max delay in samples (0.625 seconds)
#define DELAY                   13000  // init delay (in samples)
#define MIN_DELAY				250		// min delay, in samples
#define DELTA_DELAY				200		// variation step of delay time
#define FEEDB                   0.4f

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "CONSTANTS.h"

/* Exported functions --------------------------------------------------------*/

void 	delay_Init(void);
void 	delay_Clean(void);
float	delay_Compute (float x);
void 	inc_delayfeedback(void);
void	dec_delayfeedback(void);
void 	inc_delayTime(void);
void	dec_delayTime(void);

#endif
