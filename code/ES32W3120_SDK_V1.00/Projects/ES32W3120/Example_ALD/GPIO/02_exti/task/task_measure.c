#include "bsp_mpu6050.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_time.h"
#include "bsp_system.h"

#include "app_common.h"
#include "app_ble.h"
#include "app_calculate.h"

#include "task_common.h"
#include "task_measure.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern utc_time_t utc_time;
extern system_state_t system_state;
extern uint8_t *calibrate_data_p;
extern uint16_t calibrate_packet_cnt;
extern timer_cnt_t time_cnt;
extern timer_flg_t time_flg;

uint8_t measure_task(uint8_t prio)
{
    uint8_t m_SYS_SubTask_prio = 0;
    uint16_t ax = 0;
    uint16_t ay = 0;
    uint16_t az = 0;
    uint8_t i = 0;
    uint8_t ble_send_temp[20];
    uint8_t sum = 0;
    
//    ES_LOG_PRINT("measure_task\n");
    
    while(ga_Subtask[prio])
    {
        m_SYS_SubTask_prio = ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
            case CALIBRATE_START:
            {
                calibrate_packet_cnt = 0;
                if(NULL == calibrate_data_p){
                    calibrate_data_p = (uint8_t *)malloc(15 * 50 * 20);
                }

                time_cnt.calibrate_timeout_cnt = 0;
                time_flg.calibrate_flg = 1;
                
                system_state.system_flg.calibrate_key_flg = 1;

                memset(ble_send_temp, 0, 20);
                ble_send_temp[0] = 0xaa;
                ble_send_temp[1] = 0x13;
                ble_send_temp[2] = 0xc2;
                ble_send_temp[3] = 0x02;
                
                sum = 0;
                for(i=0; i<19; i++){
                    sum += ble_send_temp[i];
                }
                ble_send_temp[19] = sum;
                
                send_ble_data(ble_send_temp, 20);
            }
                break;
            
            case CALIBRATE_STOP:
            {
                system_state.system_flg.calibrate_key_flg = 0;
                        
                time_cnt.calibrate_timeout_cnt = 0;
                time_flg.calibrate_flg = 0;

                memset(ble_send_temp, 0, 20);
                ble_send_temp[0] = 0xaa;
                ble_send_temp[1] = 0x13;
                ble_send_temp[2] = 0xc2;
                ble_send_temp[3] = 0x02;
                ble_send_temp[4] = 0x01;
                
                sum = 0;
                for(i=0; i<19; i++){
                    sum += ble_send_temp[i];
                }
                ble_send_temp[19] = sum;
                
                send_ble_data(ble_send_temp, 20);
                
                set_task(BLUETOOTH, SEND_CALIBRATE_DATA);
            }
                break;
            
            case CALIBRATE_TIMEOUT:
            {
                system_state.system_flg.calibrate_key_flg = 0;
                
                time_cnt.calibrate_timeout_cnt = 0;
                time_flg.calibrate_flg = 0;
                
                calibrate_packet_cnt = 0;
                free(calibrate_data_p);
                
                memset(ble_send_temp, 0, 20);
                ble_send_temp[0] = 0xaa;
                ble_send_temp[1] = 0x13;
                ble_send_temp[2] = 0xc2;
                ble_send_temp[3] = 0x02;
                ble_send_temp[4] = 0x02;
                
                sum = 0;
                for(i=0; i<19; i++){
                    sum += ble_send_temp[i];
                }
                ble_send_temp[19] = sum;
                
                send_ble_data(ble_send_temp, 20);
            }
                break;
            
            case ACCE_DATA:
            {
                mpu_get_accelerometer(&ax, &ay, &az);
                calculate_accelerometer(ax, ay, az);
            }
                break;
            
            case BATT_VOL:
            {

            }
                break;
            
            default:
                break;
        }
        if(clear_task(prio,m_SYS_SubTask_prio) == true)
        {
            return true;
        }
    }
    return false;
}
