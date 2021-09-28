#include "bsp_flash.h"
#include "bsp_system.h"
#include "bsp_time.h"
#include "bsp_dx_bt24_t.h"

//#define FLASH_CS_SET() (md_gpio_write_pin(SPI_NSS_PORT, SPI_NSS_PIN, 1));
//#define FLASH_CS_CLR() (md_gpio_write_pin(SPI_NSS_PORT, SPI_NSS_PIN, 0));

#define FLASH_CS_SET() (ald_gpio_write_pin(GPIOB, GPIO_PIN_13, 1))
#define FLASH_CS_CLR() (ald_gpio_write_pin(GPIOB, GPIO_PIN_13, 0))

#define FLASH_WRITE_ENABLE  0x06
#define FLASH_ERASE         0x20
#define FLASH_PROGRAM       0x02
#define FLASH_READ          0x03
#define FLASH_ID            0x9F
#define FLASH_STATUS        0x05

#define DATA_ADDRESS        0x20000

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
static spi_handle_t s_gs_spi;
uint8_t g_flash_id[4] = {0};

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern system_state_t system_state;
extern utc_time_t utc_time;

/**
  * @brief  Initializate spi flash pin
  * @retval None.
  */
static void spi_pin_init(void)
{
    gpio_init_t l_gpio;

    l_gpio.mode = GPIO_MODE_OUTPUT;
    l_gpio.odos = GPIO_PUSH_PULL;
    l_gpio.pupd = GPIO_PUSH_UP;
    l_gpio.flt  = GPIO_FILTER_DISABLE;
    l_gpio.odrv = GPIO_OUT_DRIVE_NORMAL;
    l_gpio.type = GPIO_TYPE_TTL;
    l_gpio.func = GPIO_FUNC_1;

    ald_gpio_init(SPI_NSS_PORT, SPI_NSS_PIN, &l_gpio);  /* 初始化PB13为片选管脚 */
    FLASH_CS_SET();   /* 片选输出高，释放Flash */

    l_gpio.func = GPIO_FUNC_2;
    ald_gpio_init(SPI_SCK_PORT, SPI_SCK_PIN, &l_gpio);  /* 初始化PB12为时钟输出管脚 */

    l_gpio.mode = GPIO_MODE_INPUT;
    l_gpio.func = GPIO_FUNC_2;
    ald_gpio_init(SPI_MOSI_PORT, SPI_MOSI_PIN, &l_gpio);  /* 初始化PB15为MOSI管脚 */

    l_gpio.func = GPIO_FUNC_2;
    ald_gpio_init(SPI_MISO_PORT, SPI_MISO_PIN, &l_gpio);  /* 初始化PB14为MISO管脚 */
    
    return;
}

/**
  * @brief  Read flash id in blocking mode.
  * @retval flash id.
  */
static uint32_t flash_read_id(void)
{
    uint8_t i;
    int r_flag = 0;

    g_flash_id[0] = FLASH_ID;

    FLASH_CS_CLR(); /* 片选拉低，选中Flash */

    for (i = 0; i < sizeof(g_flash_id); i++)
    {
        if (ald_spi_send_byte_fast(&s_gs_spi, g_flash_id[i]) != OK)
        {
            FLASH_CS_SET();     /* 片选拉高，释放Flash */
            return ERROR;
        }
    }

    for (i = 0; i < 3; i++)
    {
        g_flash_id[i] = ald_spi_recv_byte_fast(&s_gs_spi, &r_flag);

        if (r_flag != OK)
        {
            FLASH_CS_SET();      /* 片选拉高，释放Flash */
            return ERROR;
        }
    }

    FLASH_CS_SET();

    return ((g_flash_id[0] << 16) | (g_flash_id[1] << 8) | (g_flash_id[2]));  /* 制造商ID flash_id[0]和设备ID flash_id[1:2] */
}

