#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

#include "ald_conf.h"

#include "bsp_common.h"

typedef enum {
    E_SHUTDOWN_MODE  = 0,
    E_LOW_POWER_MODE = 1,
    E_ADV_MODE       = 2,
    E_CONNECT_MODE   = 3,

    E_MODE_MAX,
}system_mode_e;

typedef struct {
    system_mode_e system_mode;
    uint8_t wxid[4];
    
} system_state_t;

void start_init_task(void);
void init_system(void);

#endif
