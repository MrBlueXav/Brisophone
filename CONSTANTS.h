/**
 ******************************************************************************
 * File Name          : CONSTANTS.h
 * Author			  : Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define SAMPLERATE              48000
#define Fs             			((float_t)(SAMPLERATE))	// samplerate
#define Ts						(1.f/Fs)  // sample period
#define _2PI                    6.283185307f
#define _PI                    	3.14159265f

#define _CCM_	__attribute__((section(".ccm")))

/************************************************************************************/
#endif