static void spi_init(void)
{
    uint32_t id;
    
    spi_pin_init();

    s_gs_spi.perh           = SPI0;               /* 使用SPI0 */
    s_gs_spi.init.mode      = SPI_MODE_MASTER;    /* SPI主机模式 */
    s_gs_spi.init.baud      = SPI_BAUD_4;         /* 时钟48MHz/4=12MHz */
    s_gs_spi.init.data_size = SPI_DATA_SIZE_8;    /* 8位模式 */
    s_gs_spi.init.polarity  = SPI_CPOL_HIGH;      /* 空闲高电平 */
    s_gs_spi.init.phase     = SPI_CPHA_SECOND;    /* 第二个边沿接收数据 */
    s_gs_spi.init.first_bit = SPI_FIRSTBIT_MSB;   /* 先发送MSB */
    s_gs_spi.init.dir       = SPI_DIRECTION_2LINES;
    s_gs_spi.init.ss_en     = DISABLE;
    s_gs_spi.init.crc_calc  = DISABLE;

    ald_spi_init(&s_gs_spi);   /* 按照参数初始化SPI外设 */
    
    id = flash_read_id();
    ES_LOG_PRINT("Manufacturer ID is %02x & Device ID is %02x %02x\n", (uint8_t)(id >> 16), (uint8_t)(id >> 8), (uint8_t)id);
    
    system_state.system_flg.flash_init_flg = 1;
    
    return;
}

void init_system_info(system_state_t *system_state)
{
    /* 从片内 flash 中读取相关数据 */
    system_info_t system_info = {0};
    
    __disable_irq();

    if (IAP_FASTPROGRAM(DATA_ADDRESS, (uint8_t *)&system_info, 128, AUTO_ERASE_TRUE, IAP_FREQUENCE_48M) != RESET){
        ES_LOG_PRINT("read data success\n");
        ES_LOG_PRINT("shake_fre: %u\n", system_info.shake_fre);
        ES_LOG_PRINT("wxid[0]: %u, wxid[1]: %u, wxid[2]: %u, wxid[3]: %u, \n", system_info.wxid[0], system_info.wxid[1], system_info.wxid[2], system_info.wxid[3]);
    }
    else{
        ES_LOG_PRINT("read data fail\n");
    }
    __enable_irq();
    
    system_state->shake_fre = system_info.shake_fre;
    system_state->wxid[0] = system_info.wxid[0];
    system_state->wxid[1] = system_info.wxid[1];
    system_state->wxid[2] = system_info.wxid[2];
    system_state->wxid[3] = system_info.wxid[3];
}

int save_system_info(void)
{
    int res = 0;
    /* 保存数据至片内 flash */
    system_info_t system_info = {0};
    
    system_info.shake_fre = system_state.shake_fre;
    system_info.wxid[0] = system_state.wxid[0];
    system_info.wxid[1] = system_state.wxid[1];
    system_info.wxid[2] = system_state.wxid[2];
    system_info.wxid[3] = system_state.wxid[3];
    
    __disable_irq();

    do
    {
        /* Double words write testing */
        if (IAP_PAGEERASE(DATA_ADDRESS, IAP_FREQUENCE_48M) == RESET){    /* Erase page 64 */
            ES_LOG_PRINT("IAP_PAGEERASE fail\n");
            res = -1;
            break;
        }
        
        /* Write data_buf to 0x20000 */
        if (IAP_FASTPROGRAM(DATA_ADDRESS, (uint8_t *)&system_info, 128, AUTO_ERASE_TRUE, IAP_FREQUENCE_48M) == RESET){
            ES_LOG_PRINT("IAP_FASTPROGRAM fail\n");
            res = -1;
            break;
        }
        else{
            ES_LOG_PRINT("IAP_FASTPROGRAM success\n");
        }
        
    }while (0);
    
    __enable_irq();
    
    return res;
}

void flash_init(void)
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
    ald_gpio_init(PWR_FLASH_PORT, PWR_FLASH_PIN, &x);
    ald_gpio_write_pin(PWR_FLASH_PORT, PWR_FLASH_PIN, 0);

    spi_init();
}

