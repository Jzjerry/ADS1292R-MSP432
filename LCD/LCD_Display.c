/*
 * LCD_Display.c
 *
 *  Created on: 2020Äê10ÔÂ7ÈÕ
 *      Author: jzjerry
 *      Transplant from Adafruit_ILI9341 Library
 */


#include "LCD_Display.h"

#include "ti_drivers_config.h"
#include "global.h"

#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

#include <driverlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stddef.h>


#define MADCTL_MY 0x80  ///< Bottom to top
#define MADCTL_MX 0x40  ///< Right to left
#define MADCTL_MV 0x20  ///< Reverse Mode
#define MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH 0x04  ///< LCD refresh right to left


/* Variables */
static const uint8_t initcmd[] = {
  0xEF, 3, 0x03, 0x80, 0x02,
  0xCF, 3, 0x00, 0xC1, 0x30,
  0xED, 4, 0x64, 0x03, 0x12, 0x81,
  0xE8, 3, 0x85, 0x00, 0x78,
  0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  0xF7, 1, 0x20,
  0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  ILI9341_DISPON  , 0x80,                // Display on
  0x00                                   // End of list
};

int _width;
int _height;


static SPI_Handle LCD_Handle;
static SPI_Transaction LCDTransaction;
int transferOK;

uint8_t masterRxBuffer[30];
uint8_t masterTxBuffer[10];


void send_Command( uint8_t cmd, uint8_t* addr, int numArgs )
{
    if(LCD_Handle)
    {
        GPIO_write(CONFIG_GPIO_LCD_DC, 0);
        LCDTransaction.txBuf = (uint8_t*)&cmd;
        LCDTransaction.count = 1;
        if(!SPI_transfer(LCD_Handle, &LCDTransaction))
        {
            while(1);
        }
        GPIO_write(CONFIG_GPIO_LCD_DC, 1);
        LCDTransaction.txBuf = (uint8_t*)addr;
        LCDTransaction.count = numArgs;
        if(!SPI_transfer(LCD_Handle, &LCDTransaction))
        {
            while(1);
        }
    }
}

static void LCD_Write8cmd(uint8_t byte) {
  // Command has DC low and CS low while writing to SPI bus.
  GPIO_write( CONFIG_GPIO_LCD_DC, 0 );
  if(LCD_Handle)
  {
      LCDTransaction.txBuf = (uint8_t*)&byte;
      LCDTransaction.count = 1;
      SPI_transfer(LCD_Handle, &LCDTransaction);
  }
}


static void LCD_Write8(uint8_t byte) {
  // Data has DC high and CS low while writing to SPI bus.
    GPIO_write( CONFIG_GPIO_LCD_DC, 1 );
    if(LCD_Handle)
    {
        LCDTransaction.txBuf = (uint8_t*)&byte;
        LCDTransaction.count = 1;
        SPI_transfer(LCD_Handle, &LCDTransaction);
    }
}

static void LCD_Write16(uint16_t data)
{
    uint8_t tx[2];
    tx[0] = data>>8;
    tx[1] = data & 0xFF;
    if(LCD_Handle)
    {
        LCDTransaction.txBuf = (uint8_t*)tx;
        LCDTransaction.count = 2;
        SPI_transfer(LCD_Handle, &LCDTransaction);
    }
}


void LCD_SPI_init()
{
    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.transferMode  = SPI_MODE_BLOCKING;
    spiParams.dataSize      = 8;

    spiParams.frameFormat   = SPI_POL0_PHA1;
    spiParams.bitRate       = 24000000;

    LCDTransaction.rxBuf = masterRxBuffer;

    LCD_Handle = SPI_open( CONFIG_SPI_LCD , &spiParams );
    if( LCD_Handle == NULL)
    {
        while(1);//Fail in SPI Opening
    }

    send_Command(ILI9341_SWRESET, 0, 0);
    usleep(150000);
    uint8_t cmd, x, numArgs;
    const uint8_t *addr = initcmd;
    while( (cmd = *(addr++))>0 )
    {
        x = *(addr++);
        numArgs = x & 0x7F;
        send_Command( cmd, (uint8_t*)addr, numArgs );
        if(x & 0x80)
        {
            usleep(150000);
        }
    }
}

void setRotation(uint8_t m)
{
    uint8_t rotation = m % 4;
    switch (rotation) {
    case 0:
      m = (MADCTL_MX | MADCTL_BGR);
      _width = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 1:
      m = (MADCTL_MV | MADCTL_BGR);
      _width = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
    case 2:
      m = (MADCTL_MY | MADCTL_BGR);
      _width = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 3:
      m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      _width = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
    }
    send_Command(ILI9341_MADCTL, &m, 1);
}


void invertDisplay(short invert)
{
    send_Command(invert ? ILI9341_INVON : ILI9341_INVOFF, 0, 0);
}

void scrollTo(uint16_t y)
{
    uint8_t data[2];
    data[0] = y >> 8;
    data[1] = y & 0xff;
    send_Command(ILI9341_VSCRSADD, (uint8_t *)data, 2);
}

void setScrollMargins(uint16_t top, uint16_t bottom)
{
    if (top + bottom <= ILI9341_TFTHEIGHT)
    {
      uint16_t middle = ILI9341_TFTHEIGHT - top + bottom;
      uint8_t data[6];
      data[0] = top >> 8;
      data[1] = top & 0xff;
      data[2] = middle >> 8;
      data[3] = middle & 0xff;
      data[4] = bottom >> 8;
      data[5] = bottom & 0xff;
      send_Command(ILI9341_VSCRDEF, (uint8_t *)data, 6);
    }
}

void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{
    uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
    LCD_Write8cmd(ILI9341_CASET); // Column address set
    LCD_Write16(x1);
    LCD_Write16(x2);
    LCD_Write8cmd(ILI9341_PASET); // Row address set
    LCD_Write16(y1);
    LCD_Write16(y2);
    LCD_Write8cmd(ILI9341_RAMWR); // Write to RAM
}

uint8_t readcommand8(uint8_t commandByte, uint8_t index)
{
  uint8_t data = 0x10 + index;
  send_Command(0xD9, &data, 1); // Set Index Register
  return masterRxBuffer[0];
}
