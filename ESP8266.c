/*
 * ESP8266.c
 *
 *  Created on: 2020Äê10ÔÂ9ÈÕ
 *      Author: Jzjerry
 */

#include "ESP8266.h"

#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>
#include <Fliter.h>

#define ESP8266_BUFFER_SIZE 2048
#define TIMEOUT             32

static UART_Handle handle;
static UART_Params params;

extern sem_t Uart_sem;
extern uint8_t bpm_u;

extern uint8_t u8_ecg;
extern int32_t temp_result;
extern uint8_t Display_output[DISPLAY_INTERVAL];
extern int32_t old_max;
extern int32_t old_min;
extern char step_cnt;




char readtemp[100]={0};


char buff[100]={0};

char ESP8266_Buffer[ESP8266_BUFFER_SIZE]={0};

char a;


void *uartThread(void *arg)
{
    ESP8266_init();

    //uint8_t data1 = 24;
    //uint8_t data2 = 42;
    //const char buff[] = "hello";

    while(1)
    {
        sem_wait(&Uart_sem);
        int i = 0;
        for(i = 0; i<DISPLAY_INTERVAL; i++)
        {
            uint8_t usend = Display_output[i];
            uart_Sendu8((uint8_t)usend);
        }
        uart_Sendu8(bpm_u);
        uart_Sendu8(temp_result&0xff);
        uart_Sendu8(temp_result>>8);
        uart_Sendu8(step_cnt);
        usleep(10000);
    }
    //return NULL;
}


void ESP8266_init( void )
{
    UART_Params_init(&params);
    params.baudRate = 115200;
    params.writeDataMode = UART_DATA_BINARY;
    params.writeMode = UART_MODE_BLOCKING;
    params.writeTimeout = 230400;
    params.dataLength = UART_LEN_8;

    params.readDataMode = UART_DATA_TEXT;
    params.readReturnMode = UART_RETURN_NEWLINE;
    params.readMode = UART_MODE_BLOCKING;
    params.readEcho = UART_ECHO_OFF;

    handle = UART_open(CONFIG_UART_ESP, &params);
    if(handle == NULL)
    {
        while(1);
    }

    //ESP8266_Start();
    sleep(1);
    //ESP8266_Connect_Wifi();
    sleep(1);
    //ESP8266_TCP_Connect();
    sleep(1);
    //start direct transport
}

uint8_t int32_to_u8(int32_t data)
{
    uint8_t result;
    float32_t temp;
    uint32_t cov = (old_max-old_min)/192;

    temp = (float32_t)(data-old_min)/cov + 64;
    if(temp>255)
    {
        temp = 255;
    }
    else if(temp<0)
    {
        temp = 0;
    }
    result = (uint8_t)temp;
    return result;
}

void uart_SendString( const char* string )
{
    const char* ptr = string;
    int len = 0;
    while(*(ptr++)!='\0')
    {
        len++;
    }
    UART_write( handle, string, len);
}

void ESP8266_Reset( void )
{
    GPIO_write(CONFIG_ESP_RESET,0);
    usleep(15000);
    GPIO_write(CONFIG_ESP_RESET,1);
}

void ESP8266_Start( void )
{
    ESP8266_Send_Cmd("AT+RST\r\n");
    usleep(150000);
    ESP8266_Send_Cmd("AT+CWMODE=1\r\n");
    usleep(150000);
    ESP8266_Send_Cmd("AT+CIPMUX=0\r\n");
}

void ESP8266_Connect_Wifi( void )
{
    ESP8266_Send_Cmd("AT+CWJAP_DEF=\"IoT_209\",\"1145141919\"\r\n");
}

void ESP8266_TCP_Connect( void )
{
    ESP8266_Send_Cmd("AT+CIPMODE=1\r\n");
    sleep(1);
    ESP8266_Send_Cmd("AT+CIPSTART=\"TCP\",\"192.168.137.1\",9999\r\n");
    sleep(1);
    ESP8266_Send_Cmd("AT+CIPSEND\r\n");
}


void ESP8266_TCP_Stop( void )
{
    uart_SendString("+++");
}

void uart_Sendu8( uint8_t data )
{
    UART_write( handle, &data, 1);
}

bool ESP8266_Wait_For_Ans( uint32_t timeout )
{
    uint8_t c;
    uint32_t i = 0;
    while(timeout)
    {
        if(UART_read(handle, &ESP8266_Buffer, 50))
        {
            return true;
        }
        timeout--;
        usleep(2400);
    }
    return false;
}


bool ESP8266_Send_Cmd(const char* cmd)
{
    uart_SendString( cmd );
    /*
    int32_t i;
    for(i = 12000; i != 0; i--);
    if(!ESP8266_Wait_For_Ans(TIMEOUT))
    {
        return false;
    }

    if(strstr(ESP8266_Buffer,"OK") == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }*/
    return true;
}

char* Data_Packet_create( void )
{
    char* string;
    return string;
}
