#include "bsp_time.h"
#include "bsp_system.h"

#include "app_ble.h"
#include "app_common.h"

#include "task_common.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
uint8_t ble_rx_buf[20];
uint8_t ble_tx_buf[256];
uint8_t ble_tx_len;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern utc_time_t utc_time;;
//extern system_state_t ststem_state;
//extern timer_cnt_t time_cnt;
//extern timer_flg_t time_flg;
//extern uint8_t retry_cnt;

int ble_data_decode(void)
{
    uint8_t data_len = sizeof(ble_rx_buf);
    ble_data_t *ble_data;
    uint8_t sum = 0;
    uint8_t i = 0;
    int ret = 0;
    data_utc_t *data_utc = NULL;
//    data_wxid_t *data_wxid = NULL;
    
    if(20 != data_len){
        return -1;
    }
    
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
                case SET_FREQUENCY:
                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
        
        case READ_STATE_CMD:
            switch(ble_data->address){
                case STATE_INFO:
                    break;
                
                case STATE_SCAN:
                    break;
                
                default:
                    ret = -1;
                    break;
            }
            break;
        
        case READ_DATA_CMD:
            switch(ble_data->address){
                case DATA_MONITOR_DATA:
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
            
//        case WXID_CMD:
//            switch(ble_data->address){
//                case WXID_WRITE:
//                    //重置重发机制
//                    time_cnt.wait_wxid_cnt = 0;
//                    time_flg.wait_wxid_flg = 0;
//                    retry_cnt = 0;
//                
//                    data_wxid = (data_wxid_t *)ble_data->data;
//                    if(ststem_state.wxid[0] == 0xff && ststem_state.wxid[1] == 0xff && ststem_state.wxid[2] == 0xff && ststem_state.wxid[3] == 0xff){
//                        //保存wxid置flash中
//                        ststem_state.wxid[0] = data_wxid->wxid_0;
//                        ststem_state.wxid[1] = data_wxid->wxid_1;
//                        ststem_state.wxid[2] = data_wxid->wxid_2;
//                        ststem_state.wxid[3] = data_wxid->wxid_3;
//                        set_task(MEM_WRITE, FLASH_WXID_WRITE);
//                    }
//                    else{
//                        if(ststem_state.wxid[0] == data_wxid->wxid_0 && ststem_state.wxid[1] == data_wxid->wxid_1 && ststem_state.wxid[2] == data_wxid->wxid_2 && ststem_state.wxid[3] == data_wxid->wxid_3){
//                            if(BIND_FLG != data_wxid->bind_flg){
//                                set_task(CONTROL, BLE_DISCON);  //主动断开ble连接
//                                
//                                //重置flash中的wxid
//                                ststem_state.wxid[0] = 0xff;
//                                ststem_state.wxid[1] = 0xff;
//                                ststem_state.wxid[2] = 0xff;
//                                ststem_state.wxid[3] = 0xff;
//                                set_task(MEM_WRITE, FLASH_WXID_WRITE);
//                            }
//                        }
//                        else{
//                            set_task(CONTROL, BLE_DISCON);  //主动断开ble连接
//                        }
//                    }
//                    
//                    break;
//                
//                default:
//                    ret = -1;
//                    break;
//            }
//            break;
            
        default:
            ret = -1;
            break;
    }
    
    return ret;
}

