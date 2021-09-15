#include "bsp_system.h"

#include "task_common.h"
#include "task_safeguard.h"

#include "app_common.h"

uint8_t sg_task(uint8_t prio)
{
//    RTT_PRINTF("SG_Task\r\n");

    uint8_t m_SYS_SubTask_prio=0;

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

            }
                break;
            
            case ADV_MODE:
            {
                /* 开启一些初始任务 */
                start_init_task();
            }
                break;
            
            case CONNECT_MODE:
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

