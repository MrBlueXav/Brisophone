/**
 ******************************************************************************
 * File Name          : chorusFD.h
 * Author             : Xavier Halgand (thanks to Gabriel Rivas)
 * Date               :
 * Description        :
 ******************************************************************************
 */
#ifndef __CHORUSFD_H__
#define __CHORUSFD_H__

#include  <stdint.h>

/*--------------------------------------------------------------------------------------*/

#define DEPTH	 		1400 // Size of delay buffer, in samples : 29.17 ms
#define LEFT_DELAY 		240 /*  initial left delay */
#define RIGHT_DELAY 	240 /*  initial right delay */
#define LEFT_SWEEP		50
#define RIGHT_SWEEP		50
#define LEFT_RATE		0.11f // in Hz
#define RIGHT_RATE		0.12f // in Hz
#define FEEDBACK		0.2f // look at the diagram
#define FORWARD			0.5f
#define MIX				0.5f


/*---------------------------------------------------------------------------------------*/
typedef struct
{
	float 	amp;
	float 	freq;
	float 	phase;
	float 	out;

} Lfo_t;

typedef struct
{
	float        mix;       	/* delay blend parameter */
	float        fb;       		/* feedback volume */
	float        fw;       		/* delay tap mix volume */
	int32_t      in_idx;    	/* delay write index */
	float        dline[DEPTH] ;	/* delay buffer */
	float        baseDelay;		/* tap position */

} fract_delay ;

/*---------------------------------------------------------------------------------------------*/

void chorusFD_init(void);
void chorusDelay_init(fract_delay *del, float delay_samples,float dfb,float dfw, float dmix);
void stereoChorus_compute (float * left_out, float * right_out, float in);
void inc_chorusFeedback(void);
void dec_chorusFeedback(void);
void inc_chorusRate(void);
void dec_chorusRate(void);
void inc_chorusDelay(void);
void dec_chorusDelay(void);
void inc_chorusSweep(void);
void dec_chorusSweep(void);
void Delay_set_fb(fract_delay *del, float val);
void Delay_set_fw(fract_delay *del, float val);
void Delay_set_mix(fract_delay *del, float val);
void Delay_set_delay(fract_delay *del, float n_delay);
float Delay_get_fb(fract_delay *del);
float Delay_get_fw(fract_delay *del);
float Delay_get_mix(fract_delay *del);
float mono_chorus_compute(fract_delay *del, Lfo_t *lfo, float xin);

/*****************************************************************************************************/
#endif
