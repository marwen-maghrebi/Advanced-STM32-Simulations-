# Advanced-STM32-Simulations-

This repository contains professional STM32 embedded systems projects, each including Proteus circuit simulations for hardware validation. The projects demonstrate advanced peripheral configurations (USART, SPI, ADC, TIMERS) using HAL libraries with optional low-level optimization.

[STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) &&
[STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.) &&
[Proteus 8](https://www.labcenter.com/) 

## Repository  Structure

The repository is organized as follows:

- **1-USART_Polling_IT_DMA_MODE/** - USART communication implementation using polling, interrupt, and DMA modes  
- **2-TIMER_Periodic_IT_Counter_Output_Compare_MODE/** - TIMER module for periodic interrupts and output compare operations  
- **3-TIMER_One-Pulse_PWM_Input_Capture_MODE/** - TIMER in One-Pulse PWM Input Capture mode for signal duration measurement  
- **4-ADC_Polling_IT_DMA/** - ADC module implementation in polling, interrupt, and DMA modes  
- **5-Displays/** - Interface for various display types including LCD  
- **6-SPI_Polling_IT_DMA_MODE/** - SPI module implementation in polling, interrupt, and DMA modes  
- **7-SPI_TC72_Temperature_Sensor/** - SPI interface with TC72 digital temperature sensor  
- **8-SPI_MATRIX_7SEG/** - SPI-driven matrix or 7-segment display control  
- **9-STM32_ST7735R/** - STM32 with ST7735R TFT display module interface  
- **10-STM32_ILI9341_TFT/** - STM32 with ILI9341 TFT display module  
- **11-STM32_PCP3008/** - 8-channel 10-bit ADC (PCP3008) integration  
- **12-Traffic_Intersection_Controller/** - STM32-based traffic light control system  
- **13-STM32_Motors/** - Motor control with L293D/L298N drivers using timers and ADC  
- **01-STM32_SSD1306_OLED/** - OLED display driver (I2C/SPI interface)  
- **02-KS0801_GLCD/** - Graphical LCD controller (128x64 resolution)  
- **03-STM32_4Digit_Display/** - 4-digit 7-segment display driver  
- **04-STM32_RTC_dock/** - Real-time clock implementation with alarm functions  
- **05-STM32_DHT11/** - Temperature and humidity sensor interface  
- **06-STM32_DHT22/** - High-precision climate sensor interface  
- **07-STM32_MPX4250AP_PressureGauge/** - Air pressure measurement system  
- **08-STM32_D518820/** - Digital sensor interface implementation  
- **09-STM32_LoadCell_HX711/** - Weight measurement system with load cell  
- **10-STM32_TMP36/** - Analog temperature sensor interface  
- **11-STM32_Thermistor/** - NTC/PTC thermistor temperature measurement  
- **12-STM32_HC-500d_Ultrasonic/** - Ultrasonic distance measurement sensor  
- **13-STM32_ADC1285/** - 12-bit ADC data acquisition system  
- **14-STM32_74HC595_LedBar/** - LED control using 74HC595 shift register  
- **15-UART_75EG_74HC595/** - UART-controlled LED driver  
- **16-STM32_MCP3208/** - 12-bit SPI ADC interface  
- **17-STM32_DC_MOTOR/** - PWM-based DC motor speed control  
- **18-STM32_L298_MotorDriver/** - H-bridge motor controller implementation  
- **19-STM32_ServoMotor/** - Precision servo motor control  
- **20-STM32_StepperMotor/** - Stepper motor driver system  
- **21-STM32_MCP4921/** - 12-bit digital-to-analog converter interface  
- **22-STM32_AD5621/** - 16-bit DAC implementation  
- **23-STM32_APDS-9002/** - Ambient light sensing system  
- **24-STM32_Thermocouple_ADR495/** - Thermocouple temperature measurement  
- **25-STM32_Thermocouple_MAX31855/** - Thermocouple interface with MAX31855  
- **26-STM32_Thermocouple_MAX31856/** - Precision thermocouple interface  
- **27-STM32_Thermocouple_MAX31865/** - RTD thermocouple measurement system  