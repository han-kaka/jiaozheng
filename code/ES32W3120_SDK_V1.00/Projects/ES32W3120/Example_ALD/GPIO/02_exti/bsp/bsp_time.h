#ifndef __BSP_TIME_H
#define __BSP_TIME_H

#include "ald_conf.h"

#include "bsp_common.h"

#define MPU6050_NORMAL_TIMEOUT     50
#define MPU6050_CALIBRATE_TIMEOUT  2

#define KEY_TIMEOUT          5
#define LONG_KEY_TIMEOUT     300
#define DOUBLE_KEY_TIMEOUT   50

typedef struct {
    uint32_t time_1s_cnt;
    uint32_t uart_timeout_cnt;
    uint8_t mpu6050_data_cnt;
    uint16_t adc_check_cnt;
    uint8_t led_twinkle_cnt;
    uint8_t calibrate_timeout_cnt;
    uint8_t key_cnt;
    uint8_t double_key_cnt;
    uint16_t long_key_cnt;
    
}timer_cnt_t;

typedef struct {
    uint8_t uart_timeout_flg  :1;
    uint8_t led_twinkle_flg   :1;
    uint8_t at_cmd_flg        :1;
    uint8_t calibrate_flg     :1;
    
    uint8_t key_flag          :1;
    uint8_t double_key_flag   :1;
    uint8_t long_key_flag     :1;
    uint8_t reserve_flag      :1;
    
} timer_flg_t;

typedef struct {
    uint8_t utc_y;
    uint8_t utc_m;
    uint8_t utc_d;
    uint8_t utc_h;
    uint8_t utc_f;
    uint8_t utc_s;
    uint8_t utc_5ms;
    
}utc_time_t;

void time_init(void);

#endif


