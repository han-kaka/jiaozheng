#include "bsp_mp6050.h"
#include "bsp_dx_bt24_t.h"
#include "bsp_flash.h"
#include "bsp_power.h"
#include "bsp_time.h"
#include "bsp_motor.h"
#include "bsp_led.h"
#include "bsp_system.h"

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
    ststem_state.system_mode = E_WAIE_CONN_MODE;
    
    /* ≥ı ºªØwxid */
    ststem_state.wxid[0] = 0x01;
    ststem_state.wxid[1] = 0x02;
    ststem_state.wxid[2] = 0x03;
    ststem_state.wxid[3] = 0x04;
}


void start_init_task(void)
{
//    i2c_init();
//    uart_init();
//    spi_init();
//    adc_init();
//    motor_init();
//    led_init();
    
//    time_init();
}



