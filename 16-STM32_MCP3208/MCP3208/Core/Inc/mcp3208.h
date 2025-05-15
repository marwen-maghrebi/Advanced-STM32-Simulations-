//////////////////////////////////////////
/*
 * mcp3208.h
 *
 *  Created on: Aug 27, 2024
 *      Author: pc1
 */

#ifndef INC_MCP3208_H_
#define INC_MCP3208_H_
#include "stm32f1xx_hal.h"
typedef struct {
    SPI_HandleTypeDef* hspi;
    GPIO_TypeDef* CS_GPIO_Port;
    uint16_t CS_Pin;
} MCP3208_HandleTypeDef;
void MCP3208_Init(MCP3208_HandleTypeDef* hmcp3208);
uint16_t MCP3208_AnalogRead(MCP3208_HandleTypeDef* hmcp3208, uint8_t pin);
int16_t analogReadDif(uint8_t pin);
int16_t MCP3208_AnalogReadDif(MCP3208_HandleTypeDef* hmcp3208, uint8_t pin);
#endif /* INC_MCP3208_H_ */

