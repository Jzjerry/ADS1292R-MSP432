/*
 * HMI_Display.h
 *
 *  Created on: 2020Äê10ÔÂ9ÈÕ
 *      Author: Jzjerry
 */

#ifndef LCD_HMI_DISPLAY_H_
#define LCD_HMI_DISPLAY_H_


#include <ti/drivers/UART.h>
#include <stdint.h>
#include <stdbool.h>
#include "ti_drivers_config.h"




void HMI_init( void );

void *uartThread(void *arg);

#endif /* LCD_HMI_DISPLAY_H_ */
