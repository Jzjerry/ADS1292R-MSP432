/*
 *  ======= MPU6050_I2C ========
 *  MPU6050_I2C target-side implementation
 *
 *  Created on: 2020年10月10日
 *  Author:     HP
 */
#include "MPU6050_I2C.h"
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>


// Define name for an index of an I2C bus
#define CONFIG_I2C_6050 0
//寄存器地址
#define AX 0x34



//Define MPU6050的接收以及输出数组
uint8_t MPU6050_IN_DATA[30];
uint8_t MPU6050_WRITE_DATA[3];


// Define address for an index of an I2C bus
const uint8_t MPU6050_ADDR = 0x68;
static I2C_Params params1;
static I2C_Handle i2cHandle;
static I2C_Transaction transaction;
bool well_trans;
//输出的三个加速度以及角度
short a[3];
extern Display_Handle handle;
extern Display_Params params;
uint8_t high_data;
uint8_t low_data;
uint16_t mpu_times = 0;


//格式转换
short CharToShort(uint8_t  high,uint8_t  low)
{
    return ((short)high<<8)|low;
}


void MPU6050_I2C_INIT(void)
{
    I2C_Params_init(&params1);
    params1.bitRate = I2C_400kHz;
    transaction.slaveAddress = MPU6050_ADDR;
    // Read from I2C slave device
    transaction.writeBuf = MPU6050_WRITE_DATA;
    transaction.writeCount = 2;
    transaction.readBuf = MPU6050_IN_DATA;
    transaction.readCount = 0;
    i2cHandle= I2C_open(CONFIG_I2C_6050, &params1);

    //电源管理寄存器(修改成功）
    MPU6050_WRITE_DATA[0] = 0X6B;
    MPU6050_WRITE_DATA[1] = 0X80;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;


    usleep(100000);

    MPU6050_WRITE_DATA[0] = 0X6B;
    MPU6050_WRITE_DATA[1] = 0X00;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;

    //陀螺仪配置寄存器(对于陀螺仪配置寄存器写入0x10，查表可知不触发自检，满量程为±1000°/s)
    MPU6050_WRITE_DATA[0] = 0X1B;
    MPU6050_WRITE_DATA[1] = 0x10;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;

    //设置加速度计配置寄存器（0X1C）(修改成功+-4g)
    MPU6050_WRITE_DATA[0] = 0X1C;
    MPU6050_WRITE_DATA[1] = 0x09;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;

    //设置采样频率分频器（0X19）(修改成功)
    MPU6050_WRITE_DATA[0] = 0X19;
    MPU6050_WRITE_DATA[1] = 0x07;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;

    //设置采样数字低通滤波器频率分频器（0X1A）(修改成功)
      MPU6050_WRITE_DATA[0] = 0X1A;
      MPU6050_WRITE_DATA[1] = 0x06;
      well_trans=I2C_transfer(i2cHandle, &transaction);
      well_trans=0;




    I2C_close(i2cHandle);

}


void MSP_MPU6050_I2C_read_data(void)
{
     transaction.writeBuf = MPU6050_WRITE_DATA;
     transaction.writeCount = 1;
     transaction.readBuf = MPU6050_IN_DATA;
     transaction.readCount = 1;


    MPU6050_WRITE_DATA[0] = 0X3B;
    i2cHandle= I2C_open(CONFIG_I2C_6050, &params1);
    well_trans=I2C_transfer(i2cHandle, &transaction);
    if(well_trans)
        mpu_times++;
        high_data=MPU6050_IN_DATA[0];
    well_trans=0;



    MPU6050_WRITE_DATA[0] = 0X3C;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    if(well_trans)
        low_data=MPU6050_IN_DATA[0];
    well_trans=0;
    a[0]=CharToShort(high_data,low_data);



    MPU6050_WRITE_DATA[0] = 0X3D;
     well_trans=I2C_transfer(i2cHandle, &transaction);
     if(well_trans)
         high_data=MPU6050_IN_DATA[0];
     well_trans=0;

     MPU6050_WRITE_DATA[0] = 0X3E;
     well_trans=I2C_transfer(i2cHandle, &transaction);
     if(well_trans)
         low_data=MPU6050_IN_DATA[0];
     well_trans=0;
     a[1]=CharToShort(high_data,low_data);



     MPU6050_WRITE_DATA[0] = 0X3F;
      well_trans=I2C_transfer(i2cHandle, &transaction);
      if(well_trans)
          high_data=MPU6050_IN_DATA[0];
      well_trans=0;

      MPU6050_WRITE_DATA[0] = 0X40;
      well_trans=I2C_transfer(i2cHandle, &transaction);
      if(well_trans)
          low_data=MPU6050_IN_DATA[0];
      well_trans=0;
      a[2]=CharToShort(high_data,low_data);




    I2C_close(i2cHandle);
}


