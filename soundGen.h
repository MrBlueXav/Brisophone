/**
 ******************************************************************************
 * File Name          : soundGen.h
 * Author				: Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOUNDGEN_H
#define __SOUNDGEN_H
/*--------------------- Global Defines ------------------------ */

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "CONSTANTS.h"
#include "random.h"
#include "drifter.h"
#include "delay.h"
#include "chorusFD.h"
#include "sinetable.h"
#include "notesTables.h"
#include "LPfilter.h"

typedef struct
{
	uint8_t		sound; // waveform function number
	float_t 	amp;
	float_t		last_amp;
	float_t 	freq;
	float_t 	phase;
	float_t		phi0;
	float_t		decayFactor;
	float_t 	out;

} Oscillator;

typedef struct
{
	float_t 	s[9];		// summit values, s[8] = s[0]
	float_t 	coeff[8];	// intermediate coefficients to accelerate computing
	bool 		autom;		// random notes ?
	bool		glide;		// glissando between notes ?
	bool		chRequest;
	bool		someNotesMuted;
	uint8_t		step;
	uint8_t 	stage; // from 0 to 7
	uint8_t 	oldstage;
	float_t 	max;
	float_t 	amp;
	float_t 	freq;
	float_t 	phase;
	float_t 	out;

} EightSegGenerator;

/* Exported functions ------------------------------------------------------- */
void toggleVibrato(void);
void toggleSynthOut(void);
void AdditiveGen_newWaveform(void);
void transpUp(void);
void transpDown(void);
void nextScale(void);
void prevScale(void);
void incSynthOut(void);
void decSynthOut(void);
void toggleDelay(void);
void toggleFilter(void);
void toggleChorus(void);
void incDecay(void);
void decDecay(void);
void nextSound(void);
void prevSound(void);
void pitchGenInit(void);
void pitchGenChangePoints(void);
void pitchGenRequestChangePoints(void);
void pitchGen_setMaxFreq( float_t fr);
void automatic_or_manual(void);
void toggleGlide(void);
void muteSomeNotes(void);
void doubleTempo(void);
void halfTempo(void);
void incTempo(void);
void decTempo(void);
void incMaxFreq(void);
void decMaxFreq(void);
void pitchGenSetFreq (float_t f);
void pitchGenResetPhase(void);
void soundGenInit(void);
void soundGenNewWave(void);
void Synth_Init(void);
void make_sound(uint16_t offset, uint16_t len);
void OpsRandFreq(void);

#endif /* __SOUNDGEN_H */
