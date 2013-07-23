/**
 ******************************************************************************
 * File Name          : soundGen.c
 * Author				: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */

#include "soundGen.h"

#define PARTIALS_NUMBER		10
#define DELTAPHI			(_2PI/8)
#define EPSI				.00002f
#define MAX_SOUNDS			15 // number of different sounds (starts at 0)

/*-------------------------------------------------------*/

extern uint16_t audiobuff[];

static Oscillator op1 , op2 , op3, lfo  ;
static EightSegGenerator pitchGen ;

static float_t	a[PARTIALS_NUMBER + 1] ;
static float_t	ph[PARTIALS_NUMBER + 1] ;

static bool 	filterON = false;
static bool		delayON = true;
static bool 	chorusON = false;

static float 	vol, env, decayFactor;

static uint8_t 	scaleIndex;
static uint8_t	*currentScale;
static uint8_t	octaveSpread = 4;
static uint8_t	rootNote = 36;
static int8_t	transpose;

const float_t slicePhase[9] = {0, DELTAPHI, 2*DELTAPHI, 3*DELTAPHI, 4*DELTAPHI, 5*DELTAPHI, 6*DELTAPHI, 7*DELTAPHI, 8*DELTAPHI};

/*===============================================================================================================*/

void toggleVibrato(void)
{
	if (lfo.amp !=0)
	{
		lfo.last_amp = lfo.amp;
		lfo.amp = 0;
	}
	else
		lfo.amp = lfo.last_amp;
}
void toggleSynthOut(void)
{
	if (op1.amp !=0)
	{
		op1.last_amp = op1.amp;
		op1.amp = 0;
		op2.last_amp = op2.amp;
		op2.amp = 0;
		op3.last_amp = op3.amp;
		op3.amp = 0;

	}
	else
	{
		op1.amp = op1.last_amp;
		op2.amp = op2.last_amp;
		op3.amp = op3.last_amp;
	}
}
/*-------------------------------------------------------*/
void transpUp(void)
{
	if (rootNote < (MAX_NOTE_INDEX - 12))
	{
		transpose = 1;
	}
}
/*-------------------------------------------------------*/
void transpDown(void)
{
	if (rootNote > FIRST_NOTE)
	{
		transpose = -1;
	}
}
/*-------------------------------------------------------*/
void chooseScale(void)
{
	switch (scaleIndex)
	{
	case 0 : 	currentScale = MIDIscale13;	break ;
	case 1 :  	currentScale = MIDIscale14;	break;
	case 2 : 	currentScale = MIDIscale07;	break;
	case 3 : 	currentScale = MIDIscale08;	break;
	case 4 : 	currentScale = MIDIscale09;	break;
	case 5 : 	currentScale = MIDIscale10;	break;
	case 6 : 	currentScale = MIDIscale04;	break;
	case 7 : 	currentScale = MIDIscale01;	break;
	case 8 : 	currentScale = MIDIscale03;	break;
	case 9 : 	currentScale = MIDIscale11;	break;
	case 10 : 	currentScale = MIDIscale02;	break;
	case 11 : 	currentScale = MIDIscale06;	break;
	case 12 : 	currentScale = MIDIscale05;	break;
	case 13 : 	currentScale = MIDIscale12;	break;
	case 14 : 	currentScale = MIDIscale11;	break;
	default :	currentScale = MIDIscale11; break ;
	}
	pitchGenRequestChangePoints();
}
/*-------------------------------------------------------*/
void nextScale(void)
{
	if (scaleIndex < MAX_SCALE_INDEX)
	{
		scaleIndex++;
		chooseScale();
	}
}
/*-------------------------------------------------------*/
void prevScale(void)
{
	if (scaleIndex > 0)
	{
		scaleIndex--;
		chooseScale();
	}
}

