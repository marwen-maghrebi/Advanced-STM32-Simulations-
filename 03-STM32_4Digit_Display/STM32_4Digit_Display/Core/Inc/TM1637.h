/*
 * TM1637.h
 *
 *  Created on: Aug 25, 2024
 *      Author: pc1
 */

#ifndef INC_TM1637_H_
#define INC_TM1637_H_


#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

#define DEFAULT_BIT_DELAY  100

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

typedef struct {
	    GPIO_TypeDef* clk_port;
		uint16_t clk_pin;
		GPIO_TypeDef* dio_port;
		uint16_t dio_pin;
		uint8_t brightness;
		uint32_t bit_delay;
} TM1637Display;

void TM1637Display_init(TM1637Display* display, GPIO_TypeDef* clk_port, uint16_t clk_pin,
		                        GPIO_TypeDef* dio_port, uint16_t dio_pin, uint32_t bit_delay);
void TM1637Display_setBrightness(TM1637Display* display, uint8_t brightness, bool on);
void TM1637Display_setSegments(TM1637Display* display, const uint8_t segments[],
		                                uint8_t length, uint8_t pos);
void TM1637Display_clear(TM1637Display* display);
void TM1637Display_showNumberDec(TM1637Display* display, int num, bool leading_zero,
		                                uint8_t length, uint8_t pos);
void TM1637Display_showNumberDecEx(TM1637Display* display, int num, uint8_t dots,
		                                bool leading_zero, uint8_t length, uint8_t pos);
void TM1637Display_showNumberHexEx(TM1637Display* display, uint16_t num, uint8_t dots,
		                                bool leading_zero, uint8_t length, uint8_t pos);
uint8_t TM1637Display_encodeDigit(uint8_t digit);

// Private function prototypes
static void TM1637Display_bitDelay(TM1637Display* display);
static void TM1637Display_start(TM1637Display* display);
static void TM1637Display_stop(TM1637Display* display);
static bool TM1637Display_writeByte(TM1637Display* display, uint8_t b);
static void TM1637Display_showDots(uint8_t dots, uint8_t* digits);
static void TM1637Display_showNumberBaseEx(TM1637Display* display, int8_t base, uint16_t num,
		                                uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos);

#endif /* INC_TM1637_H_ */
