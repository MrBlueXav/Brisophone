/**
 ******************************************************************************
 * File Name          : drifter.h
 * Author			  : Xavier Halgand
 * Date               :
 * Description        :
 ******************************************************************************
 */
#ifndef __DRIFTER_H__
#define __DRIFTER_H__
/************************************************************************************/
#include  <stdint.h>
#include "random.h"
#include "CONSTANTS.h"

/*

 */
typedef struct
{
	float 	    fmax;
	float   	fmin;
	float   	gain;
	float 		minl;
	float 		maxl;
	uint32_t    length;
	uint32_t    n; //sample counter
	float		initial;
	float       final;
	float       slope;
	float    	out;

} Drifter_t;

//void 	drifter_newSegment (Drifter_t *d) ;
//float 	drifter_nextSample (Drifter_t *d) ;
void 	drifter_init (void) ;
float 	d1_drifter_nextSample(void) ;
float 	d2_drifter_nextSample(void) ;

/************************************************************************************/
#endif
