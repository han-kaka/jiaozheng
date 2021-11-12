#include "bsp_system.h"
#include "bsp_led.h"

#include "app_common.h"

#include "task_common.h"
#include "task_safeguard.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern system_state_t system_state;

uint8_t sg_task(uint8_t prio)
{
    uint8_t m_SYS_SubTask_prio=0;
    
    ES_LOG_PRINT("sg_task\n");
    
    while(ga_Subtask[prio])
    {
        m_SYS_SubTask_prio = ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
            case SHUTDOWN_MODE:
            {

            }
                break;
            
            case LOW_POWER_MODE:
            {
                lwp_mode_init();
            }
                break;
            
            case ADV_MODE:
            {
                system_state.system_mode = E_ADV_MODE;
                system_state.system_flg.imu_data_flg = 0;
                /* 蓝灯闪烁 */
                led_twinkle();
                
//                /* 开启一些广播模式下的初始任务 */
//                start_init_task();
            }
                break;
            
            case CONNECT_MODE:
            {
                system_state.system_mode = E_CONNECT_MODE;
                
                /* 蓝灯常亮 */
                led_open();
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

