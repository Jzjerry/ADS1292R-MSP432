/*
 * Fliter.c
 *
 *  Created on: 2020Äê10ÔÂ2ÈÕ
 *      Author: Jzjerry
 */

#include "Fliter.h"


/* Global Variables */

// MaMeMi Variables
static uint8_t first_signal = 1;
const int32_t delta = 2;
const int32_t cap_delta = 2;


//45Hz Filter
/*
const int NumTap = 37;
const float32_t FIR_CoffArray[37] = {
   -0.01100959722,-0.003212515032, 0.006748287473,  0.01591334678,  0.02115010656,
    0.02010216564,  0.01202915516, -0.00171784847, -0.01775931381, -0.03132567927,
   -0.03735112399,  -0.0317681022, -0.01267499756,  0.01893868297,    0.059127599,
     0.1016436517,   0.1391815841,   0.1649356186,   0.1740979552,   0.1649356186,
     0.1391815841,   0.1016436517,    0.059127599,  0.01893868297, -0.01267499756,
    -0.0317681022, -0.03735112399, -0.03132567927, -0.01775931381, -0.00171784847,
    0.01202915516,  0.02010216564,  0.02115010656,  0.01591334678, 0.006748287473,
  -0.003212515032, -0.01100959722
};
*/

//25Hz Filter
const float32_t FIR_CoffArray[49] = {
      0.012129765,  0.01082179043,  0.00825996045, 0.004570450168,-3.820522256e-18,
  -0.005095176864, -0.01027066447, -0.01502404083, -0.01883191243, -0.02119101211,
    -0.0216601491, -0.01989955455, -0.01570430025,-0.009028786793,3.944718462e-18,
    0.01108198613,   0.0237567164,  0.03742872179,   0.0514038913,  0.06493435055,
    0.07726853341,  0.08770269156,   0.0956299305,   0.1005830169,   0.1022675559,
     0.1005830169,   0.0956299305,  0.08770269156,  0.07726853341,  0.06493435055,
     0.0514038913,  0.03742872179,   0.0237567164,  0.01108198613,3.944718462e-18,
  -0.009028786793, -0.01570430025, -0.01989955455,  -0.0216601491, -0.02119101211,
   -0.01883191243, -0.01502404083, -0.01027066447,-0.005095176864,-3.820522256e-18,
   0.004570450168,  0.00825996045,  0.01082179043,    0.012129765
};


const int LENGTH = 64;
/*
const float32_t FIR_CoffArray[64] = {
  -0.001387581113, 0.003459413303, 0.007736497093,  0.01030727103,  0.01037941873,
   0.007730200887,  0.00280758366,-0.003327071201,-0.009212699719, -0.01331851631,
   -0.01441918314, -0.01192791108,-0.006103678606, 0.001927779871,  0.01036039367,
    0.01707590371,  0.02012399584,  0.01821358502,  0.01110421401,-0.0001993334008,
   -0.01350942533, -0.02578292973, -0.03366438672,  -0.0341463089, -0.02521691658,
  -0.006360886153,  0.02119792439,  0.05457502976,  0.08962567151,    0.121602647,
     0.1459534913,   0.1591083556,   0.1591083556,   0.1459534913,    0.121602647,
    0.08962567151,  0.05457502976,  0.02119792439,-0.006360886153, -0.02521691658,
    -0.0341463089, -0.03366438672, -0.02578292973, -0.01350942533,-0.0001993334008,
    0.01110421401,  0.01821358502,  0.02012399584,  0.01707590371,  0.01036039367,
   0.001927779871,-0.006103678606, -0.01192791108, -0.01441918314, -0.01331851631,
  -0.009212699719,-0.003327071201,  0.00280758366, 0.007730200887,  0.01037941873,
    0.01030727103, 0.007736497093, 0.003459413303,-0.001387581113
};
*/

//static float32_t testoutput[SAMPLE_INPUT_LENGTH];
static float32_t firStateBuff[FIR_LENGTH+SAMPLE_INPUT_LENGTH-1];
static arm_fir_instance_f32 S_45Hz;

//static float32_t firStateBuff2[FIR_Notch_LENGTH+SAMPLE_INPUT_LENGTH-1];
static arm_fir_instance_f32 S_Notch;



/* Functions */


void Filter_Init( void )
{
    arm_fir_init_f32(&S_45Hz, FIR_LENGTH, (float32_t*)&FIR_CoffArray[0], &firStateBuff[0], SAMPLE_INPUT_LENGTH);
}


void LowPass_Filter( float32_t* input, float32_t *output)
{
    arm_fir_f32(&S_45Hz, input, output, SAMPLE_INPUT_LENGTH);
}

void Notching_Filter( float32_t* input, float32_t *output )
{
    arm_fir_f32(&S_Notch, input, output, SAMPLE_INPUT_LENGTH);
}


int32_t DualBound( const uint32_t limit, int32_t data )
{
    if( data > limit)
    {
        return limit;
    }
    else if( data < -limit )
    {
        return -limit;
    }
    else
    {
        return data;
    }
}

int32_t Slipper_Ave( int32_t data )
{
    static uint8_t win_now = 1;
    static uint8_t win_max = WIN_LENGTH;
    static int32_t win_array[WIN_LENGTH+1];
    static uint8_t ptr = 0;
    int32_t result = 0;

    win_array[ptr] = data;
    int i = 0;
    for(i = 0;i < win_now;i++)
    {
        result += win_array[i];
    }
    result = result/win_now;
    if( win_now < win_max )
    {
        win_now++;
    }
    ptr++;
    if( ptr >= win_max )
    {
        ptr = 0;
    }
    return result;
}


/*
 * @From : https://www.sciencedirect.com/science/article/pii/S1746809415001032
 * @Author : David Castells-Rufas Jordi Carrabina
 */
int32_t MaMeMi( int32_t data )
{
    int32_t result;
    static int32_t max_now;
    static int32_t min_now;
    if(first_signal)
    {
        max_now = data;
        min_now = data;
        first_signal = 0;
    }
    else
    {
        if(data > max_now)
        {
            max_now = max_now + delta * cap_delta;
        }
        else
        {
            max_now = max_now - cap_delta;
        }

        if(data < min_now)
        {
            min_now = min_now - delta * cap_delta;
        }
        else
        {
            min_now = min_now + cap_delta;
        }
    }
    result = data - (max_now + min_now)/2;
    return result;
}