void save_accelerometer(uint16_t ax, uint16_t ay, uint16_t az)
{
    uint8_t save_data_temp[20];
    uint8_t sum = 0;
    uint8_t i = 0;
    
    memset(save_data_temp, 0, 20);
    save_data_temp[0] = 0xaa;
    save_data_temp[1] = 0x13;
    save_data_temp[2] = 0xd5;
    save_data_temp[3] = 0x01;
    save_data_temp[4] = ax >> 8;
    save_data_temp[5] = ax & 0xff;
    save_data_temp[6] = ay >> 8;
    save_data_temp[7] = ay & 0xff;
    save_data_temp[8] = az >> 8;
    save_data_temp[9] = az & 0xff;
    save_data_temp[10] = utc_time.utc_y;
    save_data_temp[11] = utc_time.utc_m;
    save_data_temp[12] = utc_time.utc_d;
    save_data_temp[13] = utc_time.utc_h;
    save_data_temp[14] = utc_time.utc_f;
    save_data_temp[15] = utc_time.utc_s;
    save_data_temp[16] = 0;
    save_data_temp[17] = 0;
    save_data_temp[18] = 0;

    sum = 0;
    for(i=0; i<19; i++){
        sum += save_data_temp[i];
    }
    save_data_temp[19] = sum;
    
    /* 保存至外部flash */
    
    if(1 == system_state.system_flg.imu_data_flg){
        send_ble_data(save_data_temp, 20);
    }
}

///**
//  * @brief  transmit an amount of data in blocking mode.
//  * @param  addr: Specific address which to be write.
//  * @param  buf: Pointer to data buffer
//  * @param  size: Amount of data to be sent
//  * @retval Status, see @ref md_status_t.
//  */
//md_status_t flash_write(uint32_t addr, char *buf, uint8_t size)
//{
//    uint8_t cmd_buf[4];
//    uint8_t i = 0U;

//    if (buf == NULL)
//        return MD_ERROR;

//    cmd_buf[0] = FLASH_PROGRAM;
//    cmd_buf[1] = (addr >> 16) & 0xff;
//    cmd_buf[2] = (addr >> 8) & 0xff;
//    cmd_buf[3] = addr & 0xff;

//    FLASH_CS_CLR();     /* 片选拉低，选中Flash */

//    if (md_spi_send_byte_fast(&s_gs_spi, FLASH_WRITE_ENABLE) != MD_OK)
//    {
//        FLASH_CS_SET();
//        return MD_ERROR;
//    }

//    FLASH_CS_SET();    /* 片选拉高，释放Flash */

//    delay(100);
//    FLASH_CS_CLR();

//    for (i = 0; i < sizeof(cmd_buf); i++)     /* 发送编程指令和三个字节的Flash地址 */
//    {
//        if (md_spi_send_byte_fast(&s_gs_spi, cmd_buf[i]) != MD_OK)
//        {
//            FLASH_CS_SET();
//            return MD_ERROR;
//        }
//    }

//    for (i = 0; i < size; i++)  /* 待写数据发送到Flash */
//    {
//        if (md_spi_send_byte_fast(&s_gs_spi, buf[i]) != MD_OK)
//        {
//            FLASH_CS_SET();
//            return MD_ERROR;
//        }
//    }

//    FLASH_CS_SET();

//    return MD_OK;
//}

///**
//  * @brief  Receive an amount of data in blocking mode.
//  * @param  addr: address of flash where want to read.
//  * @param  buf: Pointer to data buffer
//  * @param  size: Amount of data to be received
//  * @retval Status, see @ref md_status_t.
//  */
//md_status_t flash_read(uint32_t addr, char *buf, uint16_t size)
//{
//    uint8_t cmd_buf[4];
//    uint8_t i = 0U;
//    int r_flag = 0;

//    if (buf == NULL)
//        return MD_ERROR;

//    cmd_buf[0] = FLASH_READ;
//    cmd_buf[1] = (addr >> 16) & 0xff;
//    cmd_buf[2] = (addr >> 8) & 0xff;
//    cmd_buf[3] = addr & 0xff;

//    FLASH_CS_CLR();     /* 片选拉低，选中Flash */

//    for (i = 0; i < sizeof(cmd_buf); i++)   /* 发送编程指令和三个字节的Flash地址 */
//    {
//        if (md_spi_send_byte_fast(&s_gs_spi, cmd_buf[i]) != MD_OK)
//        {
//            FLASH_CS_SET();     /* 片选拉高，释放Flash */
//            g_c = 1;
//            return MD_ERROR;
//        }
//    }

//    for (i = 0; i < size; i++)  /* 发送编程指令和三个字节的Flash地址 */
//    {
//        buf[i] = md_spi_recv_byte_fast(&s_gs_spi, &r_flag);

//        if (r_flag != MD_OK)
//        {
//            FLASH_CS_SET();
//            g_c = 2;
//            return MD_ERROR;
//        }
//    }

//    FLASH_CS_SET();

//    return MD_OK;
//}

