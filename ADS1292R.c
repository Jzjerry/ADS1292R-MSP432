/*
 * ASD1292R.C
 *
 *  Created on: 2020Äê10ÔÂ1ÈÕ
 *      Author: HUAWEI
 */

#include <ADS1292R.h>

uint8_t const ADS_REG[][2] =
{

 {0x00,0x73},  //ID
 {0x01,0x02},  //CONFIG1 //500 SPS
 //{0x01,0x03},  //CONFIG1 //1k SPS
// {0x01,0x04},  //CONFIG1 //2k SPS
// {0x01,0x05},  //CONFIG1 //4k SPS
 {0x02,0xE8},  //CONFIG2
 {0x03,0xF0},  //LOFF
 {0x04,0x00},  //CH1SET
 {0x05,0x00},  //CH2SET
 {0x06,0xAC},  //RLD_SENS
 {0x07,0x0F},  //LOFF_SENS
 {0x08,0x00},  //LOFF_STAT
 {0x09,0xEA},  //RESP1
 {0x0A,0x07},  //RESP2

 {0xFF,0xFF}   //End of Array

};

int DELAY_COUNT=3;

int32_t channel_1[DEBUG_BUFFER_LENGTH];
int32_t channel_2[DEBUG_BUFFER_LENGTH];
//uint32_t status[1000];

uint8_t ADS_REG_READ[12];

int32_t read_data[3];
uint32_t p2_24=0;

/*
void ADS_main( void )
{
    p2_24 = pow(2,24);

    int i=0;

    ADS_PowerOn_Init();

    int delay=0;
    for ( delay =0; delay < 10000; delay++);
    for ( delay =0; delay < 10000; delay++);
    for ( delay =0; delay < 10000; delay++);

    //Start_Read_Data_Continuous();
    MSP_ADS_SPI_write_command(RDATAC);
    for ( delay =0; delay < 10000; delay++);
    for ( delay =0; delay < 10000; delay++);
    for ( delay =0; delay < 10000; delay++);



    ADS_read_done_flag = 1;
    while(1)
    {
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
        channel_2[i] = *(status_channel_data + 2);
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
}
*/

void ADS1292_GInit( void )
{
    p2_24 = pow(2,24);

    ADS_PowerOn_Init();

    int delay=0;
    for ( delay =0; delay < 10000; delay++);
    for ( delay =0; delay < 10000; delay++);
    for ( delay =0; delay < 10000; delay++);

    //Start_Read_Data_Continuous();
    MSP_ADS_SPI_write_command(RDATAC);
    for ( delay =0; delay < 10000; delay++);
    for ( delay =0; delay < 10000; delay++);
    for ( delay =0; delay < 10000; delay++);
}

void ADS_PowerOn_Init(void)
{

   volatile unsigned short Init_i, j;

    for (j = 0; j < DELAY_COUNT; j++)
    {
        for ( Init_i =0; Init_i < 20000; Init_i++);
        for ( Init_i =0; Init_i < 20000; Init_i++);
        for ( Init_i =0; Init_i < 20000; Init_i++);
    }

   for ( Init_i =0; Init_i < 20000; Init_i++);
   for ( Init_i =0; Init_i < 20000; Init_i++);
   for ( Init_i =0; Init_i < 20000; Init_i++);

   //ADS_start_conv_command();
   MSP_ADS_SPI_write_command(START);

   //ADS_soft_stop();
   MSP_ADS_SPI_write_command(STOP);


   for (j = 0; j < DELAY_COUNT; j++)
    {
        for ( Init_i =0; Init_i < 20000; Init_i++);
    }
   //ADS_stop_read_continuous();                 // SDATAC command
   MSP_ADS_SPI_write_command(SDATAC);
   for (j = 0; j < DELAY_COUNT; j++)
    {
       for ( Init_i =0; Init_i < 35000; Init_i++);
    }
    for (j = 0; j < DELAY_COUNT; j++)
    {
       for ( Init_i =0; Init_i < 35000; Init_i++);
    }

    ADS_write_register_configs();

    int reg = 0;

    for(reg=0;reg<12;reg++)
    {
        ADS_REG_READ[reg] = ADS_register_read(reg);
    }

}


void ADS_write_register_configs(void)
{
    int i=1;

    while(ADS_REG[i][0] != 0xFF)
    {
        MSP_ADS_SPI_write_register(ADS_REG[i][0], ADS_REG[i][1]);
        i++;
    }
}

unsigned int ADS_register_read(uint8_t reg_address)
{
    uint8_t data=0;
    uint8_t return_value = 0;

    MSP_ADS_SPI_read_register(reg_address, &data);
    return_value = data;

    return return_value;
}

int32_t * ADS_read_ecg_resp_data(void)
{
    int i=0;

    uint32_t data, data_1, data_2;

    MSP_ADS_SPI_read_ecg_resp_data(&data, &data_1, &data_2);

    read_data[0] = data;
    read_data[1] = data_1;
    read_data[2] = data_2;

    for(i=1;i<3;i++)
    {
       read_data[i] &= 0x00FFFFFF;
       if (read_data[i] >= p2_24/2 /*(2^23)*/ )
       {
           read_data[i] -= p2_24;
       }
    }

    return read_data;
}

void ADS_CS_low(void)
{
    GPIO_write(CONFIG_ADS1292_CS, 0);
}

void ADS_CS_high(void)
{
    GPIO_write(CONFIG_ADS1292_CS, 1);
}