/*-------------------------------------------------------*/
void incSynthOut(void)
{
	op1.amp *= 1.2f;
	op2.amp *= 1.2f;
	op3.amp *= 1.2f;
}
/*-------------------------------------------------------*/
void decSynthOut(void)
{
	op1.amp *= .8f;
	op2.amp *= .8f;
	op3.amp *= .8f;
}
/*-------------------------------------------------------*/
void toggleDelay(void)
{
	if (delayON)
		{
			delayON = false;
			delay_Clean();
		}
	else delayON = true;
}
/*-------------------------------------------------------*/
void toggleFilter(void)
{
	if (filterON) filterON = false;
	else filterON = true;
}
/*-------------------------------------------------------*/
void toggleChorus(void)
{
	if (chorusON) chorusON = false;
	else chorusON = true;
}
/*-------------------------------------------------------*/
void automatic_or_manual(void)
{
	if (pitchGen.autom) pitchGen.autom = false;
	else pitchGen.autom = true;
}
/*-------------------------------------------------------*/
void toggleGlide(void)
{
	if (pitchGen.glide) pitchGen.glide = false;
	else pitchGen.glide = true;
}
/*-------------------------------------------------------*/
void muteSomeNotes(void)
{
	if (pitchGen.someNotesMuted) pitchGen.someNotesMuted = false;
	else pitchGen.someNotesMuted = true;
}
/*-------------------------------------------------------*/
void doubleTempo(void)
{
	if (pitchGen.freq <= 5) pitchGen.freq *= 2;
}
/*-------------------------------------------------------*/
void halfTempo(void)
{
	if (pitchGen.freq >= .05f) pitchGen.freq *= 0.5f;
}
/*-------------------------------------------------------*/
void incTempo(void)
{
	if (pitchGen.freq <= 5) pitchGen.freq += 0.01f;
}
/*-------------------------------------------------------*/
void decTempo(void)
{
	if (pitchGen.freq >= .05f) pitchGen.freq -= 0.01f;
}
/*-------------------------------------------------------*/
void incMaxFreq(void)
{
	if (octaveSpread < 8)
	{
		octaveSpread++;
		pitchGenChangePoints();
	}
}
/*-------------------------------------------------------*/
void decMaxFreq(void)
{
	if (octaveSpread > 0)
	{
		octaveSpread--;
		pitchGenChangePoints();
	}
}
/*-------------------------------------------------------*/
void incDecay(void)
{
	if (decayFactor < 1 - EPSI) decayFactor += EPSI;
}
/*-------------------------------------------------------*/
void decDecay(void)
{
	if (decayFactor > EPSI) decayFactor -= EPSI;
}
/*-------------------------------------------------------*/
void pitchGen_setMaxFreq( float_t fr)
{
	pitchGen.max = fr;
}
/*-------------------------------------------------------*/
void pitchGenRequestChangePoints(void)
{
	pitchGen.chRequest = true;
}
/*-------------------------------------------------------*/
void AdditiveGen_newWaveform(void) //
{
	uint8_t k;

	a[0] = a[1] = 1;

	for ( k= 2; k <= PARTIALS_NUMBER ; k++)
	{
		a[k] = frand_a_b(0.f , 0.9f);
		a[0] += a[k];
	}
	a[0] = 1.f / a[0];
}
/*-------------------------------------------------------*/
void nextSound(void)
{
	if (op1.sound < MAX_SOUNDS) (op1.sound)++ ; else op1.sound = MAX_SOUNDS;
}
/*-------------------------------------------------------*/
void prevSound(void)
{
	if (op1.sound > 0) (op1.sound)-- ; else op1.sound = 0;
}
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
void genInit(EightSegGenerator * gen, float_t amp, float_t freq)
{
	gen->amp = amp;
	gen->freq = freq;
	gen->phase = 0;
	gen->out = 0;
	gen->max = 80;
	gen->glide = false;
}

/*-------------------------------------------------------*/
void gen_NewPoints(EightSegGenerator * gen)
{
	uint8_t i;

	for (i = 0; i <= 7; i++)
	{
		gen->s[i] = frand_a_b(-1 , 1); // [-1 ; 1] interval
	}
	gen->s[8] = gen->s[0];
}

/*-------------------------------------------------------*/
void pitchGen_NewPoints(EightSegGenerator * gen)
{
	uint16_t i, relativeNote, octaveShift, index;

	for (i = 0; i <= 7; i++)
	{
		//gen->s[i] = notesfreq[lrintf(frand_a_b(0 , gen->max))];
		relativeNote = currentScale[lrintf(frand_a_b(1 , currentScale[0]))];
		octaveShift = 12 * lrintf(frand_a_b(0 , octaveSpread));
		index = rootNote + octaveShift + relativeNote - FIRST_NOTE;

		while (index > MAX_NOTE_INDEX) index -= 12;
		while (index < 0) index += 12;

		gen->s[i] = notesFreq[index];
	}
	gen->s[8] = gen->s[0];
}
/*-------------------------------------------------------*/
/*
void soundGen_NewPoints(EightSegGenerator * gen)
{
	uint8_t i;

	for (i = 0; i <= 7; i++)
	{
		gen->s[i] = frand_a_b(-1 , 1);
	}
	gen->s[8] = gen->s[0];
}
 */

