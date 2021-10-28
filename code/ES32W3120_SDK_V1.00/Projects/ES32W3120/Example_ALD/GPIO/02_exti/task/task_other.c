#include "bsp_flash.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_system.h"

#include "app_common.h"

#include "task_common.h"
#include "task_other.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
uint8_t send_pack_temp = 0;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern uint8_t accelerometer_data_send_temp[FLASH_READ_BUFF_LEN];
extern uint8_t send_page_temp;
extern system_state_t system_state;

uint8_t other_task(uint8_t prio)
{
    uint8_t m_SYS_SubTask_prio=0;
    uint8_t send_data_temp[20];
    uint8_t sum = 0;
    uint8_t i = 0;
    
    ES_LOG_PRINT("other_task\n");
    
    while(ga_Subtask[prio])
    {   
        m_SYS_SubTask_prio = ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
            case FLASH_DATA_SEND:
            {
                send_ble_data(accelerometer_data_send_temp, FLASH_READ_BUFF_LEN);
                
                if(10 <= send_page_temp){
                    ald_delay_ms(10);
                    send_page_temp = 0;
                    
                    memset(send_data_temp, 0, 20);
                    send_data_temp[0] = 0xaa;
                    send_data_temp[1] = 0x13;
                    send_data_temp[2] = 0xc2;
                    send_data_temp[3] = 0x05;
                    send_data_temp[4] = 0x01;
                    
                    sum = 0;
                    for(i=0; i<19; i++){
                        sum += send_data_temp[i];
                    }
                    send_data_temp[19] = sum;
                    
                    send_ble_data(send_data_temp, 20);
                    system_state.system_flg.send_flash_data_flg = 1;
                }
                else{
                    set_task(MEM_READ, FLASH_READ);
                }
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



