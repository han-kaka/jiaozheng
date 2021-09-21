#include "bsp_motor.h"
#include "bsp_system.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern system_state_t system_state;

void motor_init(void)
{
    gpio_init_t x;

    x.mode = GPIO_MODE_OUTPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_1;

    ald_gpio_init(MOTOR_CTR_PORT, MOTOR_CTR_PIN, &x);
    ald_gpio_write_pin(MOTOR_CTR_PORT, MOTOR_CTR_PIN, 1);
}

void motor_start(void)
{
    system_state.system_flg.motor_start_flg = 1;
    
    if(0x00 == system_state.shake_fre){
        /* 不震动 */
    }
    else if(0x01 == system_state.shake_fre){
        /* 低频震动 */
        ald_gpio_write_pin(MOTOR_CTR_PORT, MOTOR_CTR_PIN, 0);
    }
    else if(0x02 == system_state.shake_fre){
        /* 高频震动 */
        ald_gpio_write_pin(MOTOR_CTR_PORT, MOTOR_CTR_PIN, 0);
    }
}

void motor_stop(void)
{
    system_state.system_flg.motor_start_flg = 0;
    
    ald_gpio_write_pin(MOTOR_CTR_PORT, MOTOR_CTR_PIN, 1);
}



