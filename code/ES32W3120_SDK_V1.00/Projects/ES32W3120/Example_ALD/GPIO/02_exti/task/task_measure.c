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

uint8_t measure_task(uint8_t prio)
{
    uint8_t m_SYS_SubTask_prio = 0;
    uint16_t ax = 0;
    uint16_t ay = 0;
    uint16_t az = 0;
    
    
    ES_LOG_PRINT("measure_task\n");
    
    while(ga_Subtask[prio])
    {
        m_SYS_SubTask_prio = ga_TaskMapTable[ga_Subtask[prio]];
        switch(m_SYS_SubTask_prio)
        {
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
