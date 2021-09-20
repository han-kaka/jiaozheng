#include "bsp_time.h"
#include "bsp_power.h"
#include "bsp_system.h"

#include "app_common.h"

#include "task_common.h"


/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
timer_handle_t g_ad16c4t_init;
timer_clock_config_t g_ad16c4t_clock_config;
timer_cnt_t time_cnt = {0};
timer_flg_t time_flg = {0};
utc_time_t utc_time = {0};

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern adc_handle_t g_h_adc;
extern uint8_t g_rx_len;
extern system_state_t ststem_state;
//extern uint8_t retry_cnt;

/**
  * @brief  ald timer period elapsed callback
  * @param  arg: Pointer to timer_handle_t structure.
  * @retval None.
  */
void ald_timer_period_elapsed_callback(struct timer_handle_s *arg)
{
    uint8_t utc_day_dif = 31;
    
    time_cnt.time_1s_cnt++;
    if(100 <= time_cnt.time_1s_cnt){
        /* 1s 定时 */
        time_cnt.time_1s_cnt = 0;
        utc_time.utc_s++;
        if(60 <= utc_time.utc_s){
            utc_time.utc_s = 0;
            utc_time.utc_f++;
            if(60 <= utc_time.utc_f){
                utc_time.utc_f = 0;
                utc_time.utc_h++;
                if(24 <= utc_time.utc_h){
                    utc_time.utc_h = 0;
                    utc_time.utc_d++;
                    switch(utc_time.utc_m){
                        case 1:
                        case 3:
                        case 5:
                        case 7:
                        case 8:
                        case 10:
                        case 12:
                            utc_day_dif = 31;//31天的月份
                            break;
                        case 4:
                        case 6:
                        case 9:
                        case 11:
                            utc_day_dif = 30;//30天的月份
                            break;
                        case 2:
                            break;
                    }
                    if(utc_time.utc_d >= utc_day_dif){
                        utc_time.utc_m++;
                        utc_time.utc_d = 1;
                        if(12 <= utc_time.utc_m){
                            utc_time.utc_y++;
                            utc_time.utc_m = 1;
                            if(255 <= utc_time.utc_y){
                                utc_time.utc_y = 21;
                            }
                        }
                    }
                }
            }
            ES_LOG_PRINT("utc_y:%u, utc_m:%u, utc_d:%u, utc_h:%u, utc_f:%u\n", utc_time.utc_y, utc_time.utc_m, utc_time.utc_d, utc_time.utc_h, utc_time.utc_f);
        }
//        ES_LOG_PRINT("utc_y:%u, utc_m:%u, utc_d:%u, utc_h:%u, utc_f:%u, utc_s:%u\n", utc_time.utc_y, utc_time.utc_m, utc_time.utc_d, utc_time.utc_h, utc_time.utc_f, utc_time.utc_s);
//        /* Start normal convert, enable interrupt */
//        ald_adc_normal_start_by_it(&g_h_adc);
    }
    
    if(1 == time_flg.uart_timeout_flg){
        time_cnt.uart_timeout_cnt++;
        if(2 <= time_cnt.uart_timeout_cnt){
            time_cnt.uart_timeout_cnt = 0;
            time_flg.uart_timeout_flg = 0;
            g_rx_len = 0;
            set_task(BLUETOOTH, DATA_DECODE);
        }
    }
    
    if((E_ADV_MODE == ststem_state.system_mode) || (E_CONNECT_MODE == ststem_state.system_mode)){
        time_cnt.mpu6050_data_cnt++;
        if(5 <= time_cnt.uart_timeout_cnt){
            time_cnt.mpu6050_data_cnt = 0;
            set_task(MEASURE, IMU_DATA);
        }
    }

//    //ble 连上后延迟100ms发送请求包
//    if(1 == time_flg.wxid_req_flg){
//        time_cnt.wxid_req_cnt++;
//        if(WXID_REQ_TIMEOUT <= time_cnt.wxid_req_cnt){
//            time_flg.wxid_req_flg = 0;
//            time_cnt.wxid_req_cnt = 0;
//            set_task(BLUETOOTH, WXID_REQ);  //发送请求包
//        }
//    }
//    
//    //请求包重发机制
//    if(1 == time_flg.wait_wxid_flg){
//        time_cnt.wait_wxid_cnt++;
//        if(WAIT_WXID_TIMEOUT <= time_cnt.wait_wxid_cnt){
//            time_flg.wait_wxid_flg = 0;
//            time_cnt.wait_wxid_cnt = 0;
//            if(RETRY_TIME >= retry_cnt){
//                set_task(BLUETOOTH, WXID_REQ);  //请求包重发
//            }
//            else{
//                retry_cnt = 0;
//                set_task(CONTROL, BLE_DISCON);  //主动断开ble连接
//            }
//            
//        }
//    }
}

void time_init(void)
{
    memset(&time_cnt, 0x0, sizeof(time_cnt));
    
    /* Initialize AD16C4T1 */
    memset(&g_ad16c4t_init, 0x0, sizeof(g_ad16c4t_init));  /* initialize the g_ad16c4t_init */
    g_ad16c4t_init.perh = AD16C4T1;
    g_ad16c4t_init.init.prescaler    = 48 - 1;            /* clk_count: 1MHz */
    g_ad16c4t_init.init.mode         = TIMER_CNT_MODE_UP;  /* count up */
    g_ad16c4t_init.init.period       = 1000 - 1;           /* period is 1000 count clock */
    g_ad16c4t_init.init.clk_div      = TIMER_CLOCK_DIV1;   /* working clock of dead time and filter */
    g_ad16c4t_init.init.re_cnt       = 10 - 1;             /* 10 repeat count */
    g_ad16c4t_init.period_elapse_cbk = ald_timer_period_elapsed_callback;  /* updata period callback function */
    ald_timer_base_init(&g_ad16c4t_init);

    /* Initialize clock source */
    g_ad16c4t_clock_config.source = TIMER_SRC_INTER;   /**< internal clock sources */
    ald_timer_config_clock_source(&g_ad16c4t_init, &g_ad16c4t_clock_config);

    ald_mcu_irq_config(AD16C4T1_UP_IRQn, 0, 0, ENABLE);/* Enable AD16C4T1 interrupt */
    ald_timer_base_start_by_it(&g_ad16c4t_init);       /* Start UPDATE interrupt by interrupt */
    
    return;
}