/*-------------------------------------------------------*/
void gen_DCblock(EightSegGenerator * gen)
{
	float_t moy = 0;
	uint8_t i;

	for (i = 0; i <= 7; i++)
		moy += gen->s[i];
	moy /= 8;
	for (i = 0; i <= 7; i++)
		gen->s[i] -= moy;
	gen->s[8] = gen->s[0];
}

/*-------------------------------------------------------*/
void gen_NormalizePoints(EightSegGenerator * gen)
{
	float_t max = 0, invmax, val;
	uint8_t i;

	for (i = 0; i <= 7; i++)
	{
		val = fabsf(gen->s[i]);
		if (val > max) max = val;
	}
	if (max != 0)
	{
		invmax = 1.f / max;
		for (i = 0; i <= 7; i++)
			gen->s[i] *= invmax;
		gen->s[8] = gen->s[0];
	}
}

/*-------------------------------------------------------*/
void gen_ComputeCoeff(EightSegGenerator * gen)
{
	uint8_t i;

	for (i = 0; i <= 7; i++)
		gen->coeff[i] = (gen->s[i+1] - gen->s[i]) / DELTAPHI;
}

/*-------------------------------------------------------*/
void pitchGenInit(void)
{
	scaleIndex = 0;
	octaveSpread = 4;
	rootNote = 36;
	currentScale = MIDIscale13;
	genInit(&pitchGen, 1, 1);
	pitchGen_NewPoints(&pitchGen);
	gen_ComputeCoeff(&pitchGen);

}
/*-------------------------------------------------------*/
/*void soundGenInit(void)
{
	genInit(&soundGen, 1, 500);
	soundGen_NewPoints(&soundGen);
	gen_DCblock(&soundGen);
	gen_NormalizePoints(&soundGen);
	gen_ComputeCoeff(&soundGen);
}
-------------------------------------------------------
void soundGenNewWave(void)
{
	soundGen_NewPoints(&soundGen);
	gen_DCblock(&soundGen);
	gen_NormalizePoints(&soundGen);
	gen_ComputeCoeff(&soundGen);
}*/
/*-------------------------------------------------------*/
void pitchGenChangePoints(void)
{
	pitchGen_NewPoints(&pitchGen);
	gen_ComputeCoeff(&pitchGen);
}
/*-------------------------------------------------------*/
void pitchGenResetPhase(void)
{
	pitchGen.phase = 0;
}
/*-------------------------------------------------------*/
float_t gen_SampleCompute(EightSegGenerator * gen)
{
	uint8_t i;

	while (gen->phase < 0) // keep phase in [0, 2pi]
		gen->phase += _2PI;
	while (gen->phase >= _2PI)
		gen->phase -= _2PI;

	for (i = 0; i <= 7; i++)
	{
		if ((gen->phase >= slicePhase[i]) && (gen->phase < slicePhase[i+1]))
		{
			gen->out = gen->amp * (gen->step * (gen->coeff[i]) * (gen->phase - slicePhase[i]) + gen->s[i]) ;
			gen->stage = i;
			goto sortie;
		}
	}
	sortie :

	gen->phase += _2PI * Ts * gen->freq; // increment phase

	return gen->out;
}

/*===============================================================================================================*/

float_t OpSampleCompute0(Oscillator * op) // accurate sine
{
	float_t z;

	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	z = sinf(op->phase);
	op->out = op->amp*z;

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}

