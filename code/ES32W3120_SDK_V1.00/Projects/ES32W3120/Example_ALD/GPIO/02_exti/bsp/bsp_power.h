#ifndef __BSP_POWER_H
#define __BSP_POWER_H

#include "ald_conf.h"

#include "bsp_common.h"

//-----------¸÷IO¶¨Òå--------------------------
#define ADC_CHANNEL5_PORT           GPIOA
#define ADC_CHANNEL5_PIN            GPIO_PIN_7
#define PWR_ADC_PORT                GPIOB
#define PWR_ADC_PIN                 GPIO_PIN_7

#define CHARGE_G_PORT               GPIOB
#define CHARGE_G_PIN                GPIO_PIN_3
#define CHARGE_Y_PORT               GPIOB
#define CHARGE_Y_PIN                GPIO_PIN_2

void adc_init(void);

void charge_init(void);
#endif


