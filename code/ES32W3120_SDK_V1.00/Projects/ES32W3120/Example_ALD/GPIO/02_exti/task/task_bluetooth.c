//#include "esble_comm.h"
//#include "esapp.h"

#include "bsp_dx_bt24_t.h"
#include "bsp_time.h"
#include "bsp_system.h"

#include "app_ble.h"
#include "app_common.h"
#include "app_calculate.h"

#include "task_bluetooth.h"
#include "task_common.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
uint8_t retry_cnt = 0;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern uint8_t g_rx_buf[UART_RX_BUF_LEN];
extern uint8_t ble_rx_buf[UART_RX_BUF_LEN];
extern timer_cnt_t time_cnt;
extern timer_flg_t time_flg;
extern uint8_t g_rx_len;
extern system_state_t system_state;
extern uint8_t mpu6050_timeout;
extern uint8_t *calibrate_data_p;
extern uint16_t calibrate_packet_cnt;


uint8_t bluetooth_task(uint8_t prio)
{
    uint8_t m_SYS_SubTask_prio=0;
    uint8_t i = 0;
    uint8_t ble_send_temp[20];
    uint8_t sum = 0;
//    uint8_t tx_temp[20];
    
    ES_LOG_PRINT("bluetooth_task\n");
    
    while(ga_Subtask[prio])
    { 
        m_SYS_SubTask_prio = ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
            case DATA_DECODE:
            {
                ES_LOG_PRINT("receive data: ");
                for(i=0; i<20; i++)
                {
                    ES_LOG_PRINT("%.2x", g_rx_buf[i]);
                }
                ES_LOG_PRINT("\n");
//                ES_LOG_PRINT("receive data: %s\n", g_rx_buf);
//                send_ble_data(g_rx_buf, 20);
                memcpy(ble_rx_buf, g_rx_buf, UART_RX_BUF_LEN);
                memset(g_rx_buf, 0, UART_RX_BUF_LEN);
                ble_data_decode();
            }
                break;
            
            case SEND_CALIBRATE_DATA:
            {
                if(NULL != calibrate_data_p){
                    for(i=0; i<calibrate_packet_cnt; i++)
                    {
                        memset(ble_send_temp, 0, 20);
                        memcpy(ble_send_temp, calibrate_data_p+i*20, 20);
                        send_ble_data(ble_send_temp, 20);
                        ald_delay_ms(20);
                    }
                }
                ald_delay_ms(50);
                memset(ble_send_temp, 0, 20);
                ble_send_temp[0] = 0xaa;
                ble_send_temp[1] = 0x13;
                ble_send_temp[2] = 0xc2;
                ble_send_temp[3] = 0x03;
                ble_send_temp[4] = 0x01;
                
                sum = 0;
                for(i=0; i<19; i++){
                    sum += ble_send_temp[i];
                }
                ble_send_temp[19] = sum;
                
                send_ble_data(ble_send_temp, 20);
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
  

