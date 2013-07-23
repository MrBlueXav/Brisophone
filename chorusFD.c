/**
 ******************************************************************************
 * File Name          : chorusFD.c
 * Author			  : Xavier Halgand (thanks to Gabriel Rivas)
 * Date               :
 * Description        :
 ******************************************************************************
 */
#include "chorusFD.h"
#include <math.h>
#include "sinetable.h"
#include  <stdint.h>
#include "CONSTANTS.h"

/* Private define ------------------------------------------------------------*/
#define MARGIN				6
#define L_Del_TapCenter 	700 /*  used for add & subtracting delay from nominal tap center */
#define R_Del_TapCenter 	600 /*  used for add & subtracting delay from nominal tap center */
#define MAX_COUNT			40

/*****************************************************************************
 *       Fractional delay line implementation in C:
 *
 *                    ---------[mix >----------------------------
 *                    |                                         |
 *                    |                                         |
 *                    |x1                                       v
 *     xin ------>[+]----->[z^-M]--[interp.]----[fw >--------->[+]-----> yout
 *                 ^                         |
 *                 |                         |
 *                 --< fb]<-------------------
 *
 *******************************************************************************/

/*-------------------------------------------------------------------------------------------*/
//extern unsigned long _sccm;      /*!< Start address for the .ccm section      */
//extern unsigned long _eccm;      /*!< End address for the .ccm section        */

static Lfo_t			lfoL, lfoR;
static fract_delay 		delR, delL ;

//static fract_delay 	delR __attribute__((section(".ccm"))) ;
//static fract_delay 	delL __attribute__((section(".ccm"))) ;

//uint8_t buffX[10000] __attribute__((section(".ccm"))) = { 0 };

/*-------------------------------------------------------------------------------------------*/
float RandomLfo_SampleCompute(Lfo_t * op) //
{
	float z;

	op->phase += _2PI * Ts * op->freq; // increment phase
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	z = sinetable[lrintf(ALPHA * (op->phase))];
	op->out = op->amp*z;

	return op->out;
}
/*---------------------------------------------------------------------------------------------*/
void chorusFD_init(void)
{
	delL.baseDelay = L_Del_TapCenter;
	delR.baseDelay = R_Del_TapCenter;
	chorusDelay_init(&delL, L_Del_TapCenter, .2f, .5f, .5f);
	chorusDelay_init(&delR, R_Del_TapCenter, .2f, .5f, .5f);
	lfoL.amp = 300;
	lfoR.amp = 300;
	lfoL.freq = .11f ;
	lfoR.freq = .13f ;

}
/*
This function is used to initialize the delay object
 */
void chorusDelay_init(fract_delay *del, float delay_samples, float dfb, float dfw, float dmix)
{
	Delay_set_fb(del, dfb);
	Delay_set_fw(del, dfw);
	Delay_set_mix(del, dmix);
	del->in_idx = DEPTH - 1;// Place the input pointer at the end of the buffer
}

/*
These functions are used as interface to the delay object,
so there's not direct access to the delay object from
external modules
 */
void inc_chorusRate(void)
{
	lfoL.freq *= 1.05f;
	lfoR.freq *= 1.05f;
}
void dec_chorusRate(void)
{
	lfoL.freq *= .95f;
	lfoR.freq *= .95f;
}
void inc_chorusDelay(void)
{
	float d;

	d = delL.baseDelay * 1.1f;
	if (d < DEPTH)
	{
		delL.baseDelay = d;

	}
	d = delR.baseDelay * 1.1f;
	if (d < DEPTH)
	{
		delR.baseDelay = d;

	}
}
void dec_chorusDelay(void)
{
	delL.baseDelay *= .9f;
	delR.baseDelay *= .9f;
}
void inc_chorusFeedback(void)
{
	/* increment feedback delay */

	delL.fb *= 1.02f ;//
	delR.fb *= 1.02f ;//
}

void dec_chorusFeedback(void)
{
	/* decrement feedback delay */

	delL.fb *= 0.95f ;//
	delR.fb *= 0.95f ;//
}
void inc_chorusSweep(void)
{
	lfoL.amp *= 1.05f ;//
	lfoR.amp *= 1.05f ;//
}

void dec_chorusSweep(void)
{
	lfoL.amp *= .95f ;//
	lfoR.amp *= .95f ;//
}

void Delay_set_fb(fract_delay *del, float val)
{
	del->fb = val;
}

void Delay_set_fw(fract_delay *del, float val)
{
	del->fw = val;
}

void Delay_set_mix(fract_delay *del, float val)
{
	del->mix = val;
}

float Delay_get_fb(fract_delay *del )
{
	return del->fb;
}

float Delay_get_fw(fract_delay *del )
{
	return del->fw;
}

float Delay_get_mix(fract_delay *del )
{
	return del->mix;
}


void delay_write (fract_delay *del, float xin)
{
	del->dline[del->in_idx] = xin;
	if (del->in_idx >= DEPTH-1)
		del->in_idx = 0;
	else (del->in_idx)++;
}

float delay_read (fract_delay *del, float delay) // delay is a floating point number of samples
{
	float d;		// true delay
	float f;		// fractional part of delay
	int32_t i;		// integer part of delay
	float y_n;		// y(n)
	float y_n_1;	// y(n-1)
	float y_n_2;	// y(n-2)
	float y_n_3;	// y(n-3)
	int32_t idx;

	d = delay;
	if (d < MARGIN) d = MARGIN;
	if (d > DEPTH-MARGIN) d = DEPTH-MARGIN;

	i = (int32_t)floorf(d);
	f = d - i;

	idx = del->in_idx - i;
	if (idx < 0) idx += DEPTH;
	y_n = del->dline[idx]; 		// y(n)

	idx--;
	if (idx < 0) idx += DEPTH;
	y_n_1 = del->dline[idx];	// y(n-1)

	idx--;
	if (idx < 0) idx += DEPTH;
	y_n_2 = del->dline[idx];	// y(n-2)

	idx--;
	if (idx < 0) idx += DEPTH;
	y_n_3 = del->dline[idx];	// y(n-3)

	//return (y_n_1 - y_n) * f + y_n ; // linear interpolation

	//return (.5f)*(f-1)*(f-2)*y_n - f*(f-2)*y_n_1 + (.5f)*f*(f-1)*y_n_2 ; // 2nd order Lagrange interpolation

	//return .5f*(f-1)*((f-2)*y_n + f*y_n_2) - f*(f-2)*y_n_1 ;	// 2nd order Lagrange interpolation (faster)

	/* 3rd order Lagrange interpolation :  */
	return (f-2)*(f-3)*(-0.16666666666f *(f-1)*y_n + 0.5f * f * y_n_1) + f*(f-1)*(-0.5f * (f-3)*y_n_2 + 0.166666666666f * (f-2)*y_n_3);

}

/*---------------------------------------------------------------------------------------------*/
/*
This is the main mono chorus task,
 */

float mono_chorus_compute(fract_delay *del, Lfo_t *lfo, float xin)
{
	float yout;
	float x1;
	float x2;

	x2 = delay_read (del, del->baseDelay + RandomLfo_SampleCompute(lfo) + lfo->amp + MARGIN);
	x1 = xin + del->fb * x2;
	yout = del->mix * x1 + del->fw * x2;
	delay_write(del, x1);

	return yout;
}

/*---------------------------------------------------------------------------------------------*/

void stereoChorus_compute (float * left_out, float * right_out, float in)
{
	*left_out = mono_chorus_compute(&delL, &lfoL, in);
	*right_out = mono_chorus_compute(&delR, &lfoR, in);
}


/*------------------------------------------END--------------------------------------------*/

