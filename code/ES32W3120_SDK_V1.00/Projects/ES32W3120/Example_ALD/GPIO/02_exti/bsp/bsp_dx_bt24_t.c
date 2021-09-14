#include "bsp_dx_bt24_t.h"
#include "bsp_time.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
uart_handle_t g_h_uart;
uint8_t g_rx_buf[20];
uint8_t g_rx_len = 0;
uint8_t g_tx_buf[256];
uint8_t g_tx_len;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern timer_cnt_t time_cnt;
extern timer_flg_t time_flg;

/**
  * @brief  Init UART pin
  * @retval None
  */
static void uart_pin_init(void)
{
    gpio_init_t x;
    memset(&x, 0x00, sizeof(gpio_init_t));

    /* Initialize tx pin: PA2 */
    x.mode = GPIO_MODE_OUTPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_TTL;
    x.func = GPIO_FUNC_2;
    ald_gpio_init(UART0_TX_PORT, UART0_TX_PIN, &x);

    /* Initialize rx pin: PA3 */
    x.mode = GPIO_MODE_INPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_TTL;
    x.func = GPIO_FUNC_2;
    ald_gpio_init(UART0_RX_PORT, UART0_RX_PIN, &x);

    return;
}

/**
  * @brief  Send message complete.
  * @param  arg: Pointer to uart_handle_t structure.
  * @retval None.
  */
static void uart_send_complete(uart_handle_t *arg)
{
    return;
}

/**
  * @brief  Receive a message complete.
  * @param  arg: Pointer to uart_handle_t structure.
  * @retval None.
  */
static void uart_recv_complete(uart_handle_t *arg)
{
//    static uint8_t s_i;
//    uint8_t i;
//    
//    s_i++;

//    if ((s_i > 1) && (*(g_h_uart.rx_buf - 2) == 0x0D) && (*(g_h_uart.rx_buf - 1) == 0x0A))
//    {
//        ald_uart_send_by_it(&g_h_uart, g_rx_buf, s_i);
//        ES_LOG_PRINT("uart send:");
//        for(i=0; i<s_i; i++){
//            ES_LOG_PRINT("%x", g_rx_buf[i]);
//        }
//        ES_LOG_PRINT("s_i %u\n", s_i);
//        s_i = 0;
//        ald_uart_recv_by_it(&g_h_uart, g_rx_buf, 1);
//        
//    }
//    else
//    {
//        if (s_i >= 20) {
//            s_i = 0;
//        }

//        ald_uart_recv_by_it(&g_h_uart, g_rx_buf + s_i, 1);
//    }

//    return;

    time_cnt.uart_timeout_cnt = 0;
    time_flg.uart_timeout_flg = 1;
    g_rx_len++;

    if (20 <= g_rx_len) {
        g_rx_len = 0;
    }
    
    ald_uart_recv_by_it(&g_h_uart, g_rx_buf + g_rx_len, 1);

    return;
}

void uart_init(void)
{
    ald_mcu_irq_config(UART0_IRQn, 3, 3, ENABLE);

    /* Initialize pin */
    uart_pin_init();

    memset(&g_h_uart, 0x00, sizeof(uart_handle_t));
    /* Initialize uart */
    g_h_uart.perh             = UART0;
    g_h_uart.init.baud        = 115200;
    g_h_uart.init.word_length = UART_WORD_LENGTH_8B;
    g_h_uart.init.stop_bits   = UART_STOP_BITS_1;
    g_h_uart.init.parity      = UART_PARITY_NONE;
    g_h_uart.init.mode        = UART_MODE_UART;
    g_h_uart.init.fctl        = UART_HW_FLOW_CTL_DISABLE;
    g_h_uart.rx_cplt_cbk      = uart_recv_complete;
    g_h_uart.tx_cplt_cbk      = uart_send_complete;
    ald_uart_init(&g_h_uart);
    
    ald_uart_rx_fifo_config(&g_h_uart, UART_RXFIFO_1BYTE);
    ald_uart_tx_fifo_config(&g_h_uart, UART_TXFIFO_EMPTY);

    g_rx_len = 0;
    ald_uart_recv_by_it(&g_h_uart, g_rx_buf, 1);
    
    return;
}

void dx_bt24_t_init(void)
{
    gpio_init_t x;
    exti_init_t exti;
    
    uart_init();
    
    memset(&exti, 0, sizeof(exti));
    memset(&x, 0, sizeof(x));
    
    x.mode = GPIO_MODE_OUTPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_1;

    ald_gpio_init(BLE_REST_PORT, BLE_REST_PIN, &x);
    ald_gpio_write_pin(BLE_REST_PORT, BLE_REST_PIN, 0);
    ald_gpio_init(BLE_CONN_PORT, BLE_CONN_PIN, &x);
    ald_gpio_write_pin(BLE_CONN_PORT, BLE_CONN_PIN, 1);
    
    x.mode = GPIO_MODE_INPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_DOWN;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_1;
    ald_gpio_init(BLE_INT_PORT, BLE_INT_PIN, &x);
    
    exti.filter      = ENABLE;
    exti.cks         = EXTI_FILTER_CLOCK_10K;
    exti.filter_time = 10;
    ald_gpio_exti_init(GPIOA, GPIO_PIN_4, &exti);
    
    /* Clear interrupt flag */
    ald_gpio_exti_clear_flag_status(GPIO_PIN_4);
    /* Configure interrupt */
    ald_gpio_exti_interrupt_config(GPIO_PIN_4, EXTI_TRIGGER_RISING_EDGE, ENABLE);
    
    __NVIC_EnableIRQ(EXTI4_IRQn);
    
    return;
}

void send_ble_data(uint8_t *tx_buf, uint8_t tx_len)
{
    uint8_t i = 0;
    for(i=0; i<tx_len; i++)
    {
        ES_LOG_PRINT("%.2x", tx_buf[i]);
    }
    ES_LOG_PRINT("\n");
    
    ald_uart_send_by_it(&g_h_uart, tx_buf, tx_len);
    
    return;
}

void ble_test(void)
{
    uint8_t data[2];
    static uint16_t num = 0;
    
    num++;
    data[0] = num >> 8;
    data[1] = num & 0xff;
    send_ble_data(data, 2);
    ald_delay_ms(60);
    
    return;
}

