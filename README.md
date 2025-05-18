# Advanced-STM32-Simulations

This repository contains professional STM32 embedded systems projects, each including Proteus circuit simulations for hardware validation. The projects demonstrate advanced peripheral configurations (USART, SPI, ADC, TIMERS) using HAL libraries with optional low-level optimization.

[![STM32CubeMX](https://img.shields.io/badge/STM32CubeMX-6.8.0-blue)](https://www.st.com/en/development-tools/stm32cubemx.html)
[![STM32CubeIDE](https://img.shields.io/badge/STM32CubeIDE-1.12.0-blue)](https://www.st.com/en/development-tools/stm32cubeide.html)
[![Proteus](https://img.shields.io/badge/Proteus-8.13-green)](https://www.labcenter.com/)

## Repository Structure

### Core Peripherals
- **1-USART_Polling_IT_DMA_MODE/** - USART communication (Polling/Interrupt/DMA)
- **2-TIMER_Periodic_IT_Counter/** - Timer interrupts & output compare
- **3-TIMER_One-Pulse_PWM/** - PWM input capture & pulse measurement
- **4-ADC_Polling_IT_DMA/** - Analog-to-Digital conversion modes
- **6-SPI_Polling_IT_DMA/** - SPI communication implementations

### Display Interfaces
- **5-Displays/** - LCD display drivers
- **9-STM32_ST7735R/** - ST7735R TFT display controller
- **10-STM32_ILI9341_TFT/** - ILI9341 TFT display interface
- **01-STM32_SSD1306_OLED/** - I2C/SPI OLED display driver
- **02-KS0801_GLCD/** - KS0108 128x64 graphical LCD
- **03-STM32_4Digit_Display/** - 7-segment display controller

### Sensors
- **05-STM32_DHT11/** - Digital temperature/humidity sensor
- **06-STM32_DHT22/** - High-accuracy climate sensor
- **07-STM32_MPX4250AP/** - Analog pressure sensor
- **08-STM32_DS18B20/** - Digital temperature sensor (1-Wire)
- **10-STM32_TMP36/** - Analog temperature sensor
- **11-STM32_Thermistor/** - NTC/PTC temperature measurement
- **12-STM32_HC-SR04/** - Ultrasonic distance sensor
- **23-STM32_APDS-9002/** - Ambient light sensor

### Data Conversion
- **11-STM32_PCP3008/** - 8-channel 10-bit ADC
- **13-STM32_ADC128S/** - 12-bit ADC interface
- **16-STM32_MCP3208/** - 12-bit SPI ADC
- **21-STM32_MCP4921/** - 12-bit DAC output
- **22-STM32_AD5621/** - 16-bit DAC interface

### Motor Control
- **13-STM32_Motors/** - Motor driver implementations
- **17-STM32_DC_MOTOR/** - PWM speed control
- **18-STM32_L298/** - H-bridge motor driver
- **19-STM32_ServoMotor/** - Servo position control
- **20-STM32_StepperMotor/** - Stepper motor driver

### Specialized Interfaces
- **04-STM32_RTC_clock/** - Real-time clock with alarms
- **09-STM32_HX711/** - Load cell amplifier
- **14-STM32_74HC595/** - Shift register LED control
- **15-UART_7SEG_74HC595/** - Serial LED display
- **24-STM32_AD8495/** - Thermocouple amplifier
- **25-STM32_MAX31855/** - Thermocouple interface
- **26-STM32_MAX31856/** - Precision thermocouple
- **27-STM32_MAX31865/** - RTD thermocouple

### Control Systems
- **12-Traffic_Intersection/** - Smart traffic light controller