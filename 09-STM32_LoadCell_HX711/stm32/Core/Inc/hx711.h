#ifndef HX711_H_
#define HX711_H_


/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw
  
  Version:    1.1.1
  
  
  Reversion History:
  
  (1.1.1):
    Add power down/up.
  (1.1.0):
    Add structure, Add calibration, Add weight, change names, ...
  (1.0.0):
    First Release.
*/


  
#include "main.h"

// HX711 configuration structure
typedef struct {
    GPIO_TypeDef *clk_gpio;  // GPIO port for the clock signal
    GPIO_TypeDef *dat_gpio;  // GPIO port for the data signal
    uint16_t clk_pin;        // GPIO pin for the clock signal
    uint16_t dat_pin;        // GPIO pin for the data signal
    int32_t offset;          // Offset value for tare
    float coef;              // Calibration coefficient
    uint8_t lock;            // Lock flag for synchronization
} hx711_t;

// Function prototypes
void hx711_delay_us(uint32_t us);
void hx711_init(hx711_t *hx711, GPIO_TypeDef *clk_gpio, uint16_t clk_pin, GPIO_TypeDef *dat_gpio, uint16_t dat_pin);
int32_t hx711_value(hx711_t *hx711);
int32_t hx711_value_ave(hx711_t *hx711, uint16_t sample);
void hx711_coef_set(hx711_t *hx711, float coef);
float hx711_coef_get(hx711_t *hx711);
void hx711_calibration(hx711_t *hx711, int32_t value_noload, int32_t value_load, float scale);
void hx711_tare(hx711_t *hx711, uint16_t sample);
float hx711_weight(hx711_t *hx711, uint16_t sample);
void hx711_power_down(hx711_t *hx711);
void hx711_power_up(hx711_t *hx711);

#endif 
