/******************************************************************************
 * File Name          : LPfilter.c
 * Author				: (c) 2001-2005 Nick Dowell (amSynth) + Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
/* from amSynth
 *
 */

#include "LPfilter.h"

static float fcut;
static float reso;
static float a0, a1, a2, b1, b2;
static float d1, d2, d3, d4;

void
LPfilter_reset()
{
	d1 = d2 = d3 = d4 = 0;
	LPfilter_computeCoeff(8000, 0.5f);
}



void LPfilter_computeCoeff( float fc, float res)
{
	float w, r, k, k2, bh;

	// find final coeff values
	// constrain cutoff
	if (fc>10000)	fcut=10000;
	else if (fc<10)	fcut = 10;
	else fcut = fc;
	reso = res;

	w = fcut / SAMPLERATE; // cutoff freq [ 0 <= w <= 0.5 ]
	r = 2*(1-res);
	if(r == 0.0) r = 0.001f;
	k = tanf(w * _PI);
	k2 = k * k;
	bh = 1 + r * k + k2;
	a0 = a2 = k2 / bh;
	a1 = a0 * 2;
	b1 = -2*(k2-1) / bh;
	b2 = -(1-(r*k) + k2) / bh;
}

float	LPfilter_processSample (float input)
{
	float x, y;

	x = 0.7f * input;
	// first 2nd-order unit
	y =  a0*x  + d1;
	d1 = d2 +  a1*x  +  b1*y ;
	d2 =  a2*x  +  b2*y ;
	x = y;
	// and the second
	y =  a0*x  + d3;
	d3 = d4 +  a1*x  +  b1*y ;
	d4 =  a2*x  +  b2*y ;

	/**** prevent overflow ****/
	y = (y > 1.0f) ? 1.0f : y; //clip too loud samples
	y = (y < -1.0f) ? -1.0f : y;

	return y;
}

