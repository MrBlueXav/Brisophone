/**
 ******************************************************************************
 * File Name          : chorus.c
 * Author			  : Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */

#include "chorus.h"
#include "sinetable.h"
#include  <stdint.h>
#include <math.h>

/* Private define ------------------------------------------------------------*/

#define SAMPLERATE              48000
#define Fs             			((float_t)(SAMPLERATE))	// samplerate
#define Ts						(1.f/Fs)  // sample period
#define _2PI                    6.283185307f
#define _PI                    	3.14159265f

/* Chorus Control Parameters */
#define a0L 	0.7f /* a0 = 0.99999999, left input gain */
#define a1L 	0.707106781f /* a1 = 0.707106781, left output gain of tapped delay line */
#define a0R 	0.7f /* a0 = 0.99999999, right input gain */
#define a1R 	0.707106781f /* a1 = 0.707106781, left output gain of tapped delay line */
#define afL 	0.507106781f /* a0 = 0.707106781, negative feedback gain */
#define afR 	0.507106781f /* a0 = 0.707106781, negative feedback gain */
/* = 0xA57D8666 for positive feedback, - 0.707106781 */

#define D1 		870 /* Depth, or TD = D1/fs = 870/48000 = 18 msec */
#define D2 		1120 /* Depth, or TD = D2/fs = 1120/48000 = 23 msec */
#define DepthL 	D1 /* DepthL is equivalent to time delay of signal, or d-line 1 size */
#define DepthR 	D2 /* DepthR is equivalent to time delay of signal, or d-line 2 size */
#define L_Del_TapCenter 	(DepthL/2) /* D1/2, used for add & subtracting delay from nominal tap center */
#define R_Del_TapCenter 	(DepthR/2) /* D2/2, used for add & subtracting delay from nominal tap center */
//#define WaveSize 			4000 /* semi-random/sinusoidal wavetable size*/

/* chorus control parameters */
//#define c 					1 /* wavetable signal freq fc1 = c1*(update time)*WaveSize = 4 Hz */
#define chorus_width 		12 /* Enter # from 1 to 31. Do not enter 0, to keep #'s +/- 0.5 */
//#define modulation_rate 	80 /* Update wavetable pointer for delay calc every 40 interrupts */
/* sine wavetable has a rate of modulation of about 0.15 Hz */
/* playing with the width, modulation rate, depth size and feedback affects the intensity of the chorus'ed sound */

typedef struct
{
	float_t 	amp;
	float_t 	freq;
	float_t 	phase;
	float_t		phi0;
	float_t 	out;
} Lfo_t;

/* Private variables ---------------------------------------------------------*/

static float feedback_gainL = afL;
static float feedback_gainR = afR;
//static float sweep_rate = modulation_rate;
static float sweep_widthL = chorus_width; /* controls width of left sweep, ranges from -1 to - 15 */
static float sweep_widthR = chorus_width; /* controls width of right sweep, ranges from -1 to - 15 */
static float dlineL[DepthL + 5]; /* delay-line buffer 1, max delay = D1 */
static float dlineR[DepthR + 5]; /* delay-line buffer 2, max delay = D2 */
static int32_t	inLindex;
static int32_t	inRindex;
static int32_t	outLindex;
static int32_t	outRindex;
static int32_t	outVarLindex;
static int32_t	outVarRindex;
static int32_t excursion_valueL; /* wave table offset value for delay calculation */
static int32_t excursion_valueR; /* wave table offset value for delay calculation */
static Lfo_t	lfoL, lfoR;
//static float random[WaveSize]="sinetbl.dat"; /* store one period of the wave table */
/* minimum frequency of table =
(freq of delay update)/WaveSize = 8/4 = 2 Hz */
//static float wavetbl_counter = 0x00000000;

/*-------------------------------------------------------------------------------------------*/
float RandomLfoSampleCompute(Lfo_t * op) //
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
void chorus_init(void)
{
	outLindex = outRindex = 0; // fixed center tap indexes
	outVarLindex = outVarRindex = 0; // moving around center tap indexes
	inLindex = L_Del_TapCenter;
	inRindex = R_Del_TapCenter;
	lfoL.amp = lfoR.amp = 1;
	lfoL.freq = .2f;
	lfoR.freq = .3f;
}
/*--------------------------------------------------------------------------------------------*/
float chorus_leftCompute(float x)
{
	float y;
	float vd_x; // variable delayed x
	float fd_x; // fixed delayed x

	/* update the input and output indexes */
	if (inLindex >= DepthL) inLindex = 0; else inLindex++;
	if (outLindex >= DepthL) outLindex = 0; else outLindex++;

	excursion_valueL = (int32_t)lrintf(sweep_widthL * RandomLfoSampleCompute(&lfoL));
	outVarLindex += 1 + excursion_valueL;
	if (outVarLindex > DepthL) outVarLindex -= DepthL;
	if (outVarLindex < 0) outVarLindex += DepthL + 1 ;


	dlineL[inLindex] = x; // write input sample in the delay line
	fd_x = dlineL[outLindex];
	vd_x = dlineL[outVarLindex];
	y = a0L*x - feedback_gainL*fd_x + a1L*vd_x ;

	return y;
}
/*---------------------------------------------------------------------------------------------*/
float chorus_rightCompute(float x)
{
	float y;
	float vd_x; // variable delayed x
	float fd_x; // fixed delayed x

	/* update the input and output indexes */
	if (inRindex >= DepthR) inRindex = 0; else inRindex++;
	if (outRindex >= DepthR) outRindex = 0; else outRindex++;

	excursion_valueR = (int32_t)lrintf(sweep_widthR * RandomLfoSampleCompute(&lfoR));
	outVarRindex += 1 + excursion_valueR;
	if (outVarRindex > DepthR) outVarRindex -= DepthR;
	if (outVarRindex < 0) outVarRindex += DepthR + 1 ;


	dlineR[inRindex] = x; // write input sample in the delay line
	fd_x = dlineR[outRindex];
	vd_x = dlineR[outVarRindex];
	y = a0R*x - feedback_gainR*fd_x + a1R*vd_x ;

	return y;
}
