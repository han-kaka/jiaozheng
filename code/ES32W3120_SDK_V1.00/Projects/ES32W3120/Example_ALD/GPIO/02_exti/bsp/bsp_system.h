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
    uint16_t imu_data_flg          :1;
    uint16_t mpu6050_init_flg      :1;
    uint16_t flash_init_flg        :1;
    uint16_t dx_bt24_t_init_flg    :1;
    
    uint16_t dx_bt24_t_poweron_flg :1;
    uint16_t adc_init_flg          :1;
    uint16_t motor_start_flg       :1;
    uint16_t charging_flg          :1;
    
    uint16_t charge_comp_flg       :1;
    uint16_t calibrate_mode_flg    :1;
    uint16_t calibrate_key_flg     :1;
    uint16_t send_flash_data_flg   :1;
    
    uint16_t reserve_flag          :4;
    
}system_flg_t;

typedef struct {
    uint8_t data_flag;
    uint16_t flash_data_current_page;
    uint16_t flash_data_send_page;
    
} flash_data_t;

typedef struct {
    system_mode_e system_mode;
    uint8_t shake_fre;
    uint8_t wxid[4];
    system_flg_t system_flg;
    uint8_t ble_addr[6];
    flash_data_t flash_data;
    uint8_t mpu6050_correct_flag;
    short correct_ax;
    short correct_ay;
    short correct_az;
    
}system_state_t;

void start_init_task(void);
void init_system(void);
void lwp_mode_init(void);
#endif
