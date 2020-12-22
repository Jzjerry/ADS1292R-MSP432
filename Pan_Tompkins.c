/*
 * Pan_Tompkins.c
 *
 *  Created on: 2020Äê10ÔÂ8ÈÕ
 *      Author: Jzjerry
 *
 */

#define INTERGATE_SIZE 64

#include "Pan_Tompkins.h"


static Timer_Handle    handle;
static Timer_Params    params;

static int32_t sum[INTERGATE_SIZE+1]={0};

int32_t peak = 0;
int32_t depth = 0;
int32_t peak_now = 0;


int32_t old_max = 0;
int32_t old_min = 0;

int32_t new_max = -0xffffff;
int32_t new_min = 0xffffff;

int32_t detect_count = 0;

static int32_t last_diff = 0;
static int32_t diff = 0;
static int32_t last_result = 0;


int32_t get_count = 0;
uint16_t rate = 0;
float32_t noise_level = 1000000;
float32_t signal_level = 6000000;
float32_t threshold = 1000;
static bool peak_lock = true;


extern int32_t ecg_offset;


// NOTICE:This function have not put into use
int32_t pan_tompkins(int32_t data)
{
    static uint8_t win_now = 1;
    static uint8_t win_max = INTERGATE_SIZE;
    static uint8_t ptr = 0;
    static int32_t result = 0;
    //threshold = noise_level + 0.25*(signal_level - noise_level);

    result = 0;
    sum[ptr] = data;
    int i = 0;
    for(i = 0;i < win_now;i++)
    {
        result += sum[i];
    }
    if( win_now < win_max )
    {
        win_now++;
    }
    ptr++;
    if( ptr >= win_max )
    {
        ptr = 0;
    }
    diff = result - last_result;
    // Detected a Peak
    /*
    if(((diff<0)&&(last_diff>0))&&(peak_lock))
    {
        if( result < threshold )
        {
            noise_level = 0.125*result + 0.875*noise_level;
        }
        else
        {
            signal_level = 0.125*result + 0.875*signal_level;
            peak = result;
            peak_lock = false;
            Timer_start( handle );
        }
    }
    last_result = result;
    last_diff = diff;
    */
    return result;
}


void Pace_init( void )
{
    Timer_Params_init(&params);
    params.periodUnits = Timer_PERIOD_US;
    params.period = 300000;             //300ms
    params.timerMode  = Timer_ONESHOT_CALLBACK;
    params.timerCallback = lock_callback;
    handle = Timer_open(CONFIG_TIMER_PACE, &params);
    if (handle == NULL)
    {
        // Timer_open() failed
        while (1);
    }
}

float Get_HeartPace( void )
{
    float bpm;
    bpm = 60.0/((float)rate*0.002);
    return bpm;
}

void lock_callback(Timer_Handle myHandle)
{
    peak_lock = true;
}

void peak_detect( int32_t data)
{
    int32_t result = data;
    diff = result - last_result;
    if(((diff<0)&&(last_diff>0)))
    {
        if(peak_lock)
        {
            if( result > threshold )
            {
                peak = result;
                peak_now = result;
                rate = get_count;
                get_count = 0;
                peak_lock = false;
                Timer_start( handle );
            }
        }

        if(result > new_max)
        {
            new_max = result;
        }
    }
    else if((diff>0)&&(last_diff<0))
    {
        depth = result;
        if(result < new_min)
        {
            new_min = result;
        }
    }
    if( detect_count >= 750 )
    {
        if(new_max-new_min>192)
        {
            old_max = new_max;
            old_min = new_min;

            threshold = (old_max-old_min)*0.9 + old_min;
        }
        new_max = -0xffffff;
        new_min = 0xffffff;
        detect_count = 0;
    }
    get_count++;
    detect_count++;
    last_result = result;
    last_diff = diff;
}
