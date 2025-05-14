/*
 * uart.h
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_UART_H_
#define LIBRERIAS_UART_H_


#include "main.h"



void uartx_write(UART_HandleTypeDef *huart,uint8_t ch);
void uartx_write_text(UART_HandleTypeDef *huart, char *info);





#endif /* LIBRERIAS_UART_H_ */
