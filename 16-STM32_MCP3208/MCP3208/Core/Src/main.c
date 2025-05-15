/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mcp3208.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
MCP3208_HandleTypeDef hmcp3208;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
#define VREF 2.5 // Reference voltage
#define ADC_RESOLUTION 4096  // 12-bit ADC

const float V_REF = 2.5f;    // Adjust based on your reference voltage
const float V_REF2 = 5.0f;    // Adjust based on your reference voltage

const int ADC_MAX_VALUE = 4095; // MCP3208 is a 12-bit ADC
const int ADC_MAX_VALUE2 = 1024; // MCP3208 is a 12-bit ADC

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void print_uart(const char* str)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

float adc_to_voltage(uint16_t adc_value)
{
    return (float)adc_value * VREF / ADC_RESOLUTION ;

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // Initialize MCP3208
     hmcp3208.hspi = &hspi1;
     hmcp3208.CS_GPIO_Port = GPIOA;
     hmcp3208.CS_Pin = GPIO_PIN_4;
     MCP3208_Init(&hmcp3208);

     char temp[10];
     int ctr = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (ctr == 0)
	  {
	      HAL_UART_Transmit(&huart1, (uint8_t*)"\n\r", 2, HAL_MAX_DELAY);
	      HAL_UART_Transmit(&huart1, (uint8_t*)"\n\r              Single Ended (V)           |    Differential (V)\n\r", 68, HAL_MAX_DELAY);
	      HAL_UART_Transmit(&huart1, (uint8_t*)"   0    1    2    3    4    5    6    7  |   0     1    2    3\n\r", 66, HAL_MAX_DELAY);
	      HAL_UART_Transmit(&huart1, (uint8_t*)"------------------------------------------------------------------------\n\r", 66, HAL_MAX_DELAY);
	      HAL_UART_Transmit(&huart1, (uint8_t*)"\n\r", 2, HAL_MAX_DELAY);
	  }

	  ctr++;
	  if (ctr == 10)
	  {
	      ctr = 0;
	  }

	  // Read single-ended values
	  for (int i = 0; i < 8; i++)
	  {
	      uint16_t value = MCP3208_AnalogRead(&hmcp3208, i);
	      float voltage = (value * V_REF) / ADC_MAX_VALUE; // Convert to voltage
	      sprintf(temp, "%5.2f", voltage);  // Format to 2 decimal places with 5 characters (including leading spaces)
	      HAL_UART_Transmit(&huart1, (uint8_t*)temp, strlen(temp), HAL_MAX_DELAY);
	  }
	  HAL_UART_Transmit(&huart1, (uint8_t*)" | ", 2, HAL_MAX_DELAY);

	  // Read differential values
	  for (int i = 0; i < 4; i++)
	  {
	      int16_t diff_value = MCP3208_AnalogReadDif(&hmcp3208, i);
	      float diff_voltage = (diff_value * V_REF) / ADC_MAX_VALUE; // Convert to voltage
	      sprintf(temp, "%5.2f ", diff_voltage); // Format to 2 decimal places with a single space after the value
	      HAL_UART_Transmit(&huart1, (uint8_t*)temp, strlen(temp), HAL_MAX_DELAY);
	  }
	  HAL_UART_Transmit(&huart1, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
	  HAL_Delay(1000); // Delay for stability



  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{
  /* USER CODE BEGIN SPI1_Init 0 */
  // Additional user code can be added here, e.g., GPIO settings.
  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */
  // Further configurations can go here if needed.
  /* USER CODE END SPI1_Init 1 */

  /* SPI1 parameter configuration */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT; // Change if needed
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW; // Match with your device
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE; // Match with your device
  hspi1.Init.NSS = SPI_NSS_SOFT; // Ensure proper CS handling in your code
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; // Check this value
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10; // Only needed if CRC is enabled
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler(); // Ensure proper error handling here
  }

  /* USER CODE BEGIN SPI1_Init 2 */
  // Additional user code can be added here.
  /* USER CODE END SPI1_Init 2 */
}


/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\n\r", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
