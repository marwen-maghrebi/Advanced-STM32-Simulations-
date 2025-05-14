
#include "TM1637.h"

static const uint8_t digitToSegment[] = {
		0b00111111,    // 0
		0b00000110,    // 1
		0b01011011,    // 2
		0b01001111,    // 3
		0b01100110,    // 4
		0b01101101,    // 5
		0b01111101,    // 6
		0b00000111,    // 7
		0b01111111,    // 8
		0b01101111,    // 9
		0b01110111,    // A
		0b01111100,    // b
		0b00111001,    // C
		0b01011110,    // d
		0b01111001,    // E
		0b01110001     // F
};

static const uint8_t minusSegments = 0b01000000;

void TM1637Display_init(TM1637Display* display, GPIO_TypeDef* clk_port, uint16_t clk_pin,
		                    GPIO_TypeDef* dio_port, uint16_t dio_pin, uint32_t bit_delay)
{
	display->clk_port = clk_port;
	display->clk_pin = clk_pin;
	display->dio_port = dio_port;
	display->dio_pin = dio_pin;
	display->bit_delay = bit_delay;

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = clk_pin;
	HAL_GPIO_Init(clk_port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = dio_pin;
	HAL_GPIO_Init(dio_port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(clk_port, clk_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dio_port, dio_pin, GPIO_PIN_RESET);
}

void TM1637Display_setBrightness(TM1637Display* display, uint8_t brightness, bool on)
{
	display->brightness = (brightness & 0x7) | (on ? 0x08 : 0x00);
}

void TM1637Display_setSegments(TM1637Display* display, const uint8_t segments[], uint8_t length, uint8_t pos)
{
	TM1637Display_start(display);
	TM1637Display_writeByte(display, TM1637_I2C_COMM1);
	TM1637Display_stop(display);

	TM1637Display_start(display);
	TM1637Display_writeByte(display, TM1637_I2C_COMM2 + (pos & 0x03));

	for (uint8_t k = 0; k < length; k++)
		    TM1637Display_writeByte(display, segments[k]);

	TM1637Display_stop(display);

	TM1637Display_start(display);
	TM1637Display_writeByte(display, TM1637_I2C_COMM3 + (display->brightness & 0x0f));
	TM1637Display_stop(display);
}

void TM1637Display_clear(TM1637Display* display)
{
	uint8_t data[] = { 0, 0, 0, 0 };
	TM1637Display_setSegments(display, data, 4, 0);
}

void TM1637Display_showNumberDec(TM1637Display* display, int num, bool leading_zero, uint8_t length, uint8_t pos)
{
	TM1637Display_showNumberDecEx(display, num, 0, leading_zero, length, pos);
}

void TM1637Display_showNumberDecEx(TM1637Display* display, int num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos)
{
	TM1637Display_showNumberBaseEx(display, num < 0 ? -10 : 10, num < 0 ? -num : num, dots, leading_zero, length, pos);
}

void TM1637Display_showNumberHexEx(TM1637Display* display, uint16_t num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos)
{
	TM1637Display_showNumberBaseEx(display, 16, num, dots, leading_zero, length, pos);
}

uint8_t TM1637Display_encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x0f];
}

static void TM1637Display_bitDelay(TM1637Display* display)
{
	HAL_Delay(display->bit_delay / 1000);
	if (display->bit_delay % 1000 > 0) {
		    for (volatile uint32_t i = 0; i < (display->bit_delay % 1000) * 48; i++) {
		    	        __NOP();
		    }
	}
}

static void TM1637Display_start(TM1637Display* display)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = display->dio_pin;
	HAL_GPIO_Init(display->dio_port, &GPIO_InitStruct);
	TM1637Display_bitDelay(display);
}

static void TM1637Display_stop(TM1637Display* display)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = display->dio_pin;
	HAL_GPIO_Init(display->dio_port, &GPIO_InitStruct);
	TM1637Display_bitDelay(display);

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin = display->clk_pin;
	HAL_GPIO_Init(display->clk_port, &GPIO_InitStruct);
	TM1637Display_bitDelay(display);

	GPIO_InitStruct.Pin = display->dio_pin;
	HAL_GPIO_Init(display->dio_port, &GPIO_InitStruct);
	TM1637Display_bitDelay(display);
}

static bool TM1637Display_writeByte(TM1637Display* display, uint8_t b)
{
uint8_t data = b;

for (uint8_t i = 0; i < 8; i++) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = display->clk_pin;
	HAL_GPIO_Init(display->clk_port, &GPIO_InitStruct);
	TM1637Display_bitDelay(display);

	if (data & 0x01)
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	else
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pin = display->dio_pin;
		HAL_GPIO_Init(display->dio_port, &GPIO_InitStruct);

	TM1637Display_bitDelay(display);

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin = display->clk_pin;
	HAL_GPIO_Init(display->clk_port, &GPIO_InitStruct);
	TM1637Display_bitDelay(display);
	data = data >> 1;
	}

GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
GPIO_InitStruct.Pin = display->clk_pin;
HAL_GPIO_Init(display->clk_port, &GPIO_InitStruct);

GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pin = display->dio_pin;
HAL_GPIO_Init(display->dio_port, &GPIO_InitStruct);
TM1637Display_bitDelay(display);

GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pin = display->clk_pin;
HAL_GPIO_Init(display->clk_port, &GPIO_InitStruct);
TM1637Display_bitDelay(display);
uint8_t ack = HAL_GPIO_ReadPin(display->dio_port, display->dio_pin);
	if (ack == 0) {
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pin = display->dio_pin;
		HAL_GPIO_Init(display->dio_port, &GPIO_InitStruct);
	}

	TM1637Display_bitDelay(display);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = display->clk_pin;
	HAL_GPIO_Init(display->clk_port, &GPIO_InitStruct);
	TM1637Display_bitDelay(display);

	return ack;
}

static void TM1637Display_showDots(uint8_t dots, uint8_t* digits)
{
	for (int i = 0; i < 4; ++i) {
		    digits[i] |= (dots & 0x80);
		        dots <<= 1;
	}
}

static void TM1637Display_showNumberBaseEx(TM1637Display* display, int8_t base, uint16_t num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos)
{
    bool negative = false;
    if (base < 0) {
        base = -base;
        negative = true;
    }

    uint8_t digits[4];

    if (num == 0 && !leading_zero) {
        for (uint8_t i = 0; i < (length - 1); i++)
            digits[i] = 0;
        digits[length - 1] = TM1637Display_encodeDigit(0);
    } else {
        for (int i = length - 1; i >= 0; --i) {
            uint8_t digit = num % base;

            if (digit == 0 && num == 0 && leading_zero == false)
                digits[i] = 0;
            else
                digits[i] = TM1637Display_encodeDigit(digit);

            if (digit == 0 && num == 0 && negative) {
                digits[i] = minusSegments;
                negative = false;
            }

            num /= base;
        }
    }

    if (dots != 0) {
        TM1637Display_showDots(dots, digits);
    }

    TM1637Display_setSegments(display, digits, length, pos);
}



