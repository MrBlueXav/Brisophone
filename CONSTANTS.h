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

#define BUFF_LEN_DIV4           160 // number of samples <==> XX ms latency at 48kHz
#define BUFF_LEN_DIV2           (2*BUFF_LEN_DIV4)
#define BUFF_LEN                (4*BUFF_LEN_DIV4)  // Audio buffer length : count in 16bits half-words
#define VOL                     70 // initial output DAC volume
#define MAXVOL                  100 // maximal output DAC volume

#define SYSTICK_FREQ			500 // system tick interruption frequency in Hz
#define PARAM_MAX				20 // maximal parameter index, starting at 0

#define _CCM_					__attribute__((section(".ccm"))) // not used yet

/************************************************************************************/
#endif  /*__CONSTANTS_H__ */
