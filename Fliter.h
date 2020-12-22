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
#define FIR_LENGTH 64            //40Hz LP Filter

#define FIR_Notch_LENGTH 65

#include <math.h>
#include <stdint.h>
#include <arm_math.h>
#include <arm_const_structs.h>


void Filter_Init( void );

void LowPass_Filter( float32_t* input, float32_t *output );

int32_t Slipper_Ave( int32_t data );


#endif /* FLITER_H_ */
