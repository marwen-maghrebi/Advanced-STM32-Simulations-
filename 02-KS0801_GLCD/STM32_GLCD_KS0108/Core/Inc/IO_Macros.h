/*
 * IO_Macros.h
 *
 *  Created on: May 13, 2024
 *      Author: PC-MAGHREBI
 */
#ifndef IO_MACROS_H_INCLUDED
#define IO_MACROS_H_INCLUDED

#include "main.h"

// Define GPIO port and pin macros for STM32
#define PORTA GPIOA
#define PORTB GPIOB
#define PORTC GPIOC
// Add more ports as needed...

#define PinMode(port, pin, mode) \
    do { \
        GPIO_InitTypeDef GPIO_InitStruct; \
        GPIO_InitStruct.Pin = pin; \
        GPIO_InitStruct.Mode = mode; \
        GPIO_InitStruct.Pull = GPIO_NOPULL; \
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; \
        HAL_GPIO_Init(port, &GPIO_InitStruct); \
    } while(0)

#define DigitalRead(port, pin) HAL_GPIO_ReadPin(port, pin)
#define DigitalWrite(port, pin, value) HAL_GPIO_WritePin(port, pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define DigitalLevelToggle(pin) HAL_GPIO_TogglePin(pin)

#define BitSet(x, y) ((x) |= (1U << (y)))
#define BitClear(x, y) ((x) &= ~(1U << (y)))
#define BitToggle(x, y) ((x) ^= (1U << (y)))
#define BitCheck(x, y) (((x) & (1U << (y))) != 0)

#define Input GPIO_MODE_INPUT
#define Output GPIO_MODE_OUTPUT_PP
#define Low GPIO_PIN_RESET
#define High GPIO_PIN_SET
#define False 0
#define True 1
#define pgm_read_byte(address) (*(const unsigned char *)(address))


//------------------
#endif
