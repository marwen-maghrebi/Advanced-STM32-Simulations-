/*
 * uart.h
 *
 *  Created on: May 14, 2024
 *      Author: PC-MAGHREBI
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stm32f1xx_hal.h"

#define RX_BUFFER_SIZE 32

typedef struct _ring_buffer {
    uint8_t buffer[RX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} ring_buffer_t;


void push(ring_buffer_t*, uint8_t);
uint8_t pop(ring_buffer_t*);
void rxBufferInit(ring_buffer_t*);
int8_t uart_available(ring_buffer_t*);
void Uart_isr (UART_HandleTypeDef *huart);


#endif /* INC_UART_H_ */
