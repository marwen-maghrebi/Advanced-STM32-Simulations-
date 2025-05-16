/*
 * light_sensor.h
 *
 *  Created on: Sep 2, 2024
 *      Author: pc1
 */

#ifndef INC_LIGHT_SENSOR_H_
#define INC_LIGHT_SENSOR_H_

#include"main.h"

// Function prototypes
void LightSensor_Init(ADC_HandleTypeDef* hadc, UART_HandleTypeDef* huart);
float LightSensor_ReadVout(void);
float LightSensor_ReadLuminance(void);
void LightSensor_PrintReadings(void);
#endif /* INC_LIGHT_SENSOR_H_ */