/*-------------------------------------------------------*/
float_t OpSampleCompute1(Oscillator * op) // basic sawtooth^2
{
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->phase < _PI) op->out = 0.5f*op->amp * (op->phase / _PI)*(op->phase / _PI);
	else op->out = - .5f*(op->amp * (op->phase / _PI - 2)*(op->phase / _PI - 2));

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t OpSampleCompute2(Oscillator * op) // basic sawtooth
{
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->phase < _PI) op->out = 0.5f*op->amp * (op->phase / _PI);
	else op->out =  .5f*op->amp * (op->phase / _PI - 2);

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t OpSampleCompute3(Oscillator * op) // sin(phi)^5
{
	float_t z;

	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	z = sinf(op->phase);
	op->out = op->amp*z*z*z*z*z;

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t OpSampleCompute4(Oscillator * op) // Complex waveform : +/- |sin(phi)|^alpha(freq), tends to a sine at high freqs
{
	float_t z, x, alpha;

	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	x = op->freq;

	//http://www.math.ucla.edu/~ronmiech/Interpolation/HTMDOCS/Introduction/Interpolation_Applet.htm  :
	alpha = 81.096f -.037f * x + .582E-5f * x*x -.311E-9f * x*x*x ; //alpha varies from 80 to 1 with freq from 30Hz to 8000Hz
	//alpha = 60.695f -.023f * x + .31E-5f * x*x -.141E-9f * x*x*x ;
	//alpha = (50.f-1)/(30.f-8000)*(x - 8000) + 1 ;
	z = powf(fabsf(sinf(op->phase )), alpha) ;

	if (op->phase < _PI) op->out = op->amp * z;
	else op->out = - op->amp * z;

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t OpSampleCompute5(Oscillator * op) // Basic Triangle
{
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->phase < _PI) op->out = op->amp * (-2/_PI * op->phase + 1);
	else op->out =  op->amp * (2/_PI * op->phase - 3);

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t OpSampleCompute6(Oscillator * op) // Morphing sawtooth, tends to a triangle at high freqs
{
	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	if (op->freq < 8000) op->phi0 = _PI/8000*(op->freq); else op->phi0 = _PI;
	if (op->phi0 <= .0001f) op->phi0 = .0001f;
	if (op->phase < op->phi0) op->out = op->amp * (-2/op->phi0 * op->phase + 1);
	else op->out =  op->amp * (2 * op->phase - _2PI - op->phi0)/(_2PI - op->phi0);

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}

/*-------------------------------------------------------*/
float_t OpSampleCompute7(Oscillator * op) // basic wave table sine
{
	float_t z;

	while (op->phase < 0) // keep phase in [0, 2pi]
		op->phase += _2PI;
	while (op->phase >= _2PI)
		op->phase -= _2PI;

	z = sinetable[lrintf(ALPHA * (op->phase))];
	op->out = op->amp*z;

	op->phase += _2PI * Ts * op->freq; // increment phase
	return op->out;
}
/*-------------------------------------------------------*/
float_t AdditiveGen_SampleCompute(Oscillator * op) // additive sine generator
{
	uint8_t k=1;
	float_t y = 0;

	while ((k <= PARTIALS_NUMBER ) && (k * op->freq < SAMPLERATE/2.f))
	{
		while (ph[k] < 0) // keep phase in [0, 2pi]
			ph[k] += _2PI;
		while (ph[k] >= _2PI)
			ph[k] -= _2PI;
		y += a[k] * sinetable[lrintf(ALPHA * (ph[k]))];
		ph[k] += _2PI * Ts * k * op->freq; // increment phase
		k++;
	}
	y = a[0] * y;

	op->out = op->amp * y;

	return op->out;
}

void
OpInit(Oscillator * op, float_t amp, float_t freq)
{
	op->sound = 0;
	op->amp = amp;
	op->last_amp = amp;
	op->freq = freq;
	op->phase = 0;
	op->out = 0;
}
/*-------------------------------------------------------*/

void
OpSetFreq(Oscillator * op, float_t f)
{
	op->freq = f;
}
/*-------------------------------------------------------*/

void
OpSetPhaseAdd(Oscillator * op, float_t phi)
{
	op->phase += phi;
}

/*-------------------------------------------------------*/

void
Synth_Init(void)
{
	vol = env = 1;
	decayFactor = 0.99975f;
	LPfilter_reset();
	OpInit(&op1, 0.8f, 587.f);
	OpInit(&op2, 0.8f, 587.f);
	OpInit(&op3, 0.8f, 587.f);
	OpInit(&lfo, 0.01f, 4.f);
	AdditiveGen_newWaveform();

	}

/*===============================================================================================================*/

void make_sound(uint16_t offset, uint16_t len)
{
	uint16_t pos;
	uint16_t *outp;
	float_t y = 0, f0;
	float_t yL, yR ;
	uint16_t valueL, valueR;

	outp = audiobuff + offset;

	for (pos = 0; pos < len; pos++)
	{
		f0 = gen_SampleCompute(&pitchGen) * (1 +  OpSampleCompute7(&lfo)); // main frequency + vibrato modulation
		OpSetFreq(&op1, f0);


		if ((pitchGen.stage != pitchGen.oldstage))
		{
			// this is a new note !
			vol = frand_a_b(0.4f , .8f); // random volume for each note

			if (transpose != 0) { rootNote += transpose ; pitchGenChangePoints(); transpose = 0; }

			LPfilter_computeCoeff( frand_a_b(300 , 5000), .81f);
			if (pitchGen.someNotesMuted) env = roundf(frand_a_b(0.4f , 1)); // env = 1 or sometimes 0
			else env = 1; // reset envelope

			if (pitchGen.glide) pitchGen.step = rintf(frand_a_b(0 , 1)); else pitchGen.step = 0;

			if (pitchGen.stage == 0 && (pitchGen.autom || pitchGen.chRequest))
			{
				// A new sequence begins ...
				pitchGenChangePoints();
				pitchGen.chRequest = false;
			}
		}

		pitchGen.oldstage =pitchGen.stage;

		env *= decayFactor; // apply envelope decay

		/* choose waveform generator */
		switch (op1.sound)
		{
		case 0 : 	y = vol * env * 0.8f * OpSampleCompute6(&op1); 		break ;

		case 1 :  	{
			if (f0 < 200) y = vol * env * OpSampleCompute1(&op1);
			else if (f0 < 600) y = vol * env * OpSampleCompute2(&op1);
			else y = vol * env * OpSampleCompute5(&op1);
		} 	break;

		case 2 : 	y = vol * env * .8 * OpSampleCompute0(&op1); 	break;
		case 3 : 	y = vol * env * OpSampleCompute3(&op1);		break;
		case 4 : 	y = vol * env * OpSampleCompute5(&op1);		break;
		case 5 : 	y = vol * env * OpSampleCompute4(&op1);		break;
		case 6 : 	y = vol * env * AdditiveGen_SampleCompute(&op1);		break;
		case 7 : 	y = vol * env * 0.8f * OpSampleCompute7(&op1);		break;
		case 8 : 	y = vol * env * op1.amp * frand_a_b(-.8f , .8f);		break;

		case 9 : 	{	// fundamental + fifth : 1 5
			OpSetFreq(&op2, f0 * 1.50f);
			y = vol * env * .5f *(OpSampleCompute6(&op1) + OpSampleCompute6(&op2));
		} break;

		case 10 :	{	// major chord : 1 3maj 5
			OpSetFreq(&op2, f0 * 1.26f);
			OpSetFreq(&op3, f0 * 1.5f);
			y = vol * env * .33f *(OpSampleCompute6(&op1) + OpSampleCompute6(&op2) + OpSampleCompute6(&op3));
		} break;

		case 11 :	{	// minor chord : 1 3min 5
			OpSetFreq(&op2, f0 * 1.1892f);
			OpSetFreq(&op3, f0 * 1.5f);
			y = vol * env * .33f *(OpSampleCompute6(&op1) + OpSampleCompute6(&op2) + OpSampleCompute6(&op3));
		} break;

		case 12 :	{
			OpSetFreq(&op2, f0 * 0.996f);
			OpSetFreq(&op3, f0 * 1.006f);
			y = vol * env * .33f *(OpSampleCompute6(&op1) + OpSampleCompute6(&op2) + OpSampleCompute6(&op3));
		} break;

		case 13 :	{

			OpSetFreq(&op2, f0 * (1 + d1_drifter_nextSample()));
			OpSetFreq(&op3, f0 * (1 + d2_drifter_nextSample()));
			y = vol * env * .33f *(OpSampleCompute6(&op1) + OpSampleCompute6(&op2) + OpSampleCompute6(&op3));
		} break;

		default :	y = 0;	break ;

		}

		/* Apply Low pass filter effect */
		if (filterON)	y = LPfilter_processSample (y);

		/*  Apply delay effect */
		if (delayON) 	y = delay_Compute(y);

		/* Apply chorus/flanger effect */
		if (chorusON) stereoChorus_compute (&yL, &yR, y) ;
		else yL = yR = y;

		/* clipping */
		yL = (yL > 1.0f) ? 1.0f : yL; //clip too loud left samples
		yL = (yL < -1.0f) ? -1.0f : yL;

		yR = (yR > 1.0f) ? 1.0f : yR; //clip too loud right samples
		yR = (yR < -1.0f) ? -1.0f : yR;

		/****** let's hear the new sample *******/

		valueL = (uint16_t)((int16_t)((32767.0f) * yL)); // conversion float -> int
		valueR = (uint16_t)((int16_t)((32767.0f) * yR));

		*outp++ = valueL; // left channel sample
		*outp++ = valueR; // right channel sample
	}
}
/*-------------------------------------------------------*/
