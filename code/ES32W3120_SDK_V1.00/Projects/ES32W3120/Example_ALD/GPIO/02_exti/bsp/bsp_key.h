#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "ald_conf.h"

#include "bsp_common.h"

//-----------¸÷IO¶¨Òå--------------------------
#define KEY_PORT                               GPIOB
#define KEY_PIN                                GPIO_PIN_11

void key_init(void);

#endif

