#include "bsp_mpu6050.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_flash.h"
#include "bsp_power.h"
#include "bsp_time.h"
#include "bsp_motor.h"
#include "bsp_led.h"
#include "bsp_system.h"
#include "bsp_key.h"

#include "app_common.h"

#include "task_common.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
system_state_t system_state;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */

void init_system(void)
{
    /* 初始化设备信息 */
    init_system_info(&system_state);
    
//    system_state.wxid[0] = 0;
//    system_state.wxid[1] = 0;
//    system_state.wxid[2] = 0;
//    system_state.wxid[3] = 0;
    
    memset(&system_state.system_flg, 0, sizeof(system_state.system_flg));

    set_task(SG, ADV_MODE);
    
    time_init();
    
    /* 开启一些广播模式下的初始任务 */
    start_init_task();
    
    return;
}

void start_init_task(void)
{
    if(1 != system_state.system_flg.mpu6050_init_flg){
        mpu6050_init();
    }
    if(1 != system_state.system_flg.dx_bt24_t_init_flg){
        dx_bt24_t_init();
    }
    if(1 != system_state.system_flg.flash_init_flg){
        flash_init();
    }
    if(1 != system_state.system_flg.adc_init_flg){
        adc_init();
    }
    charge_init();
    motor_init();
    led_init();
    key_init();
    
    return;
}

void lwp_mode_init(void)
{
    dx_bt24_t_deinit();
    flash_deinit();
    led_close();
    mpu6050_lwp_init();
}

