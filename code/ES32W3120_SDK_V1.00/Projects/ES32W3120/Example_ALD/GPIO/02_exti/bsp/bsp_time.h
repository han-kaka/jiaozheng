#ifndef __BSP_TIME_H
#define __BSP_TIME_H

#include "ald_conf.h"

#include "bsp_common.h"

#define WXID_REQ_TIMEOUT     10
#define WAIT_WXID_TIMEOUT    500
#define RETRY_TIME           2

typedef struct {
    uint32_t time_1s_cnt;
    uint32_t uart_timeout_cnt;
    uint8_t wxid_req_cnt;
    uint16_t wait_wxid_cnt;
    uint8_t mpu6050_data_cnt;
    uint16_t adc_check_cnt;
    uint8_t led_twinkle_cnt;
    
}timer_cnt_t;

typedef struct {
    uint8_t uart_timeout_flg  :1;
    uint8_t wxid_req_flg      :1;
    uint8_t wait_wxid_flg     :1;
    uint8_t led_twinkle_flg   :1;
    uint8_t at_cmd_flg        :1;
    uint8_t reserve_flag      :3;
    
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


