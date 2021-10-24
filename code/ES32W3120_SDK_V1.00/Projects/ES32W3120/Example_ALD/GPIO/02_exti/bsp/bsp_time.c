#include "bsp_time.h"
#include "bsp_power.h"
#include "bsp_system.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_motor.h"

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
uint8_t mpu6050_timeout = MPU6050_NORMAL_TIMEOUT;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern adc_handle_t g_h_adc;
extern uint8_t g_rx_len;
extern system_state_t system_state;
extern uint8_t key_click_flg;
extern uint8_t g_rx_buf[UART_RX_BUF_LEN];

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
        
        if(1 == time_flg.calibrate_flg){
            time_cnt.calibrate_timeout_cnt++;
            if(15 <= time_cnt.calibrate_timeout_cnt){
                time_cnt.calibrate_timeout_cnt = 0;
                time_flg.calibrate_flg = 1;
                set_task(MEASURE, CALIBRATE_TIMEOUT);
            }
        }
        
        /* utc时间计算 */
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
    
//    串口数据读取
    if(1 == time_flg.uart_timeout_flg){
        time_cnt.uart_timeout_cnt++;
        if(2 <= time_cnt.uart_timeout_cnt){
            time_cnt.uart_timeout_cnt = 0;
            time_flg.uart_timeout_flg = 0;
            g_rx_len = 0;
            if(0 == system_state.system_flg.dx_bt24_t_poweron_flg){
                if(NULL != strstr((const char*)g_rx_buf, "Power On")){
                    system_state.system_flg.dx_bt24_t_poweron_flg = 1;
                }
            }
            else{
                if(1 == system_state.system_flg.dx_bt24_t_init_flg){
                    set_task(BLUETOOTH, DATA_DECODE);
                }
                else{
                    time_flg.at_cmd_flg = 1;
                }
            }
            
        }
    }
    
//    6050数据读取和电池电量读取
    if((E_ADV_MODE == system_state.system_mode) || (E_CONNECT_MODE == system_state.system_mode)){
        if(1 == system_state.system_flg.mpu6050_init_flg){
            time_cnt.mpu6050_data_cnt++;
            if(mpu6050_timeout <= time_cnt.mpu6050_data_cnt){
                time_cnt.mpu6050_data_cnt = 0;
                set_task(MEASURE, ACCE_DATA);
            }
        }
        if(1 == system_state.system_flg.adc_init_flg){
            time_cnt.adc_check_cnt++;
            if(30000 <= time_cnt.adc_check_cnt){
                time_cnt.adc_check_cnt = 0;
                /* Start normal convert, enable interrupt */
                ald_adc_normal_start_by_it(&g_h_adc);
            }
        }
    }

//    led闪烁
    if(1 == time_flg.led_twinkle_flg){
        time_cnt.led_twinkle_cnt++;
        if(50 == time_cnt.led_twinkle_cnt){
            ald_gpio_write_pin(LED_RUN_PORT, LED_RUN_PIN, 1);
        }
        else if(100 == time_cnt.led_twinkle_cnt){
            time_cnt.led_twinkle_cnt = 0;
            ald_gpio_write_pin(LED_RUN_PORT, LED_RUN_PIN, 0);
        }
    }
    
    
    if(1 == time_flg.key_flag){
        time_cnt.key_cnt++;
        if(KEY_TIMEOUT == time_cnt.key_cnt){
            time_flg.key_flag = 0;
            time_cnt.key_cnt = 0;
            if(0 == ald_gpio_read_pin(KEY_PORT, GPIO_PIN_11)){
                if(1 == key_click_flg){
//                    双击
                    ES_LOG_PRINT("double key\n");
                    if(1 == system_state.system_flg.calibrate_mode_flg){
                        if(1 == system_state.system_flg.calibrate_key_flg){
                            set_task(MEASURE, CALIBRATE_STOP);
                        }
                        else{
                            set_task(MEASURE, CALIBRATE_START);
                        }
                    }
                }
                else{
//                    单击按下
                    time_cnt.long_key_cnt = 0;
                    time_flg.long_key_flag = 1;
                }
                
            }
            else{
                if(1 == key_click_flg){
                    key_click_flg = 0;
                }
                else{
                    key_click_flg = 1;
                    time_cnt.double_key_cnt = 0;
                    time_flg.double_key_flag = 1;
                }
                time_cnt.long_key_cnt = 0;
                time_flg.long_key_flag = 0;
            }
            
            __NVIC_EnableIRQ(EXTI11_IRQn);
        }
    }
    
//    长按判断
    if(1 == time_flg.long_key_flag){
        time_cnt.long_key_cnt++;
        if(LONG_KEY_TIMEOUT <= time_cnt.long_key_cnt){
            time_cnt.long_key_cnt = 0;
            time_flg.long_key_flag = 0;
//            3秒长按
            ES_LOG_PRINT("long key\n");
        }
    }
    
//    双击判断
    if(1 == time_flg.double_key_flag){
        time_cnt.double_key_cnt++;
        if(DOUBLE_KEY_TIMEOUT <= time_cnt.double_key_cnt){
            time_cnt.double_key_cnt = 0;
            time_flg.double_key_flag = 0;
            key_click_flg = 0;
        }
        
    }
    
//    马达低频震动
    if(1 == time_flg.motor_flag){
        time_cnt.motor_cnt++;
        if(LOW_SHAKE_FRE_HIGH == time_cnt.motor_cnt){
            ald_gpio_write_pin(MOTOR_CTR_PORT, MOTOR_CTR_PIN, 0);
        }
        else if(LOW_SHAKE_FRE_LOW == time_cnt.motor_cnt){
            time_cnt.motor_cnt = 0;
            ald_gpio_write_pin(MOTOR_CTR_PORT, MOTOR_CTR_PIN, 1);
        }
    }
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


