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
#define PWR_6050_PORT                          GPIOB
#define PWR_6050_PIN                           GPIO_PIN_10

void mpu_get_accelerometer(short *ax, short *ay, short *az);

void mpu6050_init(void);

void mpu6050_quick_init(void);

void mpu6050_set(void);

void mpu6050_int_set(void);

void mpu6050_int_init(void);
#endif


