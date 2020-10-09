/*
 * Serial_Display.h
 *
 *  Created on: 2020��10��1��
 *      Author: Jzjerry
 */

#ifndef SERIAL_DISPLAY_H_
#define SERIAL_DISPLAY_H_


/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>



#include <ti/display/Display.h>

#include "ti_drivers_config.h"

void SerialDisplayInit( void );


#endif /* SERIAL_DISPLAY_H_ */
