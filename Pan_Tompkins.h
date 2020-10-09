/*
 * Pan_Tompkins.h
 *
 *  Created on: 2020Äê10ÔÂ8ÈÕ
 *      Author: Jzjerry
 */

#ifndef PAN_TOMPKINS_H_
#define PAN_TOMPKINS_H_


#include <arm_math.h>
#include <stdint.h>
#include <stdbool.h>
#include <ti/drivers/Timer.h>
#include "ti_drivers_config.h"

int32_t pan_tompkins(int32_t data);
void Pace_init( void );
float Get_HeartPace( void );
void peak_detect(int32_t data);
void lock_callback(Timer_Handle myHandle);



#endif /* PAN_TOMPKINS_H_ */
