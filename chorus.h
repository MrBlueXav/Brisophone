/**
 ******************************************************************************
 * File Name          : chorus.h
 * Author				: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CHORUS_H
#define __CHORUS_H



/* Exported functions --------------------------------------------------------*/

void 	chorus_init(void);
float 	chorus_leftCompute(float x);
float 	chorus_rightCompute(float x);

/**************************************************************************************/
#endif
