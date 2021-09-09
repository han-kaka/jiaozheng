#include "bsp_mpu6050.h"

#define MPU_ADDR    0x68

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
uint8_t g_recv_buf[8];
uint8_t g_recv_len = sizeof(g_recv_buf);
uint8_t g_send_buf[8];
uint8_t g_send_len = sizeof(g_send_buf);
volatile uint8_t g_rx_complete;
volatile uint8_t g_tx_complete;
i2c_handle_t g_h_i2c;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

///**
//  * @brief  delay some time.
//  * @retval None.
//  */
//static void delay(int i)
//{
//    while (i--) ;
//}

/**
  * @brief  Initializate pin of i2c module.
  * @retval None
  */
static void i2c_pin_init(void)
{
    gpio_init_t x;
    memset(&x, 0, sizeof(gpio_init_t));

    /* Initialize scl pin */
    x.mode = GPIO_MODE_OUTPUT;
    x.odos = GPIO_OPEN_DRAIN;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_TTL;
    x.func = GPIO_FUNC_3;
    ald_gpio_init(I2C1_SCL_PORT, I2C1_SCL_PIN, &x);

    /* Initialize sda pin */
    x.mode = GPIO_MODE_OUTPUT;
    x.odos = GPIO_OPEN_DRAIN;
    x.pupd = GPIO_PUSH_UP;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_TTL;
    x.func = GPIO_FUNC_3;
    ald_gpio_init(I2C1_SDA_PORT, I2C1_SDA_PIN, &x);

    return;
}

/*Completion of the host*/
void master_tx_complete(i2c_handle_t *arg)
{
    g_tx_complete = 1;
    return;
}

/*The host receives the complete*/
void master_rx_complete(i2c_handle_t *arg)
{
    g_rx_complete = 1;
    return;
}

/**
  * @brief  i2c_init
  * @retval None
  */
void i2c_init(void)
{
    uint8_t g_send_temp = 0x75;
    uint8_t g_recv_temp = 0;
    
    /* Initialize i2c pin */
    i2c_pin_init();

    /* Enable I2c interrupt */
    ald_mcu_irq_config(I2C1_EV_IRQn, 3, 3, ENABLE);
    ald_mcu_irq_config(I2C1_ERR_IRQn, 3, 3, ENABLE);

    /* clear i2c_handle_t structure */
    memset(&g_h_i2c, 0, sizeof(i2c_handle_t));
    /* Initialize i2c */
    g_h_i2c.perh = I2C1;
    g_h_i2c.init.module   = I2C_MODULE_MASTER;
    g_h_i2c.init.addr_mode    = I2C_ADDR_7BIT;
    g_h_i2c.init.clk_speed    = 100000;
    g_h_i2c.init.dual_addr    = I2C_DUALADDR_ENABLE;
    g_h_i2c.init.general_call = I2C_GENERALCALL_DISABLE;
    g_h_i2c.init.no_stretch   = I2C_NOSTRETCH_DISABLE;
    g_h_i2c.master_rx_cplt_cbk = master_rx_complete;
    g_h_i2c.master_tx_cplt_cbk = master_tx_complete;
    g_h_i2c.init.own_addr1    = 0xA0;
    ald_i2c_init(&g_h_i2c);

    SET_BIT(g_h_i2c.perh->FCON, I2C_FCON_TXFRST_MSK);
    SET_BIT(g_h_i2c.perh->FCON, I2C_FCON_RXFRST_MSK);
    MODIFY_REG(I2C1->FCON, I2C_FCON_RXFTH_MSK, (0 << I2C_FCON_RXFTH_POSS));
    MODIFY_REG(I2C1->FCON, I2C_FCON_TXFTH_MSK, (0 << I2C_FCON_TXFTH_POSS));

    /* send data by interrupt */
    g_tx_complete = 0;
    ald_i2c_master_send_by_it(&g_h_i2c, MPU_ADDR<<1, &g_send_temp, 1);

    while (g_tx_complete != 1);
    
    /* recv data by interrupt */
    g_rx_complete = 0;
    ald_i2c_master_recv_by_it(&g_h_i2c, MPU_ADDR<<1, &g_recv_temp, 1);

    while (g_rx_complete != 1);
    
    ES_LOG_PRINT("mpu6050 id:%.2x\n", g_recv_temp);
}





