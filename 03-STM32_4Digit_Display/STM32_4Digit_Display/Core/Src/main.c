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
#include "TM1637.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Module connection pins (GPIO Pins)
#define CLK_PORT GPIOB
#define CLK_PIN GPIO_PIN_10
#define DIO_PORT GPIOB
#define DIO_PIN GPIO_PIN_11
 // The amount of time (in milliseconds) between tests
#define TEST_DELAY   250

const uint8_t SEG_DONE[] = {
		SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
		SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
		SEG_C | SEG_E | SEG_G,                           // n
		SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            //e
};TM1637Display display;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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
  /* USER CODE BEGIN 2 */

  TM1637Display_init(&display, CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, DEFAULT_BIT_DELAY);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  int k;
	  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
	  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

	  TM1637Display_setBrightness(&display, 0x0f, true);

	  // All segments onr t
	  TM1637Display_setSegments(&display, data, 4, 0);
	  HAL_Delay(TEST_DELAY);
	   // Selectively set different digits
	  data[0] = TM1637Display_encodeDigit(0);
	  data[1] = TM1637Display_encodeDigit(1);
	  data[2] = TM1637Display_encodeDigit(2);
	  data[3] = TM1637Display_encodeDigit(3);
	  TM1637Display_setSegments(&display, data, 4, 0);
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_clear(&display);
	  TM1637Display_setSegments(&display, data+2, 2, 2);
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_clear(&display);
	  TM1637Display_setSegments(&display, data+2, 2, 1);
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_clear(&display);
	  TM1637Display_setSegments(&display, data+1, 3, 1);
	  HAL_Delay(TEST_DELAY);

	  // Show decimal numbers with/without leading zeros
	  TM1637Display_showNumberDec(&display, 0, false, 4, 0); // Expect: ___0
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberDec(&display, 0, true, 4, 0);  // Expect: 0000
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberDec(&display, 1, false, 4, 0); // Expect: ___1
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberDec(&display, 1, true, 4, 0);  // Expect: 0001
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberDec(&display, 301, false, 4, 0); // Expect: _301
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberDec(&display, 301, true, 4, 0); // Expect: 0301
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_clear(&display);
	  TM1637Display_showNumberDec(&display, 14, false, 2, 1); // Expect: *14*
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_clear(&display);
	  TM1637Display_showNumberDec(&display, 4, true, 2, 2);  // Expect: __04
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_showNumberDec(&display, -1, false, 4, 0);  // Expect: __-1
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberDec(&display, -12, false, 4, 0);        // Expect: _-12
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberDec(&display, -999, false, 4, 0);       // Expect: -999
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_clear(&display);
	  TM1637Display_showNumberDec(&display, -5, false, 3, 0); // Expect: *-5*
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_showNumberHexEx(&display, 0xf1af, 0, false, 4, 0);        // Expect: f1Af
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberHexEx(&display, 0x2c, 0, false, 4, 0);          // Expect: __2C
	  HAL_Delay(TEST_DELAY);
	  TM1637Display_showNumberHexEx(&display, 0xd1, 0, true, 4, 0); // Expect: 00d1
	  HAL_Delay(TEST_DELAY);

	  TM1637Display_clear(&display);
	  TM1637Display_showNumberHexEx(&display, 0xd1, 0, true, 2, 0); // Expect: d1__
	  HAL_Delay(TEST_DELAY);

	  // Run through all the dots
	  for(k=0; k <= 4; k++) {
		  TM1637Display_showNumberDecEx(&display, 0, (0x80 >> k), true, 4, 0);
		  HAL_Delay(TEST_DELAY);
	  }

	  // Brightness Test
	  for(k = 0; k < 4; k++)
		  data[k] = 0xff;
	  for(k = 0; k < 7; k++) {
		  TM1637Display_setBrightness(&display, k, true);
		  TM1637Display_setSegments(&display, data, 4, 0);
		  HAL_Delay(TEST_DELAY);
	  }

	  // On/Off test
	  for(k = 0; k < 4; k++) {
		  TM1637Display_setBrightness(&display, 7, false);  // Turn off
		  TM1637Display_setSegments(&display, data, 4, 0);
		  HAL_Delay(TEST_DELAY);
		  TM1637Display_setBrightness(&display, 7, true); // Turn on
		  TM1637Display_setSegments(&display, data, 4, 0);
		  HAL_Delay(TEST_DELAY);
	  }

	  // Done!
	  TM1637Display_setSegments(&display, SEG_DONE, 4, 0);
	  while(1);


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB10 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
