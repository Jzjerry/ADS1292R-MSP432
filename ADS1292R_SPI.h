/*
 * ASD1292R_SPI.h
 *
 *  Created on: 2020Äê10ÔÂ1ÈÕ
 *      Author: HUAWEI
 */

#ifndef ADS1292R_SPI_H_
#define ADS1292R_SPI_H_


#include "ti_drivers_config.h"

#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

#include <driverlib.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>


#include "global.h"

#include <ADS1292R.h>


void ADS1292_SPI_init( void );

void ADS1292_SPI_shutdown( void );

void Set_SPI_mode(uint8_t mode);

void MSP_ADS_SPI_write_register(uint8_t reg_address, uint8_t data);

void MSP_ADS_SPI_read_register(uint8_t reg_address, uint8_t* rx_buffer);

void MSP_ADS_SPI_write_command(uint8_t command);

void MSP_ADS_SPI_read_ecg_resp_data(uint32_t* status, uint32_t* channel_1, uint32_t* channel_2);

#endif /* ADS1292R_SPI_H_ */
