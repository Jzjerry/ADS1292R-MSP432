/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main.c ========
 */

/*
 *  FOR ADS1292R PINS :
 *
    CS      --->    P2.3
    SDIN    --->    P1.6
    SCLK    --->    P1.5
    DOUT    --->    P1.7
    DRDY    --->    P2.4

*/

/*
 *  FOR LMT70 PINS :
 *  VOUT    --->    P4.0
 */
/* For usleep() */

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/display/Display.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/Timer.h>
// #include <ti/drivers/Capture.h>
// #include <ti/drivers/Watchdog.h>
#include <pthread.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include "driverlib.h"

/* User Lib */
#include <ADS1292R.h>
#include <ADS1292R_SPI.h>
#include <LMT70_I2C.h>
#include <Fliter.h>
#include <Pan_Tompkins.h>
#include <LCD/HMI_Display.h>

/* User Variables */
static Display_Handle handle;
static Display_Params params;

int32_t channel_one[DEBUG_BUFFER_LENGTH];
int32_t channel_two[DEBUG_BUFFER_LENGTH];

float32_t FIR_input[SAMPLE_INPUT_LENGTH];
float32_t FIR_output[SAMPLE_INPUT_LENGTH];

uint8_t ADS_init_flag = 0;
int32_t result = 0;
int32_t pre_result = 0;
int32_t diff = 0;
int32_t last_diff = 0;
uint32_t counter = 0;
uint16_t temperature = 0;
int32_t temp_result = 0;


extern int32_t peak;
extern uint16_t rate;
/* User Interrupt */
void ads1292ReadyFxn(uint_least8_t index);

/* User Function */
bool led_thread_init( void );

bool uart_thread_init( void );

extern void *uartThread(void *arg);

void *ledThread (void *arg1)
{
    while(1)
    {
        if(result > 2500)
        {
            GPIO_write(CONFIG_GPIO_LED_0,1);
        }
        else
        {
            GPIO_write(CONFIG_GPIO_LED_0,0);
        }
        usleep(2000);
    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* 10 m second delay */
    // uint32_t time = 10000;

    /* Call driver init functions */
    GPIO_init();
    I2C_init();
    SPI_init();
    UART_init();
    ADC_init();
    Timer_init();
    // Watchdog_init();
    Display_init();
    Display_Params_init(&params);

    // ADC init;
    ADC_Params adc_params;
    ADC_Params_init(&adc_params);
    adc_params.isProtected = true;
    ADC_Handle adcHandle = ADC_open(CONFIG_ADC_0, &adc_params);

    handle = Display_open(Display_Type_UART, &params);

    Display_printf(handle, 0, 0, "Hello Bro!\r\n");
    /* Configure the GPIO pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_ADS1292_CS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    // GPIO_setConfig(CONFIG_ADS1292_RDY, GPIO_CFG_INPUT | GPIO_CFG_IN_INT_NONE );

    if(led_thread_init())
    {
        Display_printf(handle, 0, 0,"LED Thread Created!");
    }

    if(uart_thread_init())
    {
        Display_printf(handle, 0, 0,"Uart Thread Created!");
    }

    ADS1292_SPI_init();
    Display_printf(handle, 0, 0, "SPI Init Complete!\r\n");

    ADS1292_GInit();
    Display_printf(handle, 0, 0, "ADS Init Complete!\r\n");
    ADS_init_flag = 1;
    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    GPIO_setCallback(CONFIG_ADS1292_RDY, ads1292ReadyFxn);
    GPIO_enableInt(CONFIG_ADS1292_RDY);

    MSP_ADS_SPI_write_command(START);
    Filter_Init();
    //Pace_init();
    while (1)
    {
        if(ADS_read_done_flag)
        {
            ADS_read_done_flag = 0;
            result = ecg+88800;
            FIR_input[counter] = (float32_t)result;
            //Display_printf(handle, 0, 0, "out:%d,origin:%d,peak:%d", (int32_t)FIR_output[counter], result, peak);
            peak_detect(FIR_output[counter]);
            Display_printf(handle, 0, 0, "%f", Get_HeartPace());
            /*
            diff = (int32_t)FIR_output[counter] - pre_result;
            int32_t temp;
            temp = pan_tompkins(diff*diff);
            //Display_printf(handle, 0, 0,"%d ", Get_HeartPace());
            //Display_printf(handle, 0, 0,"Wave:%d,Peak:%d", temp, peak);
             */
            pre_result = (int32_t)FIR_output[counter];
            counter++;
            if(counter>=SAMPLE_INPUT_LENGTH)
            {
                counter = 0;
                LowPass_Filter(FIR_input, FIR_output);
            }

            ADC_convert(adcHandle, &temperature);
            temp_result = Slipper_Ave(temperature);
            temp_result = lmt70_get_temp(((float)temp_result)*2500/4096.0);

            //Display_printf(handle, 0, 0, "%.2f", (float)temp_result/100.0);
            /*
            result = Slipper_Ave( ecg+88800 );
            Display_printf(handle, 0, 0, "%d ", result);
            //Display_printf(handle, 0, 0,"%d", resp);
            */
            //usleep(500);
        }
    }
}

void ads1292ReadyFxn(uint_least8_t index)
{
    //TODO
    if(ADS_init_flag)
    {
    int i=0;
    int32_t * status_channel_data;
    uint32_t lead_status = 0;
    status_channel_data = ADS_read_ecg_resp_data();
    //if( respiration_valid ){ //update resp only if its valid. (not during pace detection time). Cannot send 0 due to filter
    //ADS_register_write(0x09, 0xEA); //set RESP_MOD_EN
    //}else{
    //      ADS_register_write(0x09, 0xAA); //unset RESP_MOD_EN
    //      }

    //status[i] = *(status_channel_data);
    //channel_1[i] = *(status_channel_data + 1);
    channel_two[i] = *(status_channel_data + 2);
    i++;
    lead_status = *(status_channel_data);
    lead_status = ((lead_status & 0x000F8000) >> 15);

    leadoff_detect = lead_status;

    resp = *(status_channel_data + 1);
    ecg = *(status_channel_data + 2);

    if(i>=DEBUG_BUFFER_LENGTH)
    {
        i = 0;
    }
    ADS_read_done_flag = 1;
    }
    return ;
}


bool led_thread_init()
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    pthread_attr_init(&attrs);

    priParam.sched_priority = 3;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, 512);
    if(retc != 0)
    {
        return false;
    }
    retc = pthread_create(&thread, &attrs, ledThread, NULL);
    return (retc == 0);
}

bool uart_thread_init()
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    pthread_attr_init(&attrs);

    priParam.sched_priority = 2;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, 512);
    if(retc != 0)
    {
        return false;
    }
    retc = pthread_create(&thread, &attrs, uartThread, NULL);
    return (retc == 0);
}
