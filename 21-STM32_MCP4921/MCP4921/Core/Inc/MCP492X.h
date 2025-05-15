#ifndef MCP492X_H
#define MCP492X_H

#include "stm32f1xx_hal.h"

typedef enum {
    MCP4901 = 1,
    MCP4911,
    MCP4921
} DAC_MCP49x1_Model;

typedef struct {
    DAC_MCP49x1_Model model;
    SPI_HandleTypeDef* hspi;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef* ldac_port;
    uint16_t ldac_pin;
    uint8_t bitwidth;
    uint8_t bufferVref;
    uint8_t gain2x;
} DAC_MCP49x1_HandleTypeDef;

HAL_StatusTypeDef DAC_MCP49x1_Init(DAC_MCP49x1_HandleTypeDef* hdac, DAC_MCP49x1_Model model,
                                   SPI_HandleTypeDef* hspi, GPIO_TypeDef* cs_port, uint16_t cs_pin,
                                   GPIO_TypeDef* ldac_port, uint16_t ldac_pin);
void DAC_MCP49x1_SetBuffer(DAC_MCP49x1_HandleTypeDef* hdac, uint8_t buffer);
HAL_StatusTypeDef DAC_MCP49x1_SetGain(DAC_MCP49x1_HandleTypeDef* hdac, uint8_t gain);
void DAC_MCP49x1_Shutdown(DAC_MCP49x1_HandleTypeDef* hdac);
HAL_StatusTypeDef DAC_MCP49x1_Output(DAC_MCP49x1_HandleTypeDef* hdac, uint16_t data);
void DAC_MCP49x1_Latch(DAC_MCP49x1_HandleTypeDef* hdac);
#endif // MCP492X_H
