#include "md_rmu.h"

#include "bsp_mpu6050.h"
#include "bsp_time.h"
#include "bsp_system.h"
#include "bsp_flash.h"

#define MPU_ADDR    0x68

#define MPU_SAMPLE_RATE_REG 0x19
#define MPU_CFG_REG         0x1a
#define MPU_GYRO_CFG_REG    0x1b
#define MPU_ACCEL_CFG_REG   0x1c
#define MPU_FF_THR_REG      0x1d
#define MPU_FF_DUR_REG      0x1e

#define MPU_FIFO_EN_REG     0x23

#define MPU_INTBP_CFG_REG   0x37
#define MPU_INT_EN_REG      0x38
#define MPU_ACCEL_XOUTH_REG 0x3b

#define MPU_USER_CTRL_REG   0x6a
#define MPU_PWR_MGMT1_REG   0x6b
#define MPU_PWR_MGMT2_REG   0x6c

#define MPU_DEVICE_ID_REG   0x75

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
uint8_t g_recv_buf[8] = {0};
uint8_t g_recv_len = sizeof(g_recv_buf);
uint8_t g_send_buf[8] = {0};
uint8_t g_send_len = sizeof(g_send_buf);
volatile uint8_t g_rx_complete;
volatile uint8_t g_tx_complete;
i2c_handle_t g_h_i2c;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern timer_cnt_t time_cnt;
extern system_state_t system_state;
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

static uint8_t iic_read_byte(uint8_t reg)
{
    uint8_t g_send_temp = reg;
    uint8_t g_recv_temp = 0;
    
    /* send data by interrupt */
    g_tx_complete = 0;
    ald_i2c_master_send_by_it(&g_h_i2c, MPU_ADDR<<1, &g_send_temp, 1);

    while (g_tx_complete != 1);
    
    /* recv data by interrupt */
    g_rx_complete = 0;
    ald_i2c_master_recv_by_it(&g_h_i2c, MPU_ADDR<<1, &g_recv_temp, 1);

    while (g_rx_complete != 1);
    
    return g_recv_temp;
}

static void iic_write_byte(uint8_t reg, uint8_t data)
{
    uint8_t g_send_temp[2];
    
    g_send_temp[0] = reg;
    g_send_temp[1] = data;
    
    ES_LOG_PRINT("write reg:%.2x, data:%.2x\n", reg, data);
    /* send data by interrupt */
    g_tx_complete = 0;
    ald_i2c_master_send_by_it(&g_h_i2c, MPU_ADDR<<1, g_send_temp, 2);
    
    while (g_tx_complete != 1);
    
    return;
}


static void iic_write_len(uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t g_send_temp[32];
    uint8_t i = 0;
    
    g_send_temp[0] = reg;
    for(i=0; i<len; i++)
    {
        g_send_temp[1+i] = buf[i];
    }
    
    /* send data by interrupt */
    g_tx_complete = 0;
    ald_i2c_master_send_by_it(&g_h_i2c, MPU_ADDR<<1, g_send_temp, 1+i);
    
    while (g_tx_complete != 1);
    
    return;
}

static void iic_read_len(uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t g_send_temp = reg;
    
    /* send data by interrupt */
    g_tx_complete = 0;
    ald_i2c_master_send_by_it(&g_h_i2c, MPU_ADDR<<1, &g_send_temp, 1);

    while (g_tx_complete != 1);
    
    /* recv data by interrupt */
    g_rx_complete = 0;
    ald_i2c_master_recv_by_it(&g_h_i2c, MPU_ADDR<<1, buf, len);

    while (g_rx_complete != 1);
    
    return;
}

static void mpu_set_gyro_fsr(uint8_t fsr)
{
    iic_write_byte(MPU_GYRO_CFG_REG, fsr<<3);
    return;
}

static void mpu_set_accel_fsr(uint8_t fsr)
{
    iic_write_byte(MPU_ACCEL_CFG_REG, fsr<<3);
    return;
}

static void mpu_set_lpf(uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188){
        data=1;
    }
    else if(lpf>=98){
        data=2;
    }
    else if(lpf>=42){
        data=3;
    }
    else if(lpf>=20){
        data=4;
    }
    else if(lpf>=10){
        data=5;
    }
    else{
        data=6;
    }
    
    iic_write_byte(MPU_CFG_REG, data);
    return;
}

static void mpu_set_rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000){
        rate = 1000;
    }
    if(rate<4){
        rate = 4;
    }
    data = 1000/rate - 1;
    iic_write_byte(MPU_SAMPLE_RATE_REG, data);
    mpu_set_lpf(rate/2);
    return;
}


/**
  * @brief  i2c_init
  * @retval None
  */
static void i2c_init(void)
{
    uint8_t mpu6050_id = 0;
    
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

    mpu6050_id = iic_read_byte(MPU_DEVICE_ID_REG);
    ES_LOG_PRINT("mpu6050 id:%.2x\n", mpu6050_id);
    
    return;
}

void mpu_get_accelerometer(short *ax, short *ay, short *az)
{
    uint8_t buf[6] = {0};
    
    iic_read_len(MPU_ACCEL_XOUTH_REG, 6, buf);
    
    *ax = ((uint16_t)buf[2] << 8)|buf[3];
    *ay = ((uint16_t)buf[0] << 8)|buf[1];
    *az = ((uint16_t)buf[4] << 8)|buf[5];
    
    *ax = *ax + system_state.correct_ax;
    *ay = *ay + system_state.correct_ay;
    *az = *az + system_state.correct_az;
    
    ES_LOG_PRINT("ax:%d, ay:%d, az:%d\n", *ax, *ay, *az);
}

