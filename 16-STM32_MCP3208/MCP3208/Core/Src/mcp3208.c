/*
 * mcp3208.c
 *
 *  Created on: Aug 27, 2024
 *      Author: pc1
 */


#include "mcp3208.h"

static uint8_t SPI_Transfer(MCP3208_HandleTypeDef* hmcp3208, uint8_t data)
{
    uint8_t received_data;
    HAL_SPI_TransmitReceive(hmcp3208->hspi, &data, &received_data, 1, 50000);
    return received_data;
}


void MCP3208_Init(MCP3208_HandleTypeDef* hmcp3208)
{
    // Set CS pin high (inactive)
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_SET);
}

uint16_t MCP3208_AnalogRead(MCP3208_HandleTypeDef* hmcp3208, uint8_t pin)
{
    uint8_t addr = 0b01100000 | ((pin & 0b111) << 2);
    uint8_t b1, b2;

    // Pull CS low
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_RESET);

    // Send the address and receive data
    (void) SPI_Transfer(hmcp3208, addr);
    b1 = SPI_Transfer(hmcp3208, 0);
    b2 = SPI_Transfer(hmcp3208, 0);

    // Pull CS high
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_SET);

    return (b1 << 4) | (b2 >> 4);  // Combine the two received bytes
}
/*int16_t MCP3208_AnalogReadDif(MCP3208_HandleTypeDef* hmcp3208, uint8_t pin)
{
    uint8_t diff;
    uint8_t b1, b2;
    uint8_t addr = 0b01000000 | ((pin & 0b11) << 3);

    // First measurement
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_RESET);
    (void) SPI_Transfer(hmcp3208, addr);
    b1 = SPI_Transfer(hmcp3208, 0);
    b2 = SPI_Transfer(hmcp3208, 0);
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_SET);

    diff = (b1 << 4) | (b2 >> 4);
    if (diff > 0) {
        return diff;
    }

    // Second measurement (reverse polarity)
    addr = 0b01000100 | ((pin & 0b11) << 3);
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_RESET);
    (void) SPI_Transfer(hmcp3208, addr);
    b1 = SPI_Transfer(hmcp3208, 0);
    b2 = SPI_Transfer(hmcp3208, 0);
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_SET);

    diff = (b1 << 4) | (b2 >> 4);
    return -diff;
}*/
/*int16_t MCP3208_AnalogReadDif(MCP3208_HandleTypeDef* hmcp3208, uint8_t pin)
{
    uint8_t b1, b2;
    uint8_t addr = 0b01000000 | ((pin & 0b11) << 3);  // Differential read address

    // Pull CS low
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_RESET);

    // Send the address and receive data
    (void) SPI_Transfer(hmcp3208, addr);
    b1 = SPI_Transfer(hmcp3208, 0);
    b2 = SPI_Transfer(hmcp3208, 0);

    // Pull CS high
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_SET);

    // Combine the bytes to form the 12-bit result
    int16_t diff = ((b1 & 0x0F) << 8) | b2;  // b1 contains the upper 4 bits of the result

    // Check if the differential reading is negative
    if (diff & 0x800) {  // Check if the 12th bit is set
        diff |= 0xF000;  // Extend the sign to make it a negative value
    }

    return diff;  // Return the differential value
}
*/
/*int16_t MCP3208_AnalogReadDif(MCP3208_HandleTypeDef* hmcp3208, uint8_t pin)
{
    uint8_t b1, b2;
    uint8_t addr = 0b01000000 | ((pin & 0b11) << 3);  // Differential read address

    // Pull CS low
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_RESET);

    // Send the address and receive data
    (void) SPI_Transfer(hmcp3208, addr);
    b1 = SPI_Transfer(hmcp3208, 0);
    b2 = SPI_Transfer(hmcp3208, 0);

    // Pull CS high
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_SET);

    // Combine the bytes to form the 12-bit result
    int16_t diff = (b1 << 8) | b2;  // Correct combination of bytes

    diff = diff >> 4;  // Right shift to align the 12-bit result

    // Check if the differential reading is negative
    if (diff & 0x800) {  // Check if the 12th bit is set
        diff |= 0xF000;  // Extend the sign to make it a negative value
    }

    return diff;  // Return the differential value
}
*/int16_t MCP3208_AnalogReadDif(MCP3208_HandleTypeDef* hmcp3208, uint8_t pin)
{
    uint8_t b1, b2;
    int16_t diff;
    uint8_t addr;

    // First measurement
    addr = 0b01000000 | ((pin & 0b11) << 3);
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_RESET);
    (void) SPI_Transfer(hmcp3208, addr);
    b1 = SPI_Transfer(hmcp3208, 0);
    b2 = SPI_Transfer(hmcp3208, 0);
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_SET);

    diff = (b1 << 4) | (b2 >> 4);
    if (diff > 0) {
        return diff;
    }

    // Second measurement with reverse polarity
    addr = 0b01000100 | ((pin & 0b11) << 3);
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_RESET);
    (void) SPI_Transfer(hmcp3208, addr);
    b1 = SPI_Transfer(hmcp3208, 0);
    b2 = SPI_Transfer(hmcp3208, 0);
    HAL_GPIO_WritePin(hmcp3208->CS_GPIO_Port, hmcp3208->CS_Pin, GPIO_PIN_SET);

    diff = (b1 << 4) | (b2 >> 4);
    return -diff;
}
