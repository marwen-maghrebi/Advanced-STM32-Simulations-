#include "light_sensor.h"
#include <stdio.h>
#include <string.h>

static ADC_HandleTypeDef* LightSensor_ADC;
static UART_HandleTypeDef* LightSensor_UART;

//static float VoutArray[] = {0.0011498, 0.0033908, 0.011498, 0.041803, 0.15199, 0.53367, 1.3689, 1.9068, 2.3};
static float VoutArray[] = {1.0202, 2.6102 ,2.6655 ,2.71968,2.74121,2.75225,2.75973,2.7654,2.76996,2.7737};

static float LuxArray[] = {1.0108, 3.1201, 9.8051, 27.43, 69.545, 232.67, 645.11, 730.52, 1000};

static float FmultiMap(float val, float * in, float * out, uint8_t size);

void LightSensor_Init(ADC_HandleTypeDef* hadc, UART_HandleTypeDef* huart) {
    LightSensor_ADC = hadc;
    LightSensor_UART = huart;
}

float LightSensor_ReadVout(void) {
    HAL_ADC_Start(LightSensor_ADC);
    HAL_ADC_PollForConversion(LightSensor_ADC, HAL_MAX_DELAY);
    uint32_t adcValue = HAL_ADC_GetValue(LightSensor_ADC);

    // Assuming 3.3V reference voltage and 12-bit ADC resolution
    float MeasuredVout = adcValue * (3.3f / 4095.0f);

    return MeasuredVout;
}

float LightSensor_ReadLuminance(void) {
    float MeasuredVout = LightSensor_ReadVout();
    float Luminance = FmultiMap(MeasuredVout, VoutArray, LuxArray, 9);
    return Luminance;
}

void LightSensor_PrintReadings(void) {
    char buffer[50];
    float vout = LightSensor_ReadVout();
    float luminance = LightSensor_ReadLuminance();

    snprintf(buffer, sizeof(buffer), "Vout = %.4f V, Luminance = %.2f Lux\r\n", vout, luminance);
    HAL_UART_Transmit(LightSensor_UART, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}

static float FmultiMap(float val, float * in, float * out, uint8_t size) {
    if (val <= in[0]) return out[0];
    if (val >= in[size-1]) return out[size-1];

    uint8_t pos = 1;
    while(val > in[pos]) pos++;

    if (val == in[pos]) return out[pos];

    return (val - in[pos-1]) * (out[pos] - out[pos-1]) / (in[pos] - in[pos-1]) + out[pos-1];
}
