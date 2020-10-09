/*
 * ASD1292R_SPI.c
 *
 *  Created on: 2020Äê10ÔÂ1ÈÕ
 *      Author: HUAWEI
 */



#include <ADS1292R_SPI.h>
uint8_t masterRxBuffer[10];
uint8_t masterTxBuffer[10];

uint32_t data;

SPI_Handle masterSpi;
SPI_Transaction masterTransaction;
UInt transferOK;



void ADS1292_SPI_init( )
{
    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.transferMode  = SPI_MODE_BLOCKING;
    spiParams.dataSize      = 8;

    spiParams.frameFormat   = SPI_POL0_PHA1;
    spiParams.bitRate       = 400000;//200000;


    masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);

    if (masterSpi == NULL)
    {
       System_abort("Error initializing SPI\n");
    }


    masterTransaction.count = 4;
    masterTransaction.txBuf = (Ptr)masterTxBuffer;
    masterTransaction.rxBuf = (Ptr)masterRxBuffer;

}


void ADS1292_SPI_shutdown()
{
    SPI_close(masterSpi);
}

void MSP_ADS_SPI_write_register(uint8_t reg_address, uint8_t data)
{
    int i=0;

    masterTransaction.count = 3;

    masterTxBuffer[0] = (WREG | reg_address);
    masterTxBuffer[1] = 0x00;
    masterTxBuffer[2] = (data);

    ADS_CS_low();

    for(i=0;i<50;i++);

    transferOK = SPI_transfer(masterSpi, &masterTransaction);
/*
    for(i=0;i<50;i++);

    masterTxBuffer[0] = (WREG | reg_address);

    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    for(i=0;i<500;i++);

    masterTxBuffer[0] = 0x00;

    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    for(i=0;i<500;i++);

    masterTxBuffer[0] = data;

    transferOK = SPI_transfer(masterSpi, &masterTransaction);
*/
    for(i=0;i<200;i++);

    ADS_CS_high();

    for(i=0;i<200;i++);

    if(transferOK)
    {
        System_printf("Master: %s\n", masterTxBuffer);
    }
    else
    {
        System_printf("Unsuccessful master SPI transfer");
    }
}


void MSP_ADS_SPI_read_register(uint8_t reg_address, uint8_t* rx_buffer)
{
    int i=0;

    masterTransaction.count = 3;

    masterTxBuffer[0] = (RREG | reg_address);
    masterTxBuffer[1] = 0x00;
    masterTxBuffer[2] = 0x00;

    ADS_CS_low();

    for(i=0;i<50;i++);

    transferOK = SPI_transfer(masterSpi, &masterTransaction);
/*
    masterTxBuffer[0] = (RREG | reg_address);

    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    for(i=0;i<500;i++);

    masterTxBuffer[0] = 0x00;

    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    for(i=0;i<500;i++);

    masterTxBuffer[0] = 0x00;

    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    for(i=0;i<1000;i++);
*/
    for(i=0;i<200;i++);

    ADS_CS_high();

    for(i=0;i<200;i++);

    if(transferOK)
    {
       //Print contents of master receive buffer
       System_printf("Master read: %s\n", masterRxBuffer);
       *rx_buffer = masterRxBuffer[2];
    }
    else
    {
       System_printf("Unsuccessful master SPI read");
       *rx_buffer = 0;
    }
}


void MSP_ADS_SPI_write_command(uint8_t command)
{

    int i=0;

    masterTransaction.count = 1;

    masterTxBuffer[0] = (command);

    ADS_CS_low();

    for(i=0;i<500;i++);

    //Initiate SPI transfer
    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    for(i=0;i<200;i++);

    ADS_CS_high();

    for(i=0;i<200;i++);

    if(transferOK)
    {
        //System_printf("Master: %s\n", masterTxBuffer);
    }
    else
    {
        System_printf("Unsuccessful master SPI transfer");
    }

}

void MSP_ADS_SPI_read_ecg_resp_data(uint32_t* status, uint32_t* channel_1, uint32_t* channel_2)
{

    int i=0;

    masterTransaction.count = 9;

    masterTxBuffer[0] = 0x00;
    masterTxBuffer[1] = 0x00;
    masterTxBuffer[2] = 0x00;
    masterTxBuffer[3] = 0x00;
    masterTxBuffer[4] = 0x00;
    masterTxBuffer[5] = 0x00;
    masterTxBuffer[6] = 0x00;
    masterTxBuffer[7] = 0x00;
    masterTxBuffer[8] = 0x00;

    ADS_CS_low();

    for(i=0;i<25;i++);

    //Initiate SPI transfer
    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    for(i=0;i<30;i++);

    ADS_CS_high();

    for(i=0;i<30;i++);

    if(transferOK)
    {
       /* Print contents of master receive buffer */
       //System_printf("Master read: %s\n", masterRxBuffer);
       *status    = ((0x00 << 24) | (masterRxBuffer[0] << 16) | (masterRxBuffer[1] << 8) | (masterRxBuffer[2]));
       *channel_1 = ((0x00 << 24) | (masterRxBuffer[3] << 16) | (masterRxBuffer[4] << 8) | (masterRxBuffer[5]));
       *channel_2 = ((0x00 << 24) | (masterRxBuffer[6] << 16) | (masterRxBuffer[7] << 8) | (masterRxBuffer[8]));

    }
    else
    {
       System_printf("Unsuccessful master SPI read");
       *status = 0;
       *channel_1 = 0;
       *channel_2 = 0;
    }

}
