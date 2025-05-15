#include "MCP492X.h"

static void CS_Select(DAC_MCP49x1_HandleTypeDef* hdac) {
    HAL_GPIO_WritePin(hdac->cs_port, hdac->cs_pin, GPIO_PIN_RESET);
}

static void CS_Deselect(DAC_MCP49x1_HandleTypeDef* hdac) {
    HAL_GPIO_WritePin(hdac->cs_port, hdac->cs_pin, GPIO_PIN_SET);
}

HAL_StatusTypeDef DAC_MCP49x1_Init(DAC_MCP49x1_HandleTypeDef* hdac, DAC_MCP49x1_Model model,
                                   SPI_HandleTypeDef* hspi, GPIO_TypeDef* cs_port, uint16_t cs_pin,
                                   GPIO_TypeDef* ldac_port, uint16_t ldac_pin) {
    hdac->model = model;
    hdac->hspi = hspi;
    hdac->cs_port = cs_port;
    hdac->cs_pin = cs_pin;
    hdac->ldac_port = ldac_port;
    hdac->ldac_pin = ldac_pin;
    hdac->bufferVref = 0;
    hdac->gain2x = 0;

    switch (model) {
        case MCP4901:
            hdac->bitwidth = 8;
            break;
        case MCP4911:
            hdac->bitwidth = 10;
            break;
        case MCP4921:
            hdac->bitwidth = 12;
            break;
        default:
            return HAL_ERROR;
    }

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = cs_pin;
    HAL_GPIO_Init(cs_port, &GPIO_InitStruct);

    if (ldac_port != NULL && ldac_pin != 0) {
        GPIO_InitStruct.Pin = ldac_pin;
        HAL_GPIO_Init(ldac_port, &GPIO_InitStruct);
    }

    CS_Deselect(hdac);
    if (ldac_port != NULL && ldac_pin != 0) {
        HAL_GPIO_WritePin(ldac_port, ldac_pin, GPIO_PIN_SET);
    }

    return HAL_OK;
}

void DAC_MCP49x1_SetBuffer(DAC_MCP49x1_HandleTypeDef* hdac, uint8_t buffer) {
    hdac->bufferVref = buffer;
}

HAL_StatusTypeDef DAC_MCP49x1_SetGain(DAC_MCP49x1_HandleTypeDef* hdac, uint8_t gain) {
    if (gain == 1) {
        hdac->gain2x = 0;
        return HAL_OK;
    } else if (gain == 2) {
        hdac->gain2x = 1;
        return HAL_OK;
    }
    return HAL_ERROR;
}

void DAC_MCP49x1_Shutdown(DAC_MCP49x1_HandleTypeDef* hdac) {
    CS_Select(hdac);
    uint16_t out = (hdac->bufferVref << 14) | ((!hdac->gain2x) << 13);
    uint8_t data[2] = {(out >> 8) & 0xFF, out & 0xFF};
    HAL_SPI_Transmit(hdac->hspi, data, 2, HAL_MAX_DELAY);
    CS_Deselect(hdac);
}

HAL_StatusTypeDef DAC_MCP49x1_Output(DAC_MCP49x1_HandleTypeDef* hdac, uint16_t data) {
    data &= (0xFFFF >> (16 - hdac->bitwidth));

    uint16_t out = (hdac->bufferVref << 14) | ((!hdac->gain2x) << 13) | (1 << 12) | (data << (12 - hdac->bitwidth));
    uint8_t spi_data[2] = {(out >> 8) & 0xFF, out & 0xFF};

    CS_Select(hdac);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hdac->hspi, spi_data, 2, HAL_MAX_DELAY);
    CS_Deselect(hdac);

    return status;
}

void DAC_MCP49x1_Latch(DAC_MCP49x1_HandleTypeDef* hdac) {
    if (hdac->ldac_port == NULL || hdac->ldac_pin == 0) return;

    HAL_GPIO_WritePin(hdac->ldac_port, hdac->ldac_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hdac->ldac_port, hdac->ldac_pin, GPIO_PIN_SET);
}
