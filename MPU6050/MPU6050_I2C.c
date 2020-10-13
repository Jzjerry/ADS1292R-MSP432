/*
 *  ======= MPU6050_I2C ========
 *  MPU6050_I2C target-side implementation
 *
 *  Created on: 2020��10��10��
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
//�Ĵ�����ַ
#define AX 0x34



//Define MPU6050�Ľ����Լ��������
uint8_t MPU6050_IN_DATA[30];
uint8_t MPU6050_WRITE_DATA[3];


// Define address for an index of an I2C bus
const uint8_t MPU6050_ADDR = 0x68;
static I2C_Params params1;
static I2C_Handle i2cHandle;
static I2C_Transaction transaction;
bool well_trans;
//������������ٶ��Լ��Ƕ�
short a[3];
extern Display_Handle handle;
extern Display_Params params;
uint8_t high_data;
uint8_t low_data;
uint16_t mpu_times = 0;


//��ʽת��
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

    //��Դ����Ĵ���(�޸ĳɹ���
    MPU6050_WRITE_DATA[0] = 0X6B;
    MPU6050_WRITE_DATA[1] = 0X80;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;


    usleep(100000);

    MPU6050_WRITE_DATA[0] = 0X6B;
    MPU6050_WRITE_DATA[1] = 0X00;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;

    //���������üĴ���(�������������üĴ���д��0x10������֪�������Լ죬������Ϊ��1000��/s)
    MPU6050_WRITE_DATA[0] = 0X1B;
    MPU6050_WRITE_DATA[1] = 0x10;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;

    //���ü��ٶȼ����üĴ�����0X1C��(�޸ĳɹ�+-4g)
    MPU6050_WRITE_DATA[0] = 0X1C;
    MPU6050_WRITE_DATA[1] = 0x09;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;

    //���ò���Ƶ�ʷ�Ƶ����0X19��(�޸ĳɹ�)
    MPU6050_WRITE_DATA[0] = 0X19;
    MPU6050_WRITE_DATA[1] = 0x07;
    well_trans=I2C_transfer(i2cHandle, &transaction);
    well_trans=0;

    //���ò������ֵ�ͨ�˲���Ƶ�ʷ�Ƶ����0X1A��(�޸ĳɹ�)
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


