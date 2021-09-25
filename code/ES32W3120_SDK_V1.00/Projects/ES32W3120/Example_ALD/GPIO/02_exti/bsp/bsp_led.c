#include "bsp_led.h"
#include "bsp_time.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern timer_cnt_t time_cnt;
extern timer_flg_t time_flg;

void led_init(void)
{
    gpio_init_t x;

    x.mode = GPIO_MODE_OUTPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_1;

    ald_gpio_init(LED_RUN_PORT, LED_RUN_PIN, &x);
    ald_gpio_write_pin(LED_RUN_PORT, LED_RUN_PIN, 0);
    
    return;
}

void led_open(void)
{
    time_flg.led_twinkle_flg = 0;
    ald_gpio_write_pin(LED_RUN_PORT, LED_RUN_PIN, 0);
}

void led_close(void)
{
    time_flg.led_twinkle_flg = 0;
    ald_gpio_write_pin(LED_RUN_PORT, LED_RUN_PIN, 1);
}

void led_twinkle(void)
{
    time_cnt.led_twinkle_cnt = 0;
    time_flg.led_twinkle_flg = 1;
}
