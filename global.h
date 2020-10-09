/*
 * global.h
 *
 *  Created on: 2020Äê10ÔÂ1ÈÕ
 *      Author: HUAWEI
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

/* variables */
int32_t Led_1, Led_1_amb, Led_2, Led_2_amb, ecg, resp, motion, leadoff_detect;
uint16_t Tmp_1,Tmp_2,Tmp_3;
uint16_t pace_height,pace_width,pace_rise_time,pace_detected,patient_alarmed;

uint8_t AFE_read_done_flag;
uint8_t ADS_read_done_flag;


#endif /* GLOBAL_H_ */
