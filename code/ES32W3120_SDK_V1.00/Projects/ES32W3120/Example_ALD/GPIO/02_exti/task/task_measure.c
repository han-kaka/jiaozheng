#include "bsp_mpu6050.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_time.h"

#include "task_common.h"
#include "task_measure.h"

#include "app_common.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern utc_time_t utc_time;;

uint8_t measure_task(uint8_t prio)
{
//    RTT_PRINTF("Measure_Task\r\n");

    uint8_t m_SYS_SubTask_prio = 0;
    uint16_t ax = 0;
    uint16_t ay = 0;
    uint16_t az = 0;
    uint8_t tx_buf_temp[20];
    uint8_t sum = 0;
    uint8_t i = 0;
    
    while(ga_Subtask[prio])
    {
        m_SYS_SubTask_prio = ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
            case IMU_DATA:
            {
                mpu_get_accelerometer(&ax, &ay, &az);
                
                    memset(tx_buf_temp, 0, 20);
                    tx_buf_temp[0] = 0xaa;
                    tx_buf_temp[1] = 0x13;
                    tx_buf_temp[2] = 0xd5;
                    tx_buf_temp[3] = 0x01;
                    tx_buf_temp[4] = ax >> 8;
                    tx_buf_temp[5] = ax & 0xff;
                    tx_buf_temp[6] = ay >> 8;
                    tx_buf_temp[7] = ay & 0xff;
                    tx_buf_temp[8] = az >> 8;
                    tx_buf_temp[9] = az & 0xff;
                    tx_buf_temp[10] = utc_time.utc_y;
                    tx_buf_temp[11] = utc_time.utc_m;
                    tx_buf_temp[12] = utc_time.utc_d;
                    tx_buf_temp[13] = utc_time.utc_h;
                    tx_buf_temp[14] = utc_time.utc_f;
                    tx_buf_temp[15] = utc_time.utc_s;
                    tx_buf_temp[16] = 0;
                    tx_buf_temp[17] = 0;
                    tx_buf_temp[18] = 0;
                
                    sum = 0;
                    for(i=0; i<19; i++){
                        sum += tx_buf_temp[i];
                    }
                    tx_buf_temp[19] = sum;
                    
                    send_ble_data(tx_buf_temp, 20);
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
