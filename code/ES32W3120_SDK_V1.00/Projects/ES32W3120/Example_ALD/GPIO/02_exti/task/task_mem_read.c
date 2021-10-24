#include "bsp_flash.h"

#include "app_common.h"

#include "task_common.h"
#include "task_mem_read.h"

uint8_t mem_read_task(uint8_t prio) 
{
    uint8_t m_SYS_SubTask_prio=0;

    ES_LOG_PRINT("mem_read_task\n");
    
    while(ga_Subtask[prio]) 
    {
        m_SYS_SubTask_prio = ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
            case FLASH_READ:
            {
                if(0 != read_accelerometer_data()){
                    return false;
                }
                else{
                    set_task(OTHER, FLASH_DATA_SEND);
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

