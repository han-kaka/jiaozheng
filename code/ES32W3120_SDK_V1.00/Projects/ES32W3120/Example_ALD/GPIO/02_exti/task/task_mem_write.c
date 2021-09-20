#include "bsp_flash.h"

#include "app_common.h"

#include "task_common.h"
#include "task_mem_write.h"

uint8_t mem_write_task(uint8_t prio)
{   
    uint8_t m_SYS_SubTask_prio=0;

    ES_LOG_PRINT("mem_write_task\n");
    
    while(ga_Subtask[prio]) 
    {
        m_SYS_SubTask_prio = ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
            case WRITE_SYSTEM_INFO:
            {
                save_system_info();
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