void mpu6050_init(void)
{
    gpio_init_t x;
    exti_init_t exti;
    
    memset(&exti, 0, sizeof(exti));
    memset(&x, 0, sizeof(x));
    
//    x.mode = GPIO_MODE_OUTPUT;
//    x.odos = GPIO_PUSH_PULL;
//    x.pupd = GPIO_PUSH_UP;
//    x.odrv = GPIO_OUT_DRIVE_NORMAL;
//    x.flt  = GPIO_FILTER_DISABLE;
//    x.type = GPIO_TYPE_CMOS;
//    x.func = GPIO_FUNC_1;
//    ald_gpio_init(PWR_6050_PORT, PWR_6050_PIN, &x);
//    ald_gpio_write_pin(PWR_6050_PORT, PWR_6050_PIN, 0);
//    
//    ald_delay_ms(1000);
    
    x.mode = GPIO_MODE_INPUT;
    x.odos = GPIO_PUSH_PULL;
    x.pupd = GPIO_PUSH_DOWN;
    x.odrv = GPIO_OUT_DRIVE_NORMAL;
    x.flt  = GPIO_FILTER_DISABLE;
    x.type = GPIO_TYPE_CMOS;
    x.func = GPIO_FUNC_1;
    ald_gpio_init(MPU6050_INT_PORT, MPU6050_INT_PIN, &x);
    
    exti.filter      = ENABLE;
    exti.cks         = EXTI_FILTER_CLOCK_10K;
    exti.filter_time = 10;
    ald_gpio_exti_init(MPU6050_INT_PORT, MPU6050_INT_PIN, &exti);
    
    /* Clear interrupt flag */
    ald_gpio_exti_clear_flag_status(MPU6050_INT_PIN);
    /* Configure interrupt */
    ald_gpio_exti_interrupt_config(MPU6050_INT_PIN, EXTI_TRIGGER_RISING_EDGE, ENABLE);

    __NVIC_EnableIRQ(EXTI13_IRQn);
    
//    i2c_init();
//    
//    ald_delay_ms(20);
//    
//    mpu6050_set();
    
    return;
}

void mpu6050_set(void)
{
    uint8_t mpu6050_id = 0;
    short ax[3] = {0};
    short ay[3] = {0};
    short az[3] = {0};
    
    iic_write_byte(MPU_PWR_MGMT1_REG, 0x80);//????MPU6050
    ald_delay_ms(100);
    iic_write_byte(MPU_PWR_MGMT1_REG, 0x00);//????MPU6050
    
    mpu_set_gyro_fsr(3); //????????????, 2000dps
    mpu_set_accel_fsr(0); //????????????, 2g
    mpu_set_rate(50); //????????????50HZ
    iic_write_byte(MPU_INT_EN_REG, 0x00); //????????????
    iic_write_byte(MPU_USER_CTRL_REG, 0x00);//I2C??????????
    iic_write_byte(MPU_FIFO_EN_REG, 0x00);//????FIFO
    iic_write_byte(MPU_INTBP_CFG_REG, 0x80);//INT??????????????
    
    mpu6050_id = iic_read_byte(MPU_DEVICE_ID_REG);
    ES_LOG_PRINT("mpu6050 id:%.2x\n", mpu6050_id);
    
    if(MPU_ADDR == mpu6050_id){
        iic_write_byte(MPU_PWR_MGMT1_REG, 0x01);//????CLKSEL,PLL X ??????
        iic_write_byte(MPU_PWR_MGMT2_REG, 0x00);//??????????????????
        mpu_set_rate(50); //????????????50HZ
        ES_LOG_PRINT("mpu6050_set ok\n");
        ald_delay_ms(200);
        mpu_get_accelerometer(&ax[0], &ay[0], &az[0]);
        ald_delay_ms(20);
        mpu_get_accelerometer(&ax[1], &ay[1], &az[1]);
        ald_delay_ms(20);
        mpu_get_accelerometer(&ax[2], &ay[2], &az[2]);
        if((0==ax[0]) && (0==ay[0]) && (0==az[0])){
            ald_gpio_write_pin(PWR_6050_PORT, PWR_6050_PIN, 1);
            md_rmu_reset();
        }
        else{
            if(0 == system_state.mpu6050_correct_flag){
                system_state.mpu6050_correct_flag = 1;
                system_state.correct_ax = 0 - (ax[0] + ax[1] + ax[2])/3;
                system_state.correct_ay = 0 - (ay[0] + ay[1] + ay[2])/3;
                system_state.correct_az = 16384 - (az[0] + az[1] + az[2])/3;
                save_system_info();
            }
        }
    }
    else{
        ES_LOG_PRINT("mpu6050_set err\n");
    }
    
    time_cnt.mpu6050_data_cnt = 0;
    
    system_state.system_flg.mpu6050_init_flg = 1;
}

void mpu6050_quick_init(void)
{
    __NVIC_DisableIRQ(EXTI13_IRQn);
    
    mpu6050_set();
}

void mpu6050_int_set(void)
{
    iic_write_byte(MPU_FF_THR_REG, 0x25);
    iic_write_byte(MPU_FF_THR_REG, 0x14);
    iic_write_byte(MPU_CFG_REG, 0x04);
    iic_write_byte(MPU_ACCEL_CFG_REG, 0x1c);
    iic_write_byte(MPU_INTBP_CFG_REG, 0x1c);
    iic_write_byte(MPU_INT_EN_REG, 0x40);
}

void mpu6050_int_init(void)
{
    system_state.system_flg.mpu6050_init_flg = 0;
    mpu6050_int_set();
    
    __NVIC_EnableIRQ(EXTI13_IRQn);
}

