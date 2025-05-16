#include "Max31856.h"

extern SPI_HandleTypeDef hspi1;
max31856_conversion_mode_t conversionMode;
bool initialized = false;

bool begin(void) {
  if (HAL_SPI_Init(&hspi1) == HAL_OK) {initialized = true;}
  else {initialized = false;}

  // assert on any fault
  writeRegister8(MAX31856_MASK_REG, 0x0);

  // enable open circuit fault detection
  writeRegister8(MAX31856_CR0_REG, MAX31856_CR0_OCFAULT0);

  // set cold junction temperature offset to zero
  writeRegister8(MAX31856_CJTO_REG, 0x0);

  // set Type K by default
  setThermocoupleType(MAX31856_TCTYPE_K);

  // set One-Shot conversion mode
  setConversionMode(MAX31856_ONESHOT);

  return initialized;
}

max31856_conversion_mode_t getConversionMode(void) {
  return conversionMode;
}

void setConversionMode(max31856_conversion_mode_t mode) {
  conversionMode = mode;
  uint8_t t = readRegister8(MAX31856_CR0_REG); // get current register value
  if (conversionMode == MAX31856_CONTINUOUS) {
    t |= MAX31856_CR0_AUTOCONVERT; // turn on automatic
    t &= ~MAX31856_CR0_1SHOT;      // turn off one-shot
  } else {
    t &= ~MAX31856_CR0_AUTOCONVERT; // turn off automatic
    t |= MAX31856_CR0_1SHOT;        // turn on one-shot
  }
  writeRegister8(MAX31856_CR0_REG, t); // write value back to register
}

void setThermocoupleType(max31856_thermocoupletype_t type) {
  uint8_t t = readRegister8(MAX31856_CR1_REG);
  t &= 0xF0; // mask off bottom 4 bits
  t |= (uint8_t)type & 0x0F;
  writeRegister8(MAX31856_CR1_REG, t);
}

max31856_thermocoupletype_t getThermocoupleType(void) {
  uint8_t t = readRegister8(MAX31856_CR1_REG);
  t &= 0x0F;

  return (max31856_thermocoupletype_t)(t);
}

uint8_t readFault(void) {
  return readRegister8(MAX31856_SR_REG);
}

void setColdJunctionFaultThreshholds(int8_t low,int8_t high) {
  writeRegister8(MAX31856_CJLF_REG, low);
  writeRegister8(MAX31856_CJHF_REG, high);
}

uint8_t readRegister8(uint8_t addr) {
  uint8_t ret = 0;
  readRegisterN(addr, &ret, 1);

  return ret;
}

void setNoiseFilter(max31856_noise_filter_t noiseFilter) {
  uint8_t t = readRegister8(MAX31856_CR0_REG);
  if (noiseFilter == MAX31856_NOISE_FILTER_50HZ) {
    t |= 0x01;
  } else {
    t &= 0xfe;
  }
  writeRegister8(MAX31856_CR0_REG, t);
}

void setTempFaultThreshholds(float flow, float fhigh) {
  int16_t low, high;

  flow *= 16;
  low = flow;

  fhigh *= 16;
  high = fhigh;

  writeRegister8(MAX31856_LTHFTH_REG, high >> 8);
  writeRegister8(MAX31856_LTHFTL_REG, high);

  writeRegister8(MAX31856_LTLFTH_REG, low >> 8);
  writeRegister8(MAX31856_LTLFTL_REG, low);
}

void triggerOneShot(void) {

  if (conversionMode == MAX31856_CONTINUOUS)
    return;

  uint8_t t = readRegister8(MAX31856_CR0_REG); // get current register value
  t &= ~MAX31856_CR0_AUTOCONVERT;              // turn off autoconvert
  t |= MAX31856_CR0_1SHOT;                     // turn on one-shot
  writeRegister8(MAX31856_CR0_REG, t);         // write value back to register
                                       // conversion starts when CS goes high
}

bool conversionComplete(void) {

  if (conversionMode == MAX31856_CONTINUOUS)
    return true;
  return !(readRegister8(MAX31856_CR0_REG) & MAX31856_CR0_1SHOT);
}

float readCJTemperature(void) {

  return readRegister16(MAX31856_CJTH_REG) / 256.0;
}

float readThermocoupleTemperature(void) {

  // for one-shot, make it happen
  if (conversionMode == MAX31856_ONESHOT) {
    triggerOneShot();
    uint32_t start = HAL_GetTick();
    while (!conversionComplete()) {
      if (HAL_GetTick() - start > 250) //250
        return NAN;
      HAL_Delay(10);

    }
  }

  // read the thermocouple temperature registers (3 bytes)
  int32_t temp24 = readRegister24(MAX31856_LTCBH_REG);
  // and compute temperature
  if (temp24 & 0x800000) {
    temp24 |= 0xFF000000; // fix sign
  }

  temp24 >>= 5; // bottom 5 bits are unused

  return temp24 * 0.0078125;
}

uint16_t readRegister16(uint8_t addr) {
  uint8_t buffer[2] = {0, 0};
  readRegisterN(addr, buffer, 2);

  uint16_t ret = (uint16_t)buffer[0]; // Cast to uint16_t before left shift
  ret <<= 8;
  ret |= buffer[1];

  return ret;
}

uint32_t readRegister24(uint8_t addr) {
  uint8_t buffer[3] = {0, 0, 0};
  readRegisterN(addr, buffer, 3);

  uint32_t ret = (uint32_t) buffer[0];
  ret <<= 8;
  ret |= buffer[1];
  ret <<= 8;
  ret |= buffer[2];

  return ret;
}

void readRegisterN(uint8_t addr, uint8_t buffer[], uint8_t n) {
  addr &= 0x7F; // MSB=0 for read, make sure top bit is not set

  HAL_GPIO_WritePin(GPIOA , SPI1_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &addr, 1, SPI_DELAY);
  HAL_SPI_Receive(&hspi1, buffer, n, SPI_DELAY);
  HAL_GPIO_WritePin(GPIOA , SPI1_CS_Pin, GPIO_PIN_SET);
}

void writeRegister8(uint8_t addr, uint8_t data) {
  addr |= 0x80; // MSB=1 for write, make sure top bit is set

  uint8_t buffer[2] = {addr, data};

  HAL_GPIO_WritePin(GPIOA, SPI1_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, buffer, 2, SPI_DELAY);  // Corrected size to 2
  HAL_GPIO_WritePin(GPIOA, SPI1_CS_Pin, GPIO_PIN_SET);
}
