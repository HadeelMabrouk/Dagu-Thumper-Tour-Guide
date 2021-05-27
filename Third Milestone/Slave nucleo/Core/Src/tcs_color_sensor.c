#include "tcs_color_sensor.h"

int _tcs34725Initialised;
tcs34725Gain_t _tcs34725Gain;
tcs34725IntegrationTime_t _tcs34725IntegrationTime;
I2C_HandleTypeDef hi2c1;

/*!
 *  @brief  Writes a register and an 8 bit value over I2C
 *  @param  reg
 *  @param  value
 */
void write8(uint8_t reg, uint32_t value) {
    uint8_t data =value & 0xFF;
    if(HAL_I2C_Mem_Write(&hi2c1, (uint16_t)TCS34725_ADDRESS<<1, TCS34725_COMMAND_BIT | reg, 1, &data, 1, 100) != HAL_OK){
        while(1);
    }
}

/*!
 *  @brief  Reads an 8 bit value over I2C
 *  @param  reg
 *  @return value
 */
uint8_t read8(uint8_t reg) {
    uint8_t data =0;
    if(HAL_I2C_Mem_Read(&hi2c1, (uint16_t)TCS34725_ADDRESS<<1, TCS34725_COMMAND_BIT | reg, 1, &data, 1, 100) != HAL_OK){
        while(1);
    }
    return data;
}
/*!
 *  @brief  Reads a 16 bit values over I2C
 *  @param  reg
 *  @return value
 */

uint16_t read16(uint8_t reg) {
  uint16_t x;
  uint16_t t;
  uint8_t data[2];
  if(HAL_I2C_Mem_Read(&hi2c1, (uint16_t)TCS34725_ADDRESS<<1, TCS34725_COMMAND_BIT | reg, 1, data, 2, 100) != HAL_OK){
      while(1);
  }
  t = (uint16_t) data[0];
  x = (uint16_t) data[1];
  x <<= 8;
  x |= t;
  return x;
}

/*!
 *  @brief  Sets inerrupt for TCS34725
 *  @param  i
 *          Interrupt (True/False)
 */
void setInterrupt(int i) {
  uint8_t r = read8(TCS34725_ENABLE);
  if (i) {
    r |= TCS34725_ENABLE_AIEN;
  } else {
    r &= ~TCS34725_ENABLE_AIEN;
  }
  write8(TCS34725_ENABLE, r);
}

/*!
 *  @brief  Clears inerrupt for TCS34725
 */
void clearInterrupt() {
    uint8_t data = TCS34725_COMMAND_BIT | 0x66;
    if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)TCS34725_ADDRESS<<1, &data, 1, 100) != HAL_OK){
        while(1);
    }
}

/*!
 *  @brief  Enables the device
 */
void tcs_enable() {
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  HAL_Delay(3);
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
  /* Set a delay for the integration time.
    This is only necessary in the case where enabling and then
    immediately trying to read values back. This is because setting
    AEN triggers an automatic integration, so if a read RGBC is
    performed too quickly, the data is not yet valid and all 0's are
    returned */
  switch (_tcs34725IntegrationTime) {
  case TCS34725_INTEGRATIONTIME_2_4MS:
    HAL_Delay(3);
    break;
  case TCS34725_INTEGRATIONTIME_24MS:
    HAL_Delay(24);
    break;
  case TCS34725_INTEGRATIONTIME_50MS:
    HAL_Delay(50);
    break;
  case TCS34725_INTEGRATIONTIME_101MS:
    HAL_Delay(101);
    break;
  case TCS34725_INTEGRATIONTIME_154MS:
    HAL_Delay(154);
    break;
  case TCS34725_INTEGRATIONTIME_700MS:
    HAL_Delay(700);
    break;
  }
}

/*!
 *  @brief  Part of begin
 *  @return True if initialization was successful, otherwise false.
 */
int tcs_init() {

  /* Make sure we're actually connected */

  uint8_t x = read8(TCS34725_ID);
  if ((x != 0x44) && (x != 0x10)) {
    return 0;
  }
  _tcs34725Initialised = 1;

  /* Set default integration time and gain */
	/* Update the timing register */
  write8(TCS34725_ATIME, _tcs34725IntegrationTime);
	write8(TCS34725_CONTROL, _tcs34725Gain);

  /* Note: by default, the device is in power down mode on bootup */
  tcs_enable();

  return 1;
}


void getRawData_noDelay(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
  *c = read16(TCS34725_CDATAL);
  *r = read16(TCS34725_RDATAL);
  *g = read16(TCS34725_GDATAL);
  *b = read16(TCS34725_BDATAL);
}

float powf(const float x, const float y) {
  return (float)(pow((double)x, (double)y));
}

uint16_t calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b) {
  float X, Y, Z; /* RGB to XYZ correlation      */
  float xc, yc;  /* Chromaticity co-ordinates   */
  float n;       /* McCamy's formula            */
  float cct;

  if (r == 0 && g == 0 && b == 0) {
    return 0;
  }

  /* 1. Map RGB values to their XYZ counterparts.    */
  /* Based on 6500K fluorescent, 3000K fluorescent   */
  /* and 60W incandescent values for a wide range.   */
  /* Note: Y = Illuminance or lux                    */
  X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
  Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
  Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

  /* 2. Calculate the chromaticity co-ordinates      */
  xc = (X) / (X + Y + Z);
  yc = (Y) / (X + Y + Z);

  /* 3. Use McCamy's formula to determine the CCT    */
  n = (xc - 0.3320F) / (0.1858F - yc);

  /* Calculate the final CCT */
  cct =
      (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

  /* Return the results in degrees Kelvin */
  return (uint16_t)cct;
}

uint16_t calculateLux(uint16_t r, uint16_t g, uint16_t b) {
  float illuminance;

  /* This only uses RGB ... how can we integrate clear or calculate lux */
  /* based exclusively on clear since this might be more reliable?      */
  illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

  return (uint16_t)illuminance;
}
