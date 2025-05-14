#include "ds18b20.h"

// Initialize the DWT for microsecond delay function
void delay_us_dwt_init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;  // Enable DWT
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;             // Enable cycle counter
}

// Delay for the specified number of microseconds using DWT
void delay_us_dwt(uint32_t us) {
    uint32_t start_cycle = DWT->CYCCNT;
    uint32_t delay_cycles = (HAL_RCC_GetHCLKFreq() / 1000000) * us;
    while ((DWT->CYCCNT - start_cycle) < delay_cycles);
}

// Read temperature from DS18B20 sensor
float DS18b20_temp(void) {
    uint8_t temp_lsb, temp_msb;
    int16_t raw_temp;

    onewire_reset();
    onewire_Write(0xCC);  // Skip ROM command
    onewire_Write(0x44);  // Start temperature conversion
    HAL_Delay(750);       // Wait for conversion (typical 750ms for 12-bit resolution)

    onewire_reset();
    onewire_Write(0xCC);  // Skip ROM command
    onewire_Write(0xBE);  // Read scratchpad command

    temp_lsb = onewire_Read();
    temp_msb = onewire_Read();

    raw_temp = (temp_msb << 8) | temp_lsb;
    return (float)raw_temp / 16.0;
}

// Initialize OneWire bus and check for sensor presence
uint8_t onewire_reset(void) {
    uint8_t sensor_present = 0;

    Output_Pin(ONEWIRE_GPIO_Port, ONEWIRE_Pin);
    HAL_GPIO_WritePin(ONEWIRE_GPIO_Port, ONEWIRE_Pin, GPIO_PIN_RESET);
    delay_us_dwt(480);  // Pull low for 480µs

    Input_Pin(ONEWIRE_GPIO_Port, ONEWIRE_Pin);
    delay_us_dwt(60);   // Wait for sensor response

    if (HAL_GPIO_ReadPin(ONEWIRE_GPIO_Port, ONEWIRE_Pin) == GPIO_PIN_RESET) {
        sensor_present = 1;
    }

    delay_us_dwt(480);  // Complete the reset sequence

    return sensor_present;
}

// Write a byte to the OneWire bus
void onewire_Write(uint8_t dato) {
    for (int i = 0; i < 8; i++) {
        if (dato & (1 << i)) {
            // Write '1' bit
            Output_Pin(ONEWIRE_GPIO_Port, ONEWIRE_Pin);
            HAL_GPIO_WritePin(ONEWIRE_GPIO_Port, ONEWIRE_Pin, GPIO_PIN_RESET);
            delay_us_dwt(6);
            Input_Pin(ONEWIRE_GPIO_Port, ONEWIRE_Pin);
            delay_us_dwt(64);
        } else {
            // Write '0' bit
            Output_Pin(ONEWIRE_GPIO_Port, ONEWIRE_Pin);
            HAL_GPIO_WritePin(ONEWIRE_GPIO_Port, ONEWIRE_Pin, GPIO_PIN_RESET);
            delay_us_dwt(60);
            Input_Pin(ONEWIRE_GPIO_Port, ONEWIRE_Pin);
            delay_us_dwt(10);
        }
    }
}

// Read a byte from the OneWire bus
uint8_t onewire_Read(void) {
    uint8_t read_byte = 0;

    for (int i = 0; i < 8; i++) {
        Output_Pin(ONEWIRE_GPIO_Port, ONEWIRE_Pin);
        HAL_GPIO_WritePin(ONEWIRE_GPIO_Port, ONEWIRE_Pin, GPIO_PIN_RESET);
        delay_us_dwt(6);

        Input_Pin(ONEWIRE_GPIO_Port, ONEWIRE_Pin);
        delay_us_dwt(9);

        if (HAL_GPIO_ReadPin(ONEWIRE_GPIO_Port, ONEWIRE_Pin) == GPIO_PIN_SET) {
            read_byte |= (1 << i);
        }

        delay_us_dwt(55);
    }

    return read_byte;
}
// Configure GPIO pin as input
void Input_Pin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    uint32_t position = 0;
    uint32_t register_offset;
    volatile uint32_t *config_register;
    uint32_t temp;

    while (GPIO_Pin) {
        if (GPIO_Pin & 0x01) {
            register_offset = (position < 8) ? 0 : 1;
            config_register = &GPIOx->CRL + register_offset;

            temp = *config_register;
            temp &= ~(0x0F << ((position & 0x07) * 4));
            // Configure as input floating (CNF = 01, MODE = 00)
            temp |= (0x04 << ((position & 0x07) * 4));
            *config_register = temp;
        }
        position++;
        GPIO_Pin >>= 1;
    }
}

// Configure GPIO pin as output
void Output_Pin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    uint32_t position = 0;
    uint32_t register_offset;
    volatile uint32_t *config_register;
    uint32_t temp;

    while (GPIO_Pin) {
        if (GPIO_Pin & 0x01) {
            register_offset = (position < 8) ? 0 : 1;
            config_register = &GPIOx->CRL + register_offset;

            temp = *config_register;
            temp &= ~(0x0F << ((position & 0x07) * 4));
            // Configure as output push-pull, medium speed (CNF = 00, MODE = 01)
            temp |= (0x01 << ((position & 0x07) * 4));
            *config_register = temp;
        }
        position++;
        GPIO_Pin >>= 1;
    }
}
