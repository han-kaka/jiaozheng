#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "ald_conf.h"

#include "bsp_common.h"

//-----------??IO????--------------------------
#define LED_RUN_PORT                               GPIOB
#define LED_RUN_PIN                                GPIO_PIN_6

void led_init(void);

void led_open(void);

void led_close(void);

void led_twinkle(void);
#endif

