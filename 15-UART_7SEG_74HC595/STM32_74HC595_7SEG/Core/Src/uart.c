/*
 * uart.c
 *
 *  Created on: May 14, 2024
 *      Author: PC-MAGHREBI
 */

#include "uart.h"

ring_buffer_t uart_rx={{0},0,0};
extern UART_HandleTypeDef huart2;

void rxBufferInit(ring_buffer_t *uart) {
    uart->head = 0;
    uart->tail = 0;
    memset(uart->buffer, 0, sizeof(RX_BUFFER_SIZE));
}

void push(ring_buffer_t *uart, uint8_t ch) {
    uint16_t i = (unsigned int) (uart->head + 1) % RX_BUFFER_SIZE;
    if (i != uart->tail) {
        uart->buffer[uart->head] = ch;
        uart->head = i;
    }
}

uint8_t pop(ring_buffer_t *uart) {

    if (uart->head == uart->tail) {
        return -1;
    } else {

        unsigned char cpop = uart->buffer[uart->tail];
        uart->tail = (uint16_t) (uart->tail + 1) % RX_BUFFER_SIZE;

        return cpop;

    }
}

int8_t uart_available(ring_buffer_t *uart) {

    uint8_t n = (unsigned int) (RX_BUFFER_SIZE + (uart->head) - (uart->tail))
            % RX_BUFFER_SIZE;
    return n;
}

void Uart_isr(UART_HandleTypeDef *huart) {
    uint32_t isrflags = READ_REG(huart->Instance->SR);
    uint32_t cr1its = READ_REG(huart->Instance->CR1);

    /* if DR is not empty and the Rx Int is enabled */
    if (((isrflags & USART_SR_RXNE) != RESET)
            && ((cr1its & USART_CR1_RXNEIE) != RESET)) {
        /******************
         *  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
         *          error) and IDLE (Idle line detected) flags are cleared by software
         *          sequence: a read operation to USART_SR register followed by a read
         *          operation to USART_DR register.
         * @note   RXNE flag can be also cleared by a read to the USART_DR register.
         * @note   TC flag can be also cleared by software sequence: a read operation to
         *          USART_SR register followed by a write operation to USART_DR register.
         * @note   TXE flag is cleared only by a write to the USART_DR register.
         *********************/
        huart->Instance->SR; /* Read status register */
        unsigned char c = huart->Instance->DR; /* Read data register */
        push(&uart_rx, c);  // store data in buffer
        return;
    }
}
