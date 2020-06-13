/*
 * Nokia.c
 *
 *  Created on: 28 mai 2016
 *      Author: D01000100
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <wiringPi.h>
#include "Nokia.h"
#include "Graphics.h"

#define VMODE 0x22
#define HMODE 0x20
#define DISPLAY_CTL 0x0D
#define SETY 0x27
#define SETX 0x7f
#define PERIOD 0

#define Hbytes 84
#define Vbytes 6
#define COLS 12

uint8_t frameBuffer[Hbytes][Vbytes] = {{0}};

// SPI parameters
uint8_t bits = 8;
uint8_t mode = 0;
const int frequency = 1e6;
int spi0fd = 0;

// Pins
#define RST_pin 5
#define DC_pin 6

void pabort(char*s){
    perror(s);
    abort();
}

void lcdReset()
{
    digitalWrite(RST_pin, 0);
    usleep(100);
    digitalWrite(RST_pin, 1);
    usleep(100);
}

void lcdPrintFormatted(const char*str){
    int size = strlen(str);
    int l = COLS;
    for(int i=0; i<size; i++){
        if(str[i] != '\n'){
            lcdChar(str[i]);
            l--;
        }
        else{
            for(; l>0; l--)
                lcdChar(' ');
            l = COLS;
        }
        if(l<0)
            l = COLS;
    }
}

void lcdWriteBurst(bool DCval, uint8_t* data,int size){
    int ret;
    uint8_t rx[1024];
    struct spi_ioc_transfer tr = {
        .tx_buf = (int)data,
        .rx_buf = (int)rx,
        .len = size,
        .delay_usecs = 0,
        .speed_hz = frequency,
        .bits_per_word = bits,
    };
    digitalWrite(DC_pin, DCval);
    ret = ioctl(spi0fd, SPI_IOC_MESSAGE(1), &tr);
    if(ret < 1)
        pabort("Could not send message");
}

void lcdWrite(bool DCval, uint8_t data)
{
    lcdWriteBurst(DCval, &data, 1);
}

void lcdInit()
{
    int ret;
    // Init pins
    pinMode(RST_pin, OUTPUT);
    pinMode(DC_pin, OUTPUT);

    // Init SPI
    spi0fd = open("/dev/spidev0.0", O_WRONLY);
    if(spi0fd < 0)
        pabort("Could not open SPI dev");

    // SPI mode
    ret = ioctl(spi0fd, SPI_IOC_WR_MODE, &mode);
    if(ret == -1)
        pabort("Could not write SPI mode");

    // SPI bits per word
    ret = ioctl(spi0fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if(ret == -1)
        pabort("Could not write SPI bits per word");

    // SPI frequency
    ret = ioctl(spi0fd, SPI_IOC_WR_MAX_SPEED_HZ, &frequency);
    if(ret == -1)
        pabort("Could not write SPI frequency");

    lcdReset();

    lcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
    lcdWrite(LCD_C, 0xc8 );  // Set LCD Vop (Contrast).
    lcdWrite(LCD_C, 0x06 );  // Set Temp coefficent. //0x04
    lcdWrite(LCD_C, 0x13 );  // LCD bias mode 1:48. //0x13
    lcdWrite(LCD_C, 0x20 );  // LCD Basic Commands
    lcdWrite(LCD_C, 0x0C );  // LCD in normal mode.

    lcdClear();
}

void lcdChar(char character)
{
    if(character>=0x20 && character<=0x7e){
        lcdWrite(LCD_D, 0x00);
        for (int index = 0; index < 5; index++)
        {
            lcdWrite(LCD_D, ASCII[character - 0x20][index]);
        }
        lcdWrite(LCD_D, 0x00);
    }
    else if(isprint(character)){
        lcdWrite(LCD_D, 0x00);
        for (int index = 0; index < 5; index++)
        {
            lcdWrite(LCD_D, 0xff);
        }
        lcdWrite(LCD_D, 0x00);
    }

}

void lcdPrint(const char *str){
    for(int i=0; str[i] != '\0'; i++){
        lcdChar(str[i]);
    }
}

void lcdClear(){
    for(int i=0;i<Hbytes*Vbytes;i++){
        lcdWrite(LCD_D, 0x00);
    }
}

void gr_updateScreen()
{
    lcdClear();
    for(int i=0;i<Vbytes;i++)
    {
        for(int j=0;j<Hbytes;j++)
        {
            lcdWrite(LCD_D, frameBuffer[j][i]); // This can be done in burst
        }
    }
}

void gr_drawImage(const uint8_t*image)
{
    int i=0;
    for(int h=0;h<Hbytes;h++)
    {
        for(int v=0;v<Vbytes;v++)
        {
            frameBuffer[h][v]=image[i++];
        }
    }
}

void gr_setPixel(int x, int y)
{
    if(x<=Hbytes-1 && y<=Vbytes*8-1)
    {
        frameBuffer[x][y/8] |= (0x01<<(y%8));
    }
}
