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
#include "fonts.h"
#include "ST7735.h"
#include <stdio.h>
#include <string.h>
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
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
char time[30];
char date[30];
char uart_buf[30];
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// State variable to check if the configuration is done
uint8_t configComplete = 0; // Configuration status

void demoTFT(char *time, char *date) {
    ST7735_DrawString(16, 41, time, Font_16x26, ST7735_BLACK, ST7735_YELLOW);
    ST7735_DrawString(16, 81, date, Font_16x26, ST7735_YELLOW, ST7735_BLACK);

}

// Function to update the date and time on the TFT screen and via UART
void updateDateTime() {
  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  sprintf(date,"%02d/%02d/%02d",sDate.Date,sDate.Month,sDate.Year);
  sprintf(time,"%02d:%02d:%02d",sTime.Hours,sTime.Minutes,sTime.Seconds);
  demoTFT(time, date);
}


// Function to validate the date
bool validateDate(RTC_DateTypeDef *date) {
    // Array with the number of days in each month
    uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Validate month
    if (date->Month < 1 || date->Month > 12) {
        return false;
    }

    // Validate day
    if (date->Date < 1 || date->Date > daysInMonth[date->Month - 1]) {
        return false;
    }

    // The date is valid
    return true;
}

// Function to validate the time
bool validateTime(RTC_TimeTypeDef *time) {
    // Validate hours, minutes, and seconds
    if (time->Hours > 23 || time->Minutes > 59 || time->Seconds > 59) {
        return false;
    }

    // The time is valid
    return true;
}

// Function to check format of the input strings
bool checkInputFormat(const char *input, const char *format) {
    // Check for the correct length
    if (strlen(input) != strlen(format)) {
        return false;
    }

    for (size_t i = 0; i < strlen(format); i++) {
        if (format[i] == ':') {
            if (input[i] != ':') return false; // Expecting ':' in time
        } else if (format[i] == '/') {
            if (input[i] != '/') return false; // Expecting '/' in date
        } else if (!isdigit(input[i])) {
            return false; // Expecting a digit
        }
    }
    return true; // Format is correct
}

void Set_RTC_TimeAndDate(void) {
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    char uart_buf[50];  // Ensure this is appropriately sized for your messages

    while (1) {  // Loop until valid date and time are set
        // Get the time from UART
        const char *timePrompt = "Enter Time (HH:MM:SS): ";
        HAL_UART_Transmit(&huart1, (uint8_t *)timePrompt, strlen(timePrompt), HAL_MAX_DELAY);
        UART_ReceiveString(uart_buf, sizeof(uart_buf));  // Assuming uart_buf is defined

        // Validate the format of the input
        if (!checkInputFormat(uart_buf, "HH:MM:SS")) {
            const char *errorMsg = "Invalid time format. Please use HH:MM:SS\n\r";
            HAL_UART_Transmit(&huart1, (uint8_t *)errorMsg, strlen(errorMsg), HAL_MAX_DELAY);
            continue;  // Prompt for input again
        }

        // Parse the received time from uart_buf
        sTime.Hours = (uart_buf[0] - '0') * 10 + (uart_buf[1] - '0');
        sTime.Minutes = (uart_buf[3] - '0') * 10 + (uart_buf[4] - '0');
        sTime.Seconds = (uart_buf[6] - '0') * 10 + (uart_buf[7] - '0');

        // Prompt for date input
        const char *datePrompt = "Enter Date (DD/MM/YY): ";
        HAL_UART_Transmit(&huart1, (uint8_t *)datePrompt, strlen(datePrompt), HAL_MAX_DELAY);
        UART_ReceiveString(uart_buf, sizeof(uart_buf));  // Assuming uart_buf is defined

        // Validate the format of the input
        if (!checkInputFormat(uart_buf, "DD/MM/YY")) {
            const char *errorMsg = "Invalid date format. Please use DD/MM/YY\n\r";
            HAL_UART_Transmit(&huart1, (uint8_t *)errorMsg, strlen(errorMsg), HAL_MAX_DELAY);
            continue;  // Prompt for input again
        }

        // Parse the received date from uart_buf
        sDate.Date =  (uart_buf[0] - '0') * 10 +   (uart_buf[1] - '0');
        sDate.Month = (uart_buf[3] - '0') * 10 +   (uart_buf[4] - '0');
        sDate.Year =  (uart_buf[6] - '0') * 10 +   (uart_buf[7] - '0');  // Two-digit year

        // Validate date and time
        bool dateValid = validateDate(&sDate);
        bool timeValid = validateTime(&sTime);

        if (!dateValid || !timeValid) {
            const char *errorMsg = "Error in time or in date. Please try again.\n\r";
            HAL_UART_Transmit(&huart1, (uint8_t *)errorMsg, strlen(errorMsg), HAL_MAX_DELAY);
            continue;  // Prompt for input again
        }

        // If valid, set the time and date
        HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

        const char *successMsg = "RTC set successfully\n\r";
        HAL_UART_Transmit(&huart1, (uint8_t *)successMsg, strlen(successMsg), HAL_MAX_DELAY);
        sprintf(date,"Date: %02d/%02d/%02d \n\r",sDate.Date,sDate.Month,sDate.Year);
      	  sprintf(time,"time: %02d:%02d:%02d \n\r",sTime.Hours,sTime.Minutes,sTime.Seconds);
      	  HAL_UART_Transmit(&huart1, (uint8_t*)time, sizeof(time), 100);
      	  HAL_UART_Transmit(&huart1, (uint8_t*)date, sizeof(date), 100);
        break;  // Exit the loop since time and date are set successfully
    }
}

void UART_ReceiveString(char *buffer, uint16_t bufferSize) {
    uint16_t i = 0;
    char received;

    // Receive each character until '\r' is received or buffer is full
    while (i < bufferSize - 1) {
        HAL_UART_Receive(&huart1, (uint8_t *)&received, 1, HAL_MAX_DELAY);
        if (received == '\r') {  // End of input
            break;
        }
        buffer[i++] = received;
    }
    buffer[i] = '\0';  // Null-terminate the string
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
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  ST7735_Init();
  ST7735_Backlight_On();


  uint8_t r = 1;
  ST7735_SetRotation(r);
  ST7735_FillRoundRect(1,1,160,128,2,ST7735_BLUE);

  // Draw borders
  for (int x = 0; x < ST7735_GetWidth(); x++) {
      ST7735_DrawPixel(x, 0, ST7735_BLACK);
      ST7735_DrawPixel(x, ST7735_GetHeight()-1 , ST7735_BLACK);
  }
  for (int y = 0; y < ST7735_GetHeight(); y++) {
      ST7735_DrawPixel(0, y, ST7735_BLACK);
      ST7735_DrawPixel(ST7735_GetWidth() -1, y, ST7735_BLACK);
  }

  ST7735_DrawString(10, 5, "CLOCK PROJECT", Font_11x18,  ST7735_BLACK , ST7735_BLUE);


  ST7735_FillRoundRect(15, 30, 130, 40,6, ST7735_YELLOW);
  ST7735_FillRoundRect(15, 70, 130, 40,6, ST7735_BLACK);
  ST7735_DrawString(5,115, "theembeddedthings.com",Font_7x10,ST7735_YELLOW ,ST7735_BLUE);

  Set_RTC_TimeAndDate();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  updateDateTime();


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
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
