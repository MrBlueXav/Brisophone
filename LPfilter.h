/******************************************************************************
 * File Name          : LPfilter.h
 * Author				: (c) 2001-2005 Nick Dowell (amSynth) + Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
/* from amSynth
 * (c) 2001-2005 Nick Dowell
 */
/**
 * A 24 dB/octave resonant low-pass filter.
 **/
#ifndef _LPFILTER_H
#define _LPFILTER_H

#include <math.h>
#include "CONSTANTS.h"
//----------------------------------------------------------------------
void 	LPfilter_reset(void);
void 	LPfilter_computeCoeff (float fc, float res);
float	LPfilter_processSample	(float input);
//----------------------------------------------------------------------

#endif
