/*
 * Nokia.h
 *
 *  Created on: 28 mai 2016
 *      Author: D01000100
 */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef NOKIA_NOKIA_H_
#define NOKIA_NOKIA_H_

#include <stdint.h>
#include <stdbool.h>

#define LCD_C false
#define LCD_D true

#define nullptr 0


void lcdReset();

void lcdInit();

void lcdPrintFormatted(const char*str);

void lcdWrite(bool DCval, uint8_t data);

void lcdChar(char character);

void lcdPrint(const char* str);

void lcdClear();

void gr_setPixel(int x, int y);

void gr_updateScreen();

void gr_drawImage(const uint8_t*image);

#endif /* NOKIA_NOKIA_H_ */

#ifdef __cplusplus
}
#endif
