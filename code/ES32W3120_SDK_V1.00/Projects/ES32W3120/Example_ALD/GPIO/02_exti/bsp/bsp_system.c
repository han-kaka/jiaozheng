#include "bsp_mpu6050.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_flash.h"
#include "bsp_power.h"
#include "bsp_time.h"
#include "bsp_motor.h"
#include "bsp_led.h"
#include "bsp_system.h"

#include "app_common.h"

#include "task_common.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
system_state_t ststem_state;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */

void init_system(void)
{ 
    ststem_state.system_mode = E_ADV_MODE;
    ststem_state.system_flg.imu_data_flg = 0;
    
    /* 初始化设备信息 */
    init_system_info(&ststem_state);
    
    set_task(SG, ADV_MODE);
    return;
}

void start_init_task(void)
{
    mpu6050_init();
    dx_bt24_t_init();
    flash_init();
    adc_init();
    charge_init();
    motor_init();
    led_init();
    
    time_init();
    
    return;
}



