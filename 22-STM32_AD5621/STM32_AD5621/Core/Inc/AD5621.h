/*
 * AD5621.h
 *
 *  Created on: Sep 12, 2024
 *      Author: pc1
 */

#ifndef INC_AD5621_H_
#define INC_AD5621_H_

#include "stm32f1xx_hal.h"  // Adjust this based on your STM32 series

// Function declarations
void ad5621_Init(SPI_HandleTypeDef *hspi, uint16_t cs_pin, GPIO_TypeDef *cs_port);
void ad5621_SendToDAC(uint16_t value);
void ad5621_GenerateSineTable(uint16_t *sine_table, uint16_t points);


#endif /* INC_AD5621_H_ */
