/*
 * HMI_Display.c
 *
 *  Created on: 2020Äê10ÔÂ9ÈÕ
 *      Author: Jzjerry
 */

#include "HMI_Display.h"


#include <pthread.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>


static UART_Handle handle;
static UART_Params params;



void *uartThread(void *arg)
{
    HMI_init();
    return NULL;
}


void HMI_init( void )
{
    UART_Params_init(&params);
    params.baudRate = 115200;
    params.writeDataMode = UART_DATA_TEXT;
    params.writeMode = UART_MODE_BLOCKING;
    /*
    params.readDataMode = UART_DATA_TEXT;
    params.readReturnMode = UART_RETURN_FULL;
    params.readEcho = UART_ECHO_OFF;
    */
    handle = UART_open(CONFIG_UART_HMI, &params);
    if(handle == NULL)
    {
        while(1);
    }
}

