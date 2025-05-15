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
#include<stdio.h>
#include<string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBOUNCE_DELAY 50 // ms

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t motorRunning = 0;

uint8_t currentDutyCycle = 0; //0: 25%, 1: 50%, 2: 75%, 3: 99%

uint32_t lastDebounceTime = 0;
static uint8_t lastButtonState;// = GPIO_PIN_RESET; // Assume button is not pressed initially

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void HandleStartStop(void);
void HandleDutyCycle(void);
void UpdatePWM(void);
void SendUartMessage(char* message);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HandleStartStop(void)
{
  static GPIO_PinState lastStartStopState = GPIO_PIN_SET; // Track previous state
  GPIO_PinState currentStartStopState = HAL_GPIO_ReadPin(START_STOP_GPIO_Port, START_STOP_Pin);

  // Detect state change from high to low (button press)
  if (lastStartStopState == GPIO_PIN_SET && currentStartStopState == GPIO_PIN_RESET)
  {
    uint32_t currentTime = HAL_GetTick();
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
    {
      motorRunning = !motorRunning;
      // Send UART message indicating motor state
	   if (motorRunning)
		 {SendUartMessage("Motor Started\n\r");}
	   else
		 {SendUartMessage("Motor Stopped\n\r");}
      UpdatePWM();
      lastDebounceTime = currentTime;
    }
  }

  // Handle the motor state based on `motorRunning` status
  if (motorRunning)
  {
    HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET); // Ensure DIR pin is set appropriately
    HAL_TIM_Base_Start(&htim2); // Start Timer2
    HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_2); // Start PWM
  }
  else
  {
	//HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET); // Ensure DIR pin is set appropriately
    HAL_TIM_Base_Stop(&htim2); // Stop Timer2
    HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_2); // Stop PWM
  }

  // Update the last button state
  lastStartStopState = currentStartStopState;
}



void HandleDutyCycle(void)
{
  uint8_t currentButtonState = HAL_GPIO_ReadPin(DUTY_CYCLE_GPIO_Port, DUTY_CYCLE_Pin);

  if (currentButtonState != lastButtonState) // Check for state change
  {
    uint32_t currentTime = HAL_GetTick();
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
    {
      if (currentButtonState == GPIO_PIN_RESET) // Button is pressed
      {
        currentDutyCycle = (currentDutyCycle + 1) % 4;
        UpdatePWM();
      }
      lastDebounceTime = currentTime;
    }
  }

  lastButtonState = currentButtonState; // Update the last button state
}

void UpdatePWM(void)
{
    if (motorRunning)
    {
        uint32_t pwmValue;
        char msg[100];
        float tau_max = 1.0; // Full-scale maximum torque in Nm (example value)
        float moment_of_inertia = 0.002; // Effective mass in kg·m²
        float angular_acceleration;

        switch (currentDutyCycle)
        {
            case 0:
                pwmValue = 65535 * 0.25; // 25% of 65535
                angular_acceleration = (0.25 * tau_max) / moment_of_inertia;
                sprintf(msg, "Duty Cycle: 25%%, Acceleration: %.2f rad/s^2\n\r", angular_acceleration);
                break;
            case 1:
                pwmValue = 65535 * 0.50; // 50% of 65535
                angular_acceleration = (0.50 * tau_max) / moment_of_inertia;
                sprintf(msg, "Duty Cycle: 50%%, Acceleration: %.2f rad/s^2\n\r", angular_acceleration);
                break;
            case 2:
                pwmValue = 65535 * 0.75; // 75% of 65535
                angular_acceleration = (0.75 * tau_max) / moment_of_inertia;
                sprintf(msg, "Duty Cycle: 75%%, Acceleration: %.2f rad/s^2\n\r", angular_acceleration);
                break;
            case 3:
                pwmValue = 65535 * 0.99; // 10% of 65535
                angular_acceleration = (0.99 * tau_max) / moment_of_inertia;
                sprintf(msg, "Duty Cycle: 99%%, Acceleration: %.2f rad/s^2\n\r", angular_acceleration);
                break;
            default:
                pwmValue = 0;
                sprintf(msg, "Duty Cycle: Error\n\r");
                break;
        }

        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwmValue);

        // Send UART message indicating new duty cycle and acceleration
        SendUartMessage(msg);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
    }
}


// Function to send UART message
void SendUartMessage(char* message)
{
  HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
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
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

    SendUartMessage("Initialization  Completed \n\r");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	    HandleStartStop();
	    HandleDutyCycle();
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : START_STOP_Pin DUTY_CYCLE_Pin */
  GPIO_InitStruct.Pin = START_STOP_Pin|DUTY_CYCLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DIR_Pin */
  GPIO_InitStruct.Pin = DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIR_GPIO_Port, &GPIO_InitStruct);

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
     ex: printf("Wrong parameters value: file %s on line %d\r\n\r\r", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
