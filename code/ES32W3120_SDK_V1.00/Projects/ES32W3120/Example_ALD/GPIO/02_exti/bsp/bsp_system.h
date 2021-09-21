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
    uint8_t imu_data_flg          :1;
    uint8_t mpu6050_init_flg      :1;
    uint8_t flash_init_flg        :1;
    uint8_t dx_bt24_t_init_flg    :1;
    uint8_t adc_init_flg          :1;
    
    uint8_t reserve_flag          :3;
    
}system_flg_t;

typedef struct {
    system_mode_e system_mode;
    uint8_t shake_fre;
    uint8_t wxid[4];
    system_flg_t system_flg;
    
}system_state_t;

void start_init_task(void);
void init_system(void);

#endif
