/*
 * LMT70_I2C.c
 *
 *  Created on: 2020��10��2��
 *      Author: HUAWEI
 */

#include "LMT70_I2C.h"
#include <ti/drivers/I2C.h>

// Define name for an index of an I2C bus
#define LMT70 0
// Define address for an index of an I2C bus
#define VOL2TEMP_TABLE_SIZE 21

//Define LMT70_IN dataDATA
uint8_t LMT70_IN_DATA[4];
uint8_t LMT70_WRITE_DATA[1];


const uint8_t LMT70_ADDR = 0x68;
//static I2C_Params params;
//static I2C_Handle i2cHandle;
//static I2C_Transaction transaction;
bool well_trans;



/*��ѹ�Ƚϱ�*/
float v2t_tabl[VOL2TEMP_TABLE_SIZE] = {
                                1350.441,1300.593,1250.398,1199.884,1149.070,
                                1097.987,1046.647, 995.050, 943.227, 891.178,
                                 838.882, 786.360, 733.608, 680.654, 627.490,
                                 574.117, 520.551, 466.760, 412.739, 358.164,
                                 302.785
};

/* 10��C��Χ�ڵĲ�ֵ���� */
float slope_tabl[VOL2TEMP_TABLE_SIZE] = {
                                4.985,5.020,5.051,5.081,5.108,
                                5.134,5.160,5.182,5.205,5.230,
                                5.252,5.275,5.295,5.316,5.337,
                                5.357,5.379,5.402,5.458,5.538,
                                5.538
};

/*
void LMT70_I2C_INIT(void)
{
    I2C_Params_init(&params);
    params.bitRate = I2C_400kHz;
    transaction.slaveAddress = LMT70_ADDR;
    // Read from I2C slave device
    transaction.writeBuf = LMT70_WRITE_DATA;
    // transaction.writeCount = 0;
    transaction.writeCount = 1;
    transaction.readBuf = LMT70_IN_DATA;
    transaction.readCount = 4;
}
*/

/*
float MSP_LMT70_I2C_read_data(void)
{
    i2cHandle= I2C_open(CONFIG_I2C_LMT70, &params);
    int32_t ADC_Read = 0;

   LMT70_WRITE_DATA[0] = 0x80;

    well_trans=I2C_transfer(i2cHandle, &transaction);
    if(well_trans)
    {
        ADC_Read = ((int32_t)(LMT70_IN_DATA[1])<<8) | (LMT70_IN_DATA[2]);
        ADC_Read &= 0x00000FFF;
    }
    I2C_close(i2cHandle);
    return (float)ADC_Read;
}
*/

/* �۰�����㷨 */
/* upΪv2t_tabl���е��±�*/
static int16_t haltserach( float vol)
{
    int16_t low = 0 ;
    int16_t up = VOL2TEMP_TABLE_SIZE ;
    int16_t mid ;

    while ( up >= low)
    {
        mid = ( low + up) >> 1 ;

        if ( v2t_tabl[mid] < vol)
        {
            up = mid - 1 ;
        }
        else if ( vol < v2t_tabl[mid])
        {
            low = mid + 1 ;
        }
        else
        {
            return mid ;
        }
    }
    return up ;
}

/** @brief  �����ѹ��Ӧ���¶�ֵ
  * @param[in]  vol lmt70�����ĵ�ѹֵ
  * @retval �¶�ֵ��С�����3λ����43.250����Ӧ��Ϊ43250
  */
int32_t lmt70_get_temp(float vol)
{
    int16_t i ;
    int32_t rev = 0 ;
    i = haltserach(vol);
    if((i!=-1)&&(i!=VOL2TEMP_TABLE_SIZE))
    //if (( i != -1) && ( i != VOL2TEMP_TABLE_SIZE))
    {
        rev = ( ( ( i * 10) - 50) + ( ( v2t_tabl[i] - vol) / slope_tabl[i])) * 100 ;
    }

    return rev ;
}




