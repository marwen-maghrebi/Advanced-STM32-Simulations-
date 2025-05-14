/*
 * uart.c
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */

#include "uart.h"


void uartx_write(UART_HandleTypeDef *huart,uint8_t ch)
{
HAL_UART_Transmit(huart, &ch, 1, 0xffff);

}

void uartx_write_text(UART_HandleTypeDef *huart, char *info)
{

while(*info)  uartx_write(huart,*info++);

}





