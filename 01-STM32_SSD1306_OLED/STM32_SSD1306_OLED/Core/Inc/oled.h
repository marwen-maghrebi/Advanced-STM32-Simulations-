/*
 * oled.h
 *
 *  Created on: Apr 9, 2024
 *      Author: PC-MAGHREBI
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * This SSD1306 LCD uses I2C for communication
 *
 * Library features functions for drawing lines, rectangles and circles.
 *
 * It also allows you to draw texts and characters using appropriate functions provided in library.
 *
 * Default pinout
 *
SSD1306    |STM32F411RE  |DESCRIPTION

VCC        |3.3V         |
GND        |GND          |
SCL        |PB8          |Serial clock line
SDA        |PB9          |Serial data line
 */

#include "main.h"                  // Device header
#include "fonts.h"
#include "stdlib.h"
#include "string.h"


/* I2C address */
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR         0x3C
//#define SSD1306_I2C_ADDR       0x7A
#endif

/* SSD1306 settings */
/* SSD1306 width in pixels */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH            128
#endif
/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT           64
#endif


typedef enum {
  SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
  SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;




uint8_t SSD1306_Init(void);


void SSD1306_UpdateScreen(void);


void SSD1306_ToggleInvert(void);


void SSD1306_Fill(SSD1306_COLOR_t Color);


void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);


void SSD1306_GotoXY(uint16_t x, uint16_t y);


char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);


char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);


void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c);


void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);

void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);


void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);


void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);


void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);


void ssd1306_I2C_Init();


void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);


void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, char *data, uint16_t count);


void SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);



void SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row);


void SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row);


void SSD1306_Scrolldiagright(uint8_t start_row, uint8_t end_row);


void SSD1306_Scrolldiagleft(uint8_t start_row, uint8_t end_row);



void SSD1306_Stopscroll(void);


// inverts the display i = 1->inverted, i = 0->normal

void SSD1306_InvertDisplay (int i);


// clear the display

void SSD1306_Clear (void);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_OLED_H_ */
