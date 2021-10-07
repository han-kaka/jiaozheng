#include "bsp_key.h"
#include "bsp_time.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
uint8_t key_click_flg = 0;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern timer_cnt_t time_cnt;
extern timer_flg_t time_flg;

void key_init(void)
{
    gpio_init_t x;
    exti_init_t exti;

    x.mode = GPIO_MODE_INPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_1;
    ald_gpio_init(KEY_PORT, KEY_PIN, &x);
    
    exti.filter      = ENABLE;
    exti.cks         = EXTI_FILTER_CLOCK_10K;
    exti.filter_time = 10;
    ald_gpio_exti_init(KEY_PORT, KEY_PIN, &exti);
    
    /* Clear interrupt flag */
    ald_gpio_exti_clear_flag_status(KEY_PIN);
    /* Configure interrupt */
    ald_gpio_exti_interrupt_config(KEY_PIN, EXTI_TRIGGER_BOTH_EDGE, ENABLE);
    
    __NVIC_EnableIRQ(EXTI11_IRQn);
    
    return;
}


