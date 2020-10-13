/*
 * MPU6050.c
 *
 *  Created on: 2020年10月10日
 *      Author: HP
 */
#include <stdint.h>
#include "MPU6050.h"
filter_avg_t data_pack;
peak_value_t peak_VALUE;
slid_reg_t filter;
axis_info_t step_sample;
extern char step_cnt;
extern uint16_t mpu_times;

//读取xyz数据存入均值滤波器，存满进行计算，滤波后样本存入sample,将四次读取后处理的平均值视为一个样本
/*
static void filter_calculate(filter_avg_t *filter, axis_info_t *sample)
{
    unsigned int i;
    short x_sum = 0, y_sum = 0, z_sum = 0;
    for (i = 0; i < FILTER_CNT; i++) {
        x_sum += filter->info[i].x;
        y_sum += filter->info[i].y;
        z_sum += filter->info[i].z;
    }
    sample->x = x_sum / FILTER_CNT;
    sample->y = y_sum / FILTER_CNT;
    sample->z = z_sum / FILTER_CNT;
}*/

axis_info_t MPU_Slipper_Ave( axis_info_t data )
{
    static uint8_t win_now = 1;
    static uint8_t win_max = 10;
    static axis_info_t win_array[11];
    static uint8_t ptr = 0;
    axis_info_t result = {0,0,0};

    win_array[ptr].x = data.x;
    win_array[ptr].y = data.y;
    win_array[ptr].z = data.z;
    int i = 0;
    for(i = 0;i < win_now;i++)
    {
        result.x += win_array[i].x;
        result.y += win_array[i].y;
        result.z += win_array[i].z;
    }
    result.x = result.x/win_now;
    result.y = result.y/win_now;
    result.z = result.z/win_now;
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


//在动态阈值结构体初始化时，一定要将max的值都赋值为最小值，min赋值为最大值，这样才有利于动态更新。
static void peak_value_init(peak_value_t *peak)
{
    peak->newmax.x = -32768;
    peak->newmax.y = -32768;
    peak->newmax.z = -32768;

    peak->newmin.x = 32767;
    peak->newmin.y = 32767;
    peak->newmin.z = 32767;
}

//更新峰值，根据现在的样本值对原先的峰值进行更新
static void peak_update(peak_value_t *peak, axis_info_t *cur_sample)
{
    static unsigned int sample_size = 0;
    sample_size ++;
    if (sample_size > SAMPLE_SIZE) {
        /*采样达到50个，更新一次旧值*/
        sample_size = 1;
        peak->oldmax = peak->newmax;
        peak->oldmin = peak->newmin;
        //初始化
        peak_value_init(peak);
    }
    peak->newmax.x = MAX(peak->newmax.x, cur_sample->x);
    peak->newmax.y = MAX(peak->newmax.y, cur_sample->y);
    peak->newmax.z = MAX(peak->newmax.z, cur_sample->z);

    peak->newmin.x = MIN(peak->newmin.x, cur_sample->x);
    peak->newmin.y = MIN(peak->newmin.y, cur_sample->y);
    peak->newmin.z = MIN(peak->newmin.z, cur_sample->z);
}

//更新滤波器中的两个新旧样本，以此来确定是否样本变化幅度剧烈（0为不剧烈，1为剧烈）
static char slid_update(slid_reg_t *slid, axis_info_t *cur_sample)
{
    char res = 0;
    if (ABS((cur_sample->x - slid->new_sample.x)) > DYNAMIC_PRECISION) {
        slid->old_sample.x = slid->new_sample.x;
        slid->new_sample.x = cur_sample->x;
        res = 1;
    } else {
        slid->old_sample.x = slid->new_sample.x;
    }
    if (ABS((cur_sample->y - slid->new_sample.y)) > DYNAMIC_PRECISION) {
        slid->old_sample.y = slid->new_sample.y;
        slid->new_sample.y = cur_sample->y;
        res = 1;
    } else {
        slid->old_sample.y = slid->new_sample.y;
    }

    if (ABS((cur_sample->z - slid->new_sample.z)) > DYNAMIC_PRECISION) {
        slid->old_sample.z = slid->new_sample.z;
        slid->new_sample.z = cur_sample->z;
        res = 1;
    } else {
        slid->old_sample.z = slid->new_sample.z;
    }
    return res;
}
//对slid滤波器里面的新旧两个样本进行第一次初始化
void  slid_Init(slid_reg_t *slid)
{

        slid->old_sample.x = -32768;
        slid->old_sample.y = -32768;
        slid->old_sample.z = -32768;
        slid->new_sample=slid->old_sample;
}





/*判断当前最活跃轴*/
static char is_most_active(peak_value_t *peak)
{
    char res = MOST_ACTIVE_NULL;
    short x_change = ABS((peak->newmax.x - peak->newmin.x));
    short y_change = ABS((peak->newmax.y - peak->newmin.y));
    short z_change = ABS((peak->newmax.z - peak->newmin.z));

    if (x_change > y_change && x_change > z_change && x_change >= ACTIVE_PRECISION) {
        res = MOST_ACTIVE_X;
    } else if (y_change > x_change && y_change > z_change && y_change >= ACTIVE_PRECISION) {
        res = MOST_ACTIVE_Y;
    } else if (z_change > x_change && z_change > y_change && z_change >= ACTIVE_PRECISION) {
        res = MOST_ACTIVE_Z;
    }
    return res;
}

/*判断是否走步*/
static void detect_step(peak_value_t *peak, slid_reg_t *slid)
{
    char res = is_most_active(peak);
    switch (res) {
        case MOST_ACTIVE_NULL: {
            break;
        }
        case MOST_ACTIVE_X: {
            short threshold_x = (peak->oldmax.x + peak->oldmin.x) / 2;
            if (slid->old_sample.x > threshold_x && slid->new_sample.x < threshold_x) {
                mpu_times = 0;
                step_cnt ++;
            }
            break;
        }
        case MOST_ACTIVE_Y: {
            short threshold_y = (peak->oldmax.y + peak->oldmin.y) / 2;
            if (slid->old_sample.y > threshold_y && slid->new_sample.y < threshold_y) {
                mpu_times = 0;
                step_cnt ++;
            }
            break;
        }
        case MOST_ACTIVE_Z: {
            short threshold_z = (peak->oldmax.z + peak->oldmin.z) / 2;
            if (slid->old_sample.z > threshold_z && slid->new_sample.z < threshold_z) {
                    mpu_times = 0;
                    step_cnt ++;
            }
            break;
        }
        default:
            break;
    }
}

//对算法的各个量进行初始化
void  MPU6050_Init(void)
{
  peak_value_init(&peak_VALUE);
  slid_Init(&filter);

}
//采集完成后只在主程序中调用这一个函数即可
void check_step(void)
{
    //filter_calculate(&data_pack, &step_sample);
    peak_update(&peak_VALUE, &step_sample);
    if(slid_update(&filter, &step_sample))
        if(mpu_times>400)
        {
            detect_step(&peak_VALUE, &filter);
        }
}
