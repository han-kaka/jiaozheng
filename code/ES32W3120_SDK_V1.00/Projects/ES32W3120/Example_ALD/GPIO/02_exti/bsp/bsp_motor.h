#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "ald_conf.h"

#include "bsp_common.h"

//-----------??IO????--------------------------
#define MOTOR_CTR_PORT                             GPIOA
#define MOTOR_CTR_PIN                              GPIO_PIN_12

void motor_init(void);

void motor_start(void);

void motor_stop(void);
#endif


