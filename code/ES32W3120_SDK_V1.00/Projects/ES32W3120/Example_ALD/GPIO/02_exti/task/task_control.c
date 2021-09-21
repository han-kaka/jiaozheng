#include "app_common.h"

#include "task_common.h"
#include "task_control.h"

uint8_t contrl_task(uint8_t prio)
{
    uint8_t m_SYS_SubTask_prio=0;

    ES_LOG_PRINT("contrl_task\n");
    
    while(ga_Subtask[prio])
    {
        m_SYS_SubTask_prio= ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
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
