/**
 ******************************************************************************
 * File Name          : drifter.c
 * Author			  : Xavier Halgand
 * Date               :
 * Description        : random segment waveform generator
 ******************************************************************************
 */

#include "drifter.h"

/*-------------------------------------------------------------------------------------------*/

static Drifter_t 	d1, d2;

/*-------------------------------------------------------------------------------------------*/
void drifter_newSegment(Drifter_t *d) //
{
	d->n = 0;
	d->initial = d->final;
	d->minl = 0.5f * Fs / d->fmax ;
	d->maxl = 0.5f * Fs / d->fmin ;
	d->length = frand_a_b(d->minl,d->maxl);
	d->final = frand_a_b(-1, 1);
	d->slope = (d->final - d->initial) / d->length ;
}
/*-------------------------------------------------------------------------------------------*/
float drifter_nextSample(Drifter_t *d) //
{
	d->out = d->gain * (d->slope * d->n + d->initial);
	(d->n)++;
	if (d->n >= d->length)
		{
			drifter_newSegment(d);
		}
	return d->out;
}
/*-------------------------------------------------------------------------------------------*/
float d1_drifter_nextSample(void) //
{
	return drifter_nextSample(&d1);
}
/*-------------------------------------------------------------------------------------------*/
float d2_drifter_nextSample(void) //
{
	return drifter_nextSample(&d2);
}
/*---------------------------------------------------------------------------------------------*/
void drifter_prepare(Drifter_t *d)
{
	d->initial = 0;
	d->final = 0;
	d->fmax = 4;
	d->fmin = 2;
	d->gain = .01f;

	drifter_newSegment(d);
}
/*---------------------------------------------------------------------------------------------*/
void drifter_init(void)
{
	drifter_prepare(&d1);
	drifter_prepare(&d2);
}

/*------------------------------------------END--------------------------------------------*/

