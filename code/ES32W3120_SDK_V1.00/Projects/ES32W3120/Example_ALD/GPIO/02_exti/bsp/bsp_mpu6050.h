#ifndef __BSP_MP6050_H
#define __BSP_MP6050_H

#include "ald_conf.h"

#include "bsp_common.h"

//-----------¸÷IO¶¨Òå--------------------------
#define I2C1_SCL_PORT                          GPIOB
#define I2C1_SCL_PIN                           GPIO_PIN_4      /* I2CSCL:PB4 */
#define I2C1_SDA_PORT                          GPIOB
#define I2C1_SDA_PIN                           GPIO_PIN_5      /* I2CSDA:PB5 */

#define MPU6050_INT_PORT                       GPIOA
#define MPU6050_INT_PIN                        GPIO_PIN_13

void i2c_init(void);

#endif


