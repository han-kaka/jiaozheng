#include "bsp_time.h"
#include "bsp_system.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_motor.h"

#include "app_ble.h"
#include "app_common.h"
#include "app_calculate.h"

#include "task_common.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
uint8_t ble_rx_buf[UART_RX_BUF_LEN];
uint8_t ble_tx_buf[256];
uint8_t ble_tx_len;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern utc_time_t utc_time;
extern system_state_t system_state;
extern timer_cnt_t time_cnt;
extern timer_flg_t time_flg;
extern uint32_t g_adc_result;
extern uint8_t mpu6050_timeout;
extern uint8_t *calibrate_data_p;
extern uint16_t calibrate_packet_cnt;

//extern uint8_t retry_cnt;

int ble_data_decode(void)
{
    ble_data_t *ble_data;
    uint8_t sum = 0;
    uint8_t i = 0;
    int ret = 0;
    data_utc_t *data_utc = NULL;
    data_wxid_t *data_wxid = NULL;
     
    ble_data = (ble_data_t *)ble_rx_buf;
    for(i=0; i<19; i++){
        sum += ble_rx_buf[i];
    }
    
    if(sum != ble_data->sum){
        return -1;
    }
    
    switch(ble_data->cmd){
        case SYSTEM_CMD:
            switch(ble_data->address){
                case SYSTEM_HEART_BEATS:
                    break;
                
                case SYSTEM_ACK:
                    break;
                
                case SYSTEM_RESEND:
                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
        
        case CONTROL_CMD:
            switch(ble_data->address){
                case CONTROL_CALIBRATE:
                    break;
                
                case CONTROL_SCAN:
                    break;
                
                case CONTROL_SEND_FLASH_DATA:
                    switch(ble_data->address){
                        case SEND_FLASH_DATA_START:
                            set_task(MEM_READ, FLASH_DATA_SEND);  //上传数据
                            break;
                        
                        case SEND_FLASH_DATA_DELETE:
                            set_task(MEM_WRITE, FLASH_DELETE);  //删除flash中已上传的数据
                            break;
                        
                        default:
                            ret = -1;
                            break;
                    }
                    break;
                
                case CONTROL_NO_DATA:
                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
        
        case SET_DATA_CMD:
            switch(ble_data->address){
                case SET_SHAKE_FRE:
                    system_state.shake_fre = ble_data->data[0];
                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
        
        case READ_STATE_CMD:
            switch(ble_data->address){
//                case STATE_INFO:
//                    memset(ble_tx_buf, 0, 20);
//                    ble_tx_buf[0] = 0xaa;
//                    ble_tx_buf[1] = 0x13;
//                    ble_tx_buf[2] = 0xd4;
//                    ble_tx_buf[3] = 0x01;
//                    ble_tx_buf[4] = (g_adc_result - 2900) * 100 / 4200;
//                    ble_tx_buf[5] = 0x0a;
//                    ble_tx_buf[6] = system_state.shake_fre;
//                    ble_tx_buf[7] = system_state.ble_addr[0];
//                    ble_tx_buf[8] = system_state.ble_addr[1];
//                    ble_tx_buf[9] = system_state.ble_addr[2];
//                    ble_tx_buf[10] = system_state.ble_addr[3];
//                    ble_tx_buf[11] = system_state.ble_addr[4];
//                    ble_tx_buf[12] = system_state.ble_addr[5];
//                    ble_tx_buf[13] = 0x00;
//                    ble_tx_buf[14] = 0x00;
//                    ble_tx_buf[15] = 0x00;
//                    ble_tx_buf[16] = 0x00;
//                    ble_tx_buf[17] = 0x00;
//                    ble_tx_buf[18] = 0x00;
//                    
//                    sum = 0;
//                    for(i=0; i<19; i++){
//                        sum += ble_tx_buf[i];
//                    }
//                    ble_tx_buf[19] = sum;
//                    
//                    send_ble_data(ble_tx_buf, 20);
//                    break;
//                
//                case STATE_SCAN:
//                    if(0x00 == ble_data->data[0]){
//                        system_state.system_flg.calibrate_mode_flg = 0;
//                        system_state.system_flg.calibrate_key_flg = 0;
//                        
//                        time_cnt.calibrate_timeout_cnt = 0;
//                        time_flg.calibrate_flg = 0;
//                        
//                        time_cnt.mpu6050_data_cnt = 0;
//                        mpu6050_timeout = MPU6050_NORMAL_TIMEOUT;
//                        
//                        calibrate_packet_cnt = 0;
//                        free(calibrate_data_p);
//                    }
//                    else{
//                        system_state.system_flg.calibrate_mode_flg = 1;
//                        
//                        time_cnt.mpu6050_data_cnt = 0;
//                        mpu6050_timeout = MPU6050_CALIBRATE_TIMEOUT;
//                    }
//                    
//                    memset(ble_tx_buf, 0, 20);
//                    ble_tx_buf[0] = 0xaa;
//                    ble_tx_buf[1] = 0x13;
//                    ble_tx_buf[2] = 0xc1;
//                    ble_tx_buf[3] = 0x02;
//                    ble_tx_buf[4] = 0x01;
//                    
//                    sum = 0;
//                    for(i=0; i<19; i++){
//                        sum += ble_tx_buf[i];
//                    }
//                    ble_tx_buf[19] = sum;
//                    
//                    send_ble_data(ble_tx_buf, 20);
//                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
        
        case READ_DATA_CMD:
            switch(ble_data->address){
                case DATA_MONITOR_DATA:
                    if(0x00 == ble_data->data[0]){
                        system_state.system_flg.imu_data_flg = 1;
                    }
                    break;
                
                case DATA_UTC:
                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
        
        case WRITE_DATA_CMD:
            switch(ble_data->address){
                case DATA_UTC:
                    data_utc = (data_utc_t *)ble_data->data;
                    utc_time.utc_y = data_utc->utc_y;
                    utc_time.utc_m = data_utc->utc_m;
                    utc_time.utc_d = data_utc->utc_d;
                    utc_time.utc_h = data_utc->utc_h;
                    utc_time.utc_f = data_utc->utc_f;
                    utc_time.utc_s = data_utc->utc_s;
                    utc_time.utc_5ms = data_utc->utc_5ms;
                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
            
        case WXID_CMD:
            switch(ble_data->address){
                case WXID_WRITE:
                    data_wxid = (data_wxid_t *)ble_data->data;
                    if((0x00 == system_state.wxid[0] && 0x00 == system_state.wxid[1] && 0x00 == system_state.wxid[2] && 0x00 == system_state.wxid[3]) ||
                        (0xff == system_state.wxid[0] && 0xff == system_state.wxid[1] && 0xff == system_state.wxid[2] && 0xff == system_state.wxid[3])){   //未绑定状态
                        memcpy(system_state.wxid, data_wxid, 4);
                        set_task(MEM_WRITE, WRITE_SYSTEM_INFO);
                        
                        memset(ble_tx_buf, 0, 20);
                        ble_tx_buf[0] = 0xaa;
                        ble_tx_buf[1] = 0x13;
                        ble_tx_buf[2] = 0xd9;
                        ble_tx_buf[3] = 0x03;
                        
                        sum = 0;
                        for(i=0; i<19; i++){
                            sum += ble_tx_buf[i];
                        }
                        ble_tx_buf[19] = sum;
                    }
                    else{
                        if(system_state.wxid[0] == data_wxid->wxid_0 && system_state.wxid[1] == data_wxid->wxid_1 && system_state.wxid[2] == data_wxid->wxid_2 && system_state.wxid[3] == data_wxid->wxid_3){   //已绑定状态
                            memset(ble_tx_buf, 0, 20);
                            ble_tx_buf[0] = 0xaa;
                            ble_tx_buf[1] = 0x13;
                            ble_tx_buf[2] = 0xd9;
                            ble_tx_buf[3] = 0x03;
                            
                            sum = 0;
                            for(i=0; i<19; i++){
                                sum += ble_tx_buf[i];
                            }
                            ble_tx_buf[19] = sum;
                        }
                        else{
                            memset(ble_tx_buf, 0, 20);
                            ble_tx_buf[0] = 0xaa;
                            ble_tx_buf[1] = 0x13;
                            ble_tx_buf[2] = 0xd9;
                            ble_tx_buf[3] = 0x03;
                            ble_tx_buf[4] = 0x01;
                            
                            sum = 0;
                            for(i=0; i<19; i++){
                                sum += ble_tx_buf[i];
                            }
                            ble_tx_buf[19] = sum;
                        }
                    }
                    send_ble_data(ble_tx_buf, 20);
                    
                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
            
        default:
            ret = -1;
            break;
    }
    
    return ret;
}

