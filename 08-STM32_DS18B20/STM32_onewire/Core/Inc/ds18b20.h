/*
 * ds18b20.h
 *
 *  Created on: 12/03/2023
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_DS18B20_H_
#define LIBRERIAS_DS18B20_H_

#include "main.h"

// Delay functions
#define delay_us delay_us_dwt

void delay_us_dwt(uint32_t us);
void delay_us_dwt_init(void);

// DS18B20 functions
float DS18b20_temp();

// OneWire functions
uint8_t onewire_reset();
void onewire_Write(uint8_t dato);
uint8_t onewire_Read();

// GPIO functions
void Output_Pin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Input_Pin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /* LIBRERIAS_DS18B20_H_ */
