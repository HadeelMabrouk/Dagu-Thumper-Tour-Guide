#ifndef TCS_COLOR_SENSOR
#define TCS_COLOR_SENSOR

#include "stdio.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "stm32l4xx_hal.h"

/** Integration time settings for TCS34725 */
typedef enum {
  TCS34725_INTEGRATIONTIME_2_4MS =
      0xFF, /**<  2.4ms - 1 cycle    - Max Count: 1024  */
  TCS34725_INTEGRATIONTIME_24MS =
      0xF6, /**<  24ms  - 10 cycles  - Max Count: 10240 */
  TCS34725_INTEGRATIONTIME_50MS =
      0xEB, /**<  50ms  - 20 cycles  - Max Count: 20480 */
  TCS34725_INTEGRATIONTIME_101MS =
      0xD5, /**<  101ms - 42 cycles  - Max Count: 43008 */
  TCS34725_INTEGRATIONTIME_154MS =
      0xC0, /**<  154ms - 64 cycles  - Max Count: 65535 */
  TCS34725_INTEGRATIONTIME_700MS =
      0x00 /**<  700ms - 256 cycles - Max Count: 65535 */
} tcs34725IntegrationTime_t;

/** Gain settings for TCS34725  */
typedef enum {
  TCS34725_GAIN_1X = 0x00,  /**<  No gain  */
  TCS34725_GAIN_4X = 0x01,  /**<  4x gain  */
  TCS34725_GAIN_16X = 0x02, /**<  16x gain */
  TCS34725_GAIN_60X = 0x03  /**<  60x gain */
} tcs34725Gain_t;

#define TCS34725_ADDRESS (0x29)     /**< I2C address **/
#define TCS34725_COMMAND_BIT (0x80) /**< Command bit **/
#define TCS34725_ENABLE (0x00)      /**< Interrupt Enable register */
#define TCS34725_ENABLE_AIEN (0x10) /**< RGBC Interrupt Enable */
#define TCS34725_PERS  (0x0C) /**< Persistence register - basic SW filtering mechanism for          \
            interrupts */
#define TCS34725_PERS_NONE (0b0000) /**< Every RGBC cycle generates an interrupt */
	
#define TCS34725_CDATAL (0x14) /**< Clear channel data low byte */
#define TCS34725_CDATAH (0x15) /**< Clear channel data high byte */
#define TCS34725_RDATAL (0x16) /**< Red channel data low byte */
#define TCS34725_RDATAH (0x17) /**< Red channel data high byte */
#define TCS34725_GDATAL (0x18) /**< Green channel data low byte */
#define TCS34725_GDATAH (0x19) /**< Green channel data high byte */
#define TCS34725_BDATAL (0x1A) /**< Blue channel data low byte */
#define TCS34725_BDATAH (0x1B) /**< Blue channel data high byte */
#define TCS34725_ID                                                            \
  (0x12) /**< 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727 */
#define TCS34725_ATIME (0x01) /**< Integration time */
#define TCS34725_CONTROL (0x0F) /**< Set the gain level for the sensor */
#define TCS34725_ENABLE_PON                                                    \
  (0x01) /**< Power on - Writing 1 activates the internal oscillator, 0        \
            disables it */
#define TCS34725_ENABLE_AEN                                                    \
  (0x02) /**< RGBC Enable - Writing 1 actives the ADC, 0 disables it */
	
	
void write8(uint8_t reg, uint32_t value);
uint8_t read8(uint8_t reg);
uint16_t read16(uint8_t reg);
void setInterrupt(int i);
void clearInterrupt(void);
void tcs_enable(void);
int tcs_init(void);
void getRawData_noDelay(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
float powf(const float x, const float y) ;
uint16_t calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b);
uint16_t calculateLux(uint16_t r, uint16_t g, uint16_t b);

#endif
