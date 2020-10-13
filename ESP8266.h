/*
 * ESP8266.h
 *
 *  Created on: 2020Äê10ÔÂ9ÈÕ
 *      Author: Jzjerry
 */

#ifndef ESP8266_H_
#define ESP8266_H_


#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include "ti_drivers_config.h"


#define DISPLAY_INTERVAL 64


uint8_t int32_to_u8(int32_t data);

void ESP8266_init( void );

void *uartThread(void *arg);

void uart_SendString( const char* string );

void ESP8266_Reset( void );

void ESP8266_Start( void );

void ESP8266_Connect_Wifi( void );

void ESP8266_TCP_Connect( void );

void ESP8266_TCP_Stop( void );

bool ESP8266_Send_Cmd(const char* cmd);

bool ESP8266_Wait_For_Ans( uint32_t timeout );

void uart_Sendu8( uint8_t data );

char* Data_Packet_create( void );

#endif /* ESP8266_H_ */
