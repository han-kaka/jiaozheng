#ifndef __BSP_MP6050_H
#define __BSP_MP6050_H

#include "ald_conf.h"

#include "bsp_common.h"

//-----------��IO����--------------------------
#define I2C1_SCL_PORT                          GPIOB
#define I2C1_SCL_PIN                           GPIO_PIN_4      /* I2CSCL:PB4 */
#define I2C1_SDA_PORT                          GPIOB
#define I2C1_SDA_PIN                           GPIO_PIN_5      /* I2CSDA:PB5 */

#define MPU6050_INT_PORT                       GPIOA
#define MPU6050_INT_PIN                        GPIO_PIN_13
#define PWR_6050_PORT                          GPIOB
#define PWR_6050_PIN                           GPIO_PIN_10

void i2c_init(void);

void iic_write_byte(uint8_t reg, uint8_t data);

uint8_t iic_read_byte(uint8_t reg);

void MPU6050_init(void);

#endif


