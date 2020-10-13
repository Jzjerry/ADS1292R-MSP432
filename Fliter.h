/*
 * Fliter.h
 *
 *  Created on: 2020Äê10ÔÂ2ÈÕ
 *      Author: Jzjerry
 */

#ifndef FLITER_H_
#define FLITER_H_



#define WIN_LENGTH 10
#define SAMPLE_INPUT_LENGTH 512
//#define FIR_LENGTH 49            //25Hz LP Filter
#define FIR_LENGTH 64            //40Hz LP Filter

#define FIR_Notch_LENGTH 65

#include <math.h>
#include <stdint.h>
#include <arm_math.h>
#include <arm_const_structs.h>
//#include "arm_fir_init_f32.c"
//#include "arm_fir_f32.c"


void Filter_Init( void );


void LowPass_Filter( float32_t* input, float32_t *output );


int32_t DualBound( const uint32_t limit, int32_t data );

int32_t Slipper_Ave( int32_t data );

int32_t MaMeMi( int32_t data );


#endif /* FLITER_H_ */
