#include "bsp_power.h"
#include "bsp_system.h"
#include "bsp_time.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */
adc_handle_t g_h_adc;
adc_nch_conf_t g_nch_config;
uint32_t g_adc_result = 0;

/* Public Variables ---------------------------------------------------------- */

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern system_state_t system_state;
extern timer_cnt_t time_cnt;

/**
  * @brief  Configure the ADC Pins.
  * @param  None
  * @retval None
  */
static void gpio_pin_config()
{
    /* Initialize adc gpio pin */
    gpio_init_t x;
    memset(&x, 0x0, sizeof(gpio_init_t));

    x.mode = GPIO_MODE_CLOSE;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_0;
    ald_gpio_init(ADC_CHANNEL5_PORT, ADC_CHANNEL5_PIN, &x);

    return;
}

void adc_init(void)
{
    gpio_init_t x;
    
    memset(&x, 0, sizeof(x));
    
    x.mode = GPIO_MODE_OUTPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_1;
    ald_gpio_init(PWR_ADC_PORT, PWR_ADC_PIN, &x);
    ald_gpio_write_pin(PWR_ADC_PORT, PWR_ADC_PIN, 0);
    
    /* Enable interrupt */
    ald_mcu_irq_config(ADC_IRQn, 1, 1, ENABLE);

    /* ADC gpio init */
    gpio_pin_config();

    memset(&g_h_adc, 0x0, sizeof(adc_handle_t));
    memset(&g_nch_config, 0x0, sizeof(adc_nch_conf_t));

    /* Initialize adc */
    g_h_adc.perh = ADC;
    g_h_adc.init.align    = ADC_DATAALIGN_RIGHT;
    g_h_adc.init.scan     = ENABLE;
    g_h_adc.init.cont     = DISABLE;
    g_h_adc.init.disc     = ADC_ALL_DISABLE;
    g_h_adc.init.disc_nr  = ADC_DISC_NR_1;
    g_h_adc.init.data_bit = ADC_CONV_BIT_12;
    g_h_adc.init.div      = ADC_CKDIV_16;
    g_h_adc.init.nche_sel = ADC_NCHESEL_MODE_ALL;
    g_h_adc.init.nch_nr   = ADC_NCH_NR_1;
    g_h_adc.init.n_ref    = ADC_NEG_REF_VSS;
    g_h_adc.init.p_ref    = ADC_POS_REF_VDD;
    ADC_ENABLE(&g_h_adc);
    ADC_DISABLE(&g_h_adc);
    ald_adc_init(&g_h_adc);

    /* Initialize normal convert channel */
    g_nch_config.ch   = ADC_CHANNEL_5;
    g_nch_config.idx  = ADC_NCH_IDX_1;
    g_nch_config.samp = ADC_SAMPLETIME_15;
    ald_adc_normal_channel_config(&g_h_adc, &g_nch_config);

    /* Start normal convert, enable interrupt */
    ald_adc_normal_start_by_it(&g_h_adc);
    
    time_cnt.adc_check_cnt = 0;
    
    system_state.system_flg.adc_init_flg = 1;
    
    return;
}

void charge_init(void)
{
    gpio_init_t x;
    exti_init_t exti;
    
    memset(&exti, 0, sizeof(exti));
    memset(&x, 0x0, sizeof(gpio_init_t));

    x.mode = GPIO_MODE_INPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_1;
    ald_gpio_init(CHARGE_G_PORT, CHARGE_G_PIN, &x);
    ald_gpio_init(CHARGE_Y_PORT, CHARGE_Y_PIN, &x);
    
    exti.filter      = ENABLE;
    exti.cks         = EXTI_FILTER_CLOCK_10K;
    exti.filter_time = 10;
    ald_gpio_exti_init(CHARGE_G_PORT, CHARGE_G_PIN, &exti);
    ald_gpio_exti_init(CHARGE_Y_PORT, CHARGE_Y_PIN, &exti);
    
    /* Clear interrupt flag */
    ald_gpio_exti_clear_flag_status(CHARGE_G_PIN);
    /* Configure interrupt */
    ald_gpio_exti_interrupt_config(CHARGE_G_PIN, EXTI_TRIGGER_BOTH_EDGE, ENABLE);

/* Clear interrupt flag */
    ald_gpio_exti_clear_flag_status(CHARGE_Y_PIN);
    /* Configure interrupt */
    ald_gpio_exti_interrupt_config(CHARGE_Y_PIN, EXTI_TRIGGER_BOTH_EDGE, ENABLE);

    __NVIC_EnableIRQ(EXTI2_IRQn);
    __NVIC_EnableIRQ(EXTI3_IRQn);
    
    return;
}
