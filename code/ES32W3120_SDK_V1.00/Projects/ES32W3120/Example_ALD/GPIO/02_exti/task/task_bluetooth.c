//#include "esble_comm.h"
//#include "esapp.h"

#include "bsp_dx_bt24_t.h"
#include "bsp_time.h"

#include "app_ble.h"
#include "app_common.h"

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
extern uint8_t g_rx_buf[20];
extern uint8_t ble_rx_buf[20];
extern uint8_t ble_tx_len;
extern timer_cnt_t time_cnt;
extern timer_flg_t time_flg;

uint8_t bluetooth_task(uint8_t prio)
{
    uint8_t m_SYS_SubTask_prio=0;
    uint8_t i = 0;
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
//                
//                ble_tx_buf[0] = 0x99;
//                memcpy(ble_tx_buf+1, g_rx_buf, 20);
//                memset(g_rx_buf, 0, 20);
//                send_ble_data(ble_tx_buf, 21);
                memcpy(ble_rx_buf, g_rx_buf, 20);
                memset(g_rx_buf, 0, 20);
                ble_data_decode();
            }
                break;
            
            case RET_ACK:
            {
//                memcpy(g_tx_buf, ble_tx_buf, ble_tx_len);
//                memset(ble_tx_buf, 0x00, 256);
//                g_tx_len = ble_tx_len;
//                send_ble_data();
            }
                break;
            
            case WXID_REQ:
            {
//                tx_temp[0] = 0xaa;
//                tx_temp[1] = 0x13;
//                tx_temp[2] = 0xd9;
//                tx_temp[3] = 0x02;
//                tx_temp[4] = 0x00;
//                tx_temp[5] = 0x00;
//                tx_temp[6] = 0x00;
//                tx_temp[7] = 0x00;
//                tx_temp[8] = 0x00;
//                tx_temp[9] = 0x00;
//                tx_temp[10] = 0x00;
//                tx_temp[11] = 0x00;
//                tx_temp[12] = 0x00;
//                tx_temp[13] = 0x00;
//                tx_temp[14] = 0x00;
//                tx_temp[15] = 0x00;
//                tx_temp[16] = 0x00;
//                tx_temp[17] = 0x00;
//                tx_temp[18] = 0x00;
//                tx_temp[19] = 0x98;
//                esapp_button(20, tx_temp);  //发送请求包给小程序
//                
//                //设置重发参数
//                time_cnt.wait_wxid_cnt = 0;
//                time_flg.wait_wxid_flg = 1;
//                retry_cnt++;
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
  

