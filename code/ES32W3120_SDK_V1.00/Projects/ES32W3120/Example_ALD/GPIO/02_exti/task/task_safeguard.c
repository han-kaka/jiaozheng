#include "bsp_system.h"
#include "bsp_led.h"
#include "bsp_mpu6050.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_flash.h"

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
                system_state.system_mode = E_LOW_POWER_MODE;
                lwp_mode_init();
            }
                break;
            
            case ADV_MODE:
            {
                if(0x01 == system_state.system_flg.device_init_flg){
                    if(0x01 != system_state.system_flg.mpu6050_init_flg){
                        mpu6050_set();
                    }
                    if(0x01 != system_state.system_flg.dx_bt24_t_init_flg){
                        dx_bt24_t_quick_init();
                    }
                    if(0x01 != system_state.system_flg.flash_init_flg){
                        flash_quick_init();
                    }
                    system_state.system_flg.device_init_flg = 0x00;
                }
                system_state.system_flg.imu_data_flg = 0;
                /* ¿∂µ∆…¡À∏ */
                led_twinkle();
                system_state.system_mode = E_ADV_MODE;
            }
                break;
            
            case CONNECT_MODE:
            {
                system_state.system_mode = E_CONNECT_MODE;
                
                /* ¿∂µ∆≥£¡¡ */
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

