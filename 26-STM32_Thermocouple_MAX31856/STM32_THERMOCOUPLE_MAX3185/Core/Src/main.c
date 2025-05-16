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
#include "Max31856.h"
#include<stdio.h>
#include<string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  char MSG[100];
       char MSG0[100];
       char MSG1[100]=  "Could not initialize thermocouple\n\r";
       char MSG3[100]=  "$$***************************************$$\n\r";
       char MSG2[100]=  "Thermocouple type: ";
       char MSG4[100]=  "B Type\n\r";
       char MSG5[100]=  "E Type\n\r";
       char MSG6[100]=  "J Type\n\r";
       char MSG7[100]=  "K Type\n\r";
       char MSG8[100]=  "N Type\n\r";
       char MSG9[100]=  "R Type\n\r";
       char MSG10[100]= "S Type\n\r";
       char MSG11[100]= "T Type\n\r";
       char MSG12[100]= "Voltage x8 Gain mode\n\r";
       char MSG13[100]= "Voltage x8 Gain mode\n\r";
       char MSG14[100]= "Unknown\n\r";
       char MSG15[100]= "Cold Junction Range Fault\n\r";
       char MSG16[100]= "Thermocouple Range Fault\n\r";
       char MSG17[100]= "Cold Junction High Fault\n\r";
       char MSG18[100]= "Cold Junction Low Fault\n\r";
       char MSG19[100]= "Thermocouple High Fault\n\r";
       char MSG20[100]= "Thermocouple Low Fault\n\r";
       char MSG21[100]= "Over/Under Voltage Fault\n\r";
       char MSG22[100]= "hermocouple Open Faultn\r";

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
  if (!begin()) {
        HAL_UART_Transmit(&huart1,(uint8_t*)MSG1,sizeof(MSG1), 100);
        while (1) HAL_Delay(10);
      }
    setThermocoupleType(MAX31856_TCTYPE_K);
    HAL_UART_Transmit(&huart1,(uint8_t*)MSG2,sizeof(MSG2), 100);

  switch (getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: HAL_UART_Transmit(&huart1,(uint8_t*)MSG4,sizeof(MSG4), 100);  break;
    case MAX31856_TCTYPE_E: HAL_UART_Transmit(&huart1,(uint8_t*)MSG5,sizeof(MSG5), 100);  break;
    case MAX31856_TCTYPE_J: HAL_UART_Transmit(&huart1,(uint8_t*)MSG6,sizeof(MSG6), 100);  break;
    case MAX31856_TCTYPE_K: HAL_UART_Transmit(&huart1,(uint8_t*)MSG7,sizeof(MSG7), 100);  break;
    case MAX31856_TCTYPE_N: HAL_UART_Transmit(&huart1,(uint8_t*)MSG8,sizeof(MSG8), 100);  break;
    case MAX31856_TCTYPE_R: HAL_UART_Transmit(&huart1,(uint8_t*)MSG9,sizeof(MSG9), 100);  break;
    case MAX31856_TCTYPE_S: HAL_UART_Transmit(&huart1,(uint8_t*)MSG10,sizeof(MSG10), 100);  break;
    case MAX31856_TCTYPE_T: HAL_UART_Transmit(&huart1,(uint8_t*)MSG11,sizeof(MSG11), 100);  break;
    case MAX31856_VMODE_G8:  HAL_UART_Transmit(&huart1,(uint8_t*)MSG12,sizeof(MSG12), 100); break;
    case MAX31856_VMODE_G32: HAL_UART_Transmit(&huart1,(uint8_t*)MSG13,sizeof(MSG13), 100); break;
    default:  HAL_UART_Transmit(&huart1,(uint8_t*)MSG14,sizeof(MSG14), 100);break;
    }
  HAL_Delay(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    sprintf(MSG,"1-Cold Junction Temp: %.2f degrees Celsius\n\r",readCJTemperature());
    sprintf(MSG0,"2-Thermocouple Temp: %.2f degrees Celsius\n\r",
    readThermocoupleTemperature());
    HAL_UART_Transmit(&huart1,(uint8_t *) MSG, sizeof(MSG), 100);
    HAL_UART_Transmit(&huart1,(uint8_t *) MSG0, sizeof(MSG0), 100);
    HAL_UART_Transmit(&huart1,(uint8_t *) MSG3, sizeof(MSG3), 100);
    uint8_t fault = readFault();
    if (fault) {
    if (fault & MAX31856_FAULT_CJRANGE)  HAL_UART_Transmit(&huart1,(uint8_t*)MSG15,sizeof(MSG15), 100);
    if (fault & MAX31856_FAULT_TCRANGE)  HAL_UART_Transmit(&huart1,(uint8_t*)MSG16,sizeof(MSG16), 100);
    if (fault & MAX31856_FAULT_CJHIGH)   HAL_UART_Transmit(&huart1,(uint8_t*)MSG17,sizeof(MSG17), 100);
    if (fault & MAX31856_FAULT_CJLOW)    HAL_UART_Transmit(&huart1,(uint8_t*)MSG18,sizeof(MSG18), 100);
    if (fault & MAX31856_FAULT_TCHIGH)   HAL_UART_Transmit(&huart1,(uint8_t*)MSG19,sizeof(MSG19), 100);
    if (fault & MAX31856_FAULT_TCLOW)    HAL_UART_Transmit(&huart1,(uint8_t*)MSG20,sizeof(MSG20), 100);
    if (fault & MAX31856_FAULT_OVUV)     HAL_UART_Transmit(&huart1,(uint8_t*)MSG21,sizeof(MSG21), 100);
    if (fault & MAX31856_FAULT_OPEN)     HAL_UART_Transmit(&huart1,(uint8_t*)MSG22,sizeof(MSG22), 100);
    }
    HAL_UART_Transmit(&huart1,(uint8_t *) MSG, sizeof(MSG), 100);
    HAL_Delay(1000);
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
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

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

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
  huart1.Init.BaudRate = 9600;
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
  HAL_GPIO_WritePin(GPIOA, MAX_FAULT_Pin|SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : MAX_FAULT_Pin SPI1_CS_Pin */
  GPIO_InitStruct.Pin = MAX_FAULT_Pin|SPI1_CS_Pin;
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
