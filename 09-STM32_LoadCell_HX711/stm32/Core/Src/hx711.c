#include "hx711.h"

#define hx711_delay(x)    HAL_Delay(x)
extern TIM_HandleTypeDef htim1;

// Delay function using a timer for microsecond precision
void hx711_delay_us(uint32_t us) {
    uint32_t timer_ticks = (SystemCoreClock / 1000000) * us;
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    HAL_TIM_Base_Start(&htim1);
    while (__HAL_TIM_GET_COUNTER(&htim1) < timer_ticks);
    HAL_TIM_Base_Stop(&htim1);
}

// Lock function to ensure mutual exclusion
void hx711_lock(hx711_t *hx711) {
    while (hx711->lock) {
        hx711_delay(1);
    }
    hx711->lock = 1;
}

// Unlock function to release mutual exclusion
void hx711_unlock(hx711_t *hx711) {
    hx711->lock = 0;
}

// Initialization function for HX711
void hx711_init(hx711_t *hx711, GPIO_TypeDef *clk_gpio, uint16_t clk_pin, GPIO_TypeDef *dat_gpio, uint16_t dat_pin) {
    hx711_lock(hx711);

    hx711->clk_gpio = clk_gpio;
    hx711->clk_pin = clk_pin;
    hx711->dat_gpio = dat_gpio;
    hx711->dat_pin = dat_pin;

    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Pin = clk_pin;
    HAL_GPIO_Init(clk_gpio, &gpio);

    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    gpio.Pin = dat_pin;
    HAL_GPIO_Init(dat_gpio, &gpio);

    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
    hx711_delay(10);
    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
    hx711_delay(10);

    hx711_value(hx711);
    hx711_value(hx711);

    hx711_unlock(hx711);
}

// Function to read a value from HX711
int32_t hx711_value(hx711_t *hx711) {
    uint32_t data = 0;
    uint32_t startTime = HAL_GetTick();

    while (HAL_GPIO_ReadPin(hx711->dat_gpio, hx711->dat_pin) == GPIO_PIN_SET) {
        hx711_delay(1);
        if (HAL_GetTick() - startTime > 150) {
            return 0;
        }
    }

    for (int8_t i = 0; i < 24; i++) {
        HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
        hx711_delay_us(1);
        HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
        hx711_delay_us(1);
        data = data << 1;
        if (HAL_GPIO_ReadPin(hx711->dat_gpio, hx711->dat_pin) == GPIO_PIN_SET) {
            data++;
        }
    }

    data ^= 0x800000;
    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);   
    hx711_delay_us(1);
    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
    hx711_delay_us(1);

    return data;
}

// Function to get an average value from HX711
int32_t hx711_value_ave(hx711_t *hx711, uint16_t sample) {
    hx711_lock(hx711);

    int64_t ave = 0;
    for (uint16_t i = 0; i < sample; i++) {
        ave += hx711_value(hx711);
        hx711_delay(5);
    }

    int32_t answer = (int32_t)(ave / sample);
    hx711_unlock(hx711);

    return answer;
}

// Function to tare the HX711 (set the offset)
void hx711_tare(hx711_t *hx711, uint16_t sample) {
    hx711_lock(hx711);

    int64_t ave = 0;
    for (uint16_t i = 0; i < sample; i++) {
        ave += hx711_value(hx711);
        hx711_delay(5);
    }

    hx711->offset = (int32_t)(ave / sample);
    hx711_unlock(hx711);
}

// Calibration function for HX711
void hx711_calibration(hx711_t *hx711, int32_t noload_raw, int32_t load_raw, float scale) {
    hx711_lock(hx711);

    hx711->offset = noload_raw;
    hx711->coef = (load_raw - noload_raw) / scale;

    hx711_unlock(hx711);
}

// Function to get weight using HX711
float hx711_weight(hx711_t *hx711, uint16_t sample) {
    hx711_lock(hx711);

    int64_t ave = 0;
    for (uint16_t i = 0; i < sample; i++) {
        ave += hx711_value(hx711);
        hx711_delay(5);
    }

    int32_t data = (int32_t)(ave / sample);
    float answer = (data - hx711->offset) / hx711->coef;
    hx711_unlock(hx711);

    return answer;
}

// Function to set the calibration coefficient
void hx711_coef_set(hx711_t *hx711, float coef) {
    hx711->coef = coef;
}

// Function to get the calibration coefficient
float hx711_coef_get(hx711_t *hx711) {
    return hx711->coef;
}

// Function to power down HX711
void hx711_power_down(hx711_t *hx711) {
    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_SET);
    hx711_delay(1);
}

// Function to power up HX711
void hx711_power_up(hx711_t *hx711) {
    HAL_GPIO_WritePin(hx711->clk_gpio, hx711->clk_pin, GPIO_PIN_RESET);
}
