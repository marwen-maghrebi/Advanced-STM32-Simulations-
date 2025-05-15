/*
 * AD5621.c
 *
 *  Created on: Sep 12, 2024
 *      Author: pc1
 */



#include "AD5621.h"
#include <math.h>  // For sine calculations

// Private variables to hold the SPI handle, CS pin, and GPIO port
static SPI_HandleTypeDef *ad5621_hspi;
static uint16_t ad5621_cs_pin;
static GPIO_TypeDef *ad5621_cs_port;
#define DAC_RESOLUTION 65535  // 2^16 - 1 for 16-bit DAC

void ad5621_Init(SPI_HandleTypeDef *hspi, uint16_t cs_pin, GPIO_TypeDef *cs_port)
{
	ad5621_hspi = hspi;       // Store the SPI handle
	ad5621_cs_pin = cs_pin;   // Store the CS pin
	ad5621_cs_port = cs_port; // Store the CS GPIO port
}

void ad5621_SendToDAC(uint16_t value)
{
	HAL_GPIO_WritePin(ad5621_cs_port, ad5621_cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(ad5621_hspi, (uint8_t*)&value, 1, 1000);  // Transmit 16-bit data
	HAL_GPIO_WritePin(ad5621_cs_port, ad5621_cs_pin, GPIO_PIN_SET);
}

void ad5621_GenerateSineTable(uint16_t *sine_table, uint16_t points)
{
	for (int i = 0; i < points; i++)
	{
		    float radia = (float)i * 2 * M_PI / points;
		    // Scale sine wave
		     sine_table[i] = (uint16_t)round((sinf(radia) + 1.0f) * DAC_RESOLUTION / 10.0f);
	}
}

