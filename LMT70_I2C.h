/*
 * LMT70_I2C.h
 *
 *  Created on: 2020Äê10ÔÂ2ÈÕ
 *      Author: HUAWEI
 */

#ifndef LMT70_I2C_H_
#define LMT70_I2C_H_


#include "ti_drivers_config.h"
#include "ti/drivers/I2C.h"
#include "driverlib.h"
#include "global.h"



void LMT70_I2C_INIT(void);
float MSP_LMT70_I2C_read_data(void);
void MSP_LMT70_I2C_write_data(uint8_t data);
//void Board_ADC_LMT70_init(void);
int32_t lmt70_get_temp(float vol);

#endif /* LMT70_I2C_H_ */
