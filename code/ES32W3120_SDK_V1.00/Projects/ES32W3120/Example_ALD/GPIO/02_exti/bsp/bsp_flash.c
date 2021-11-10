#include "bsp_flash.h"
#include "bsp_system.h"
#include "bsp_time.h"
#include "bsp_dx_bt24_t.h"

#define FLASH_CS_SET() (ald_gpio_write_pin(GPIOB, GPIO_PIN_13, 1))
#define FLASH_CS_CLR() (ald_gpio_write_pin(GPIOB, GPIO_PIN_13, 0))

#define FLASH_WRITE_ENABLE  0x06
#define FLASH_WRITE_DISABLE 0x04
#define FLASH_ERASE         0x20
#define FLASH_PROGRAM       0x02
#define FLASH_READ          0x03
#define FLASH_ID            0x9F
#define FLASH_STATUS        0x05

#define FLASH_PAGE_SIZE     (256)

#define IAP_DATA_ADDRESS    0x20000

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
static spi_handle_t s_gs_spi;
uint8_t g_flash_id[4] = {0};
uint8_t accelerometer_data_temp[FLASH_WRITE_BUFF_LEN] = {0};
static uint8_t save_pack_temp = 0;
uint8_t accelerometer_data_send_temp[FLASH_READ_BUFF_LEN] = {0};
uint8_t send_page_temp = 0;

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

/**
  * @brief  wait until flash unbusy.
  * @retval Status, see @ref ald_status_t.
  */
static ald_status_t flash_wait_unbusy(void)
{
    uint8_t status;
    int r_flag = 0;

    FLASH_CS_CLR(); /* 片选拉低，选中Flash */

    if (ald_spi_send_byte_fast(&s_gs_spi, (uint8_t)FLASH_STATUS) != OK)   /* 发送读状态命令 */
    {
        FLASH_CS_SET();     /* 片选拉高，释放Flash */
        return ERROR;
    }

    do
    {
        status = ald_spi_recv_byte_fast(&s_gs_spi, &r_flag);

        if (r_flag != OK)
        {
            FLASH_CS_SET();
            return ERROR;
        }
    }
    while (status & 0x01);

    FLASH_CS_SET();

    return OK;
}

static ald_status_t flash_wait_busy_timeout(uint8_t milliseconds)
{
    uint8_t count = milliseconds;

    for (; count > 0; count--)
    {
        if (OK == flash_wait_unbusy())
            break;

        ald_delay_ms(1);
    }

    if (count == 0)
        return ERROR;

    return OK;
}

static ald_status_t flash_write_enable(void)
{
    FLASH_CS_CLR(); /* 片选拉低，选中Flash */

    if (OK != ald_spi_send_byte_fast(&s_gs_spi, FLASH_WRITE_ENABLE)){
        FLASH_CS_SET();      /* 片选拉高，释放Flash */
        return ERROR;
    }

    FLASH_CS_SET();      /* 片选拉高，释放Flash */

    return OK;
}

static ald_status_t flash_write_disable(void)
{
    FLASH_CS_CLR(); /* 片选拉低，选中Flash */

    if (OK != ald_spi_send_byte_fast(&s_gs_spi, FLASH_WRITE_DISABLE)){
        FLASH_CS_SET();      /* 片选拉高，释放Flash */
        return ERROR;
    }

    FLASH_CS_SET();      /* 片选拉高，释放Flash */

    return OK;
}

/**
  * @brief  transmit an amount of data in blocking mode.
  * @param  addr: Specific address which to be write.
  * @param  buf: Pointer to data buffer
  * @param  size: Amount of data to be sent
  * @retval Status, see @ref ald_status_t.
  */
static ald_status_t flash_page_program(uint32_t addr, char *buf, uint16_t size)
{
    uint8_t cmd_buf[4];
    uint16_t i = 0U;

    if (buf == NULL)
        return ERROR;

    if(flash_wait_busy_timeout(FLASH_BUSY_TIMEOUT)){
        return BUSY;
    }
    
    if (OK != flash_write_enable()){
        return ERROR;
    }
    
    cmd_buf[0] = FLASH_PROGRAM;
    cmd_buf[1] = (addr >> 16) & 0xff;
    cmd_buf[2] = (addr >> 8) & 0xff;
    cmd_buf[3] = addr & 0xff;

    ald_delay_ms(100);
    FLASH_CS_CLR();

    for (i = 0; i < sizeof(cmd_buf); i++)     /* 发送编程指令和3个字节Flash地址 */
    {
        if (ald_spi_send_byte_fast(&s_gs_spi, cmd_buf[i]) != OK)
        {
            FLASH_CS_SET();
            return ERROR;
        }
    }

    for (i = 0; i < size; i++)  /* 待写数据发送到Flash */
    {
        if (ald_spi_send_byte_fast(&s_gs_spi, buf[i]) != OK)
        {
            FLASH_CS_SET();
            return ERROR;
        }
    }

    FLASH_CS_SET();

    return OK;
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

/**
  * @brief  transmit an amount of data in blocking mode.
  * @param  addr: Specific address which to be write.
  * @param  buf: Pointer to data buffer
  * @param  size: Amount of data to be sent
  * @retval Status, see @ref ald_status_t.
  */
ald_status_t flash_write_data(uint32_t addr, char *buf, uint16_t size)
{
    
    uint16_t count = size / FLASH_PAGE_SIZE;
    uint16_t left = size % FLASH_PAGE_SIZE;
    uint8_t i = 0;

    if (size == 0){
        return ERROR;
    }

    for (i = 0; i < count; i++){
        if (OK != flash_page_program(addr + i * FLASH_PAGE_SIZE, buf + i * FLASH_PAGE_SIZE, FLASH_PAGE_SIZE)){
            return ERROR;
        }
    }

    if (left)
    {
        if (OK != flash_page_program(addr + i * FLASH_PAGE_SIZE, buf + i * FLASH_PAGE_SIZE, left)){
            return ERROR;
        }
    }

    return OK;
}

/**
  * @brief  flash sector erase function
  * @param  addr: Specific address which sector to be erase.
  * @retval Status.
  */
ald_status_t flash_sector_erase(uint32_t addr)
{
    uint8_t cmd_buf[4];
    uint8_t i = 0U;

    if(flash_wait_busy_timeout(FLASH_BUSY_TIMEOUT)){
        return BUSY;
    }
    
    if (OK != flash_write_enable()){
        return ERROR;
    }
    
    cmd_buf[0] = FLASH_ERASE;       /* Flash扇区擦除指令 */
    cmd_buf[1] = (addr >> 16) & 0xff;   /* 24 bit Flash地址 */
    cmd_buf[2] = (addr >> 8) & 0xff;
    cmd_buf[3] = addr & 0xff;

    ald_delay_ms(100);
    FLASH_CS_CLR();

    for (i = 0; i < sizeof(cmd_buf); i++)     /* 发送扇区擦除指令和3个字节的Flash地址 */
    {
        if (ald_spi_send_byte_fast(&s_gs_spi, cmd_buf[i]) != OK)
        {
            FLASH_CS_SET();
            return ERROR;
        }
    }

    FLASH_CS_SET();

    return OK;
}

/**
  * @brief  Receive an amount of data in blocking mode.
  * @param  addr: address of flash where want to read.
  * @param  buf: Pointer to data buffer
  * @param  size: Amount of data to be received
  * @retval Status, see @ref ald_status_t.
  */
ald_status_t flash_read(uint32_t addr, char *buf, uint16_t size)
{
    uint8_t cmd_buf[4];
    uint8_t i = 0U;
    int r_flag = 0;

    if (buf == NULL)
        return ERROR;

    if(flash_wait_busy_timeout(FLASH_BUSY_TIMEOUT)){
        return BUSY;
    }
    
    cmd_buf[0] = FLASH_READ;
    cmd_buf[1] = (addr >> 16) & 0xff;
    cmd_buf[2] = (addr >> 8) & 0xff;
    cmd_buf[3] = addr & 0xff;

    FLASH_CS_CLR();     /* ????,??Flash */

    for (i = 0; i < sizeof(cmd_buf); i++)   /* ????????????Flash?? */
    {
        if (ald_spi_send_byte_fast(&s_gs_spi, cmd_buf[i]) != OK)
        {
            FLASH_CS_SET();     /* ????,??Flash */
            return ERROR;
        }
    }

    for (i = 0; i < size; i++)  /* ????????????Flash?? */
    {
        buf[i] = ald_spi_recv_byte_fast(&s_gs_spi, &r_flag);

        if (r_flag != OK)
        {
            FLASH_CS_SET();
            return ERROR;
        }
    }

    FLASH_CS_SET();

    return OK;
}

void init_system_info(system_state_t *system_state)
{
    /* 从片内 flash 中读取相关数据 */
    system_info_t system_info = {0};
    
    __disable_irq();

    if (IAP_FASTPROGRAM(IAP_DATA_ADDRESS, (uint8_t *)&system_info, 128, AUTO_ERASE_TRUE, IAP_FREQUENCE_48M) != RESET){
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
    if(system_info.mpu6050_correct_flag){
        system_state->mpu6050_correct_flag = system_info.mpu6050_correct_flag;
        system_state->correct_ax = system_info.correct_ax;
        system_state->correct_ay = system_info.correct_ay;
        system_state->correct_az = system_info.correct_az;
    }
    else{
        system_state->mpu6050_correct_flag = 0;
    }
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
    system_info.mpu6050_correct_flag = system_state.mpu6050_correct_flag;
    system_info.correct_ax = system_state.correct_ax;
    system_info.correct_ay = system_state.correct_ay;
    system_info.correct_az = system_state.correct_az;
    
    __disable_irq();

    do
    {
        /* Double words write testing */
        if (IAP_PAGEERASE(IAP_DATA_ADDRESS, IAP_FREQUENCE_48M) == RESET){    /* Erase page 64 */
            ES_LOG_PRINT("IAP_PAGEERASE fail\n");
            res = -1;
            break;
        }
        
        /* Write data_buf to 0x20000 */
        if (IAP_FASTPROGRAM(IAP_DATA_ADDRESS, (uint8_t *)&system_info, 128, AUTO_ERASE_TRUE, IAP_FREQUENCE_48M) == RESET){
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
    ald_status_t status;
//    char s_flash_txbuf[32] = "essemi mcu spi flash example!";     /* 长度必须小于一页(256字节) */
//    char s_flash_rxbuf[32];
    
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

    ald_delay_ms(20);
    
    spi_init();

//    flash_sector_erase(0);
    status = flash_read(0, (char *)(&system_state.flash_data), sizeof(flash_data_t));
    if (status == OK){
        ES_LOG_PRINT("read OK!flash data page:%u, send data page:%u\n", system_state.flash_data.flash_data_current_page, system_state.flash_data.flash_data_send_page);
    }
    
    if(0xaa != system_state.flash_data.data_flag){
        system_state.flash_data.data_flag = 0xaa;
        system_state.flash_data.flash_data_current_page = 4;
        system_state.flash_data.flash_data_send_page = 4;
    }
}

void save_accelerometer(uint16_t ax, uint16_t ay, uint16_t az)
{
    uint8_t save_data_temp[20];
    uint8_t sum = 0;
    uint8_t i = 0;
//    uint16_t j = 0;
    ald_status_t status;
    
    memset(save_data_temp, 0, 20);
    save_data_temp[0] = 0xaa;
    save_data_temp[1] = 0x13;
    save_data_temp[2] = 0xd5;
    save_data_temp[3] = 0x03;
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

    sum = 0;
    for(i=0; i<19; i++){
        sum += save_data_temp[i];
    }
    save_data_temp[19] = sum;
    
    /* 保存至外部flash */
    memcpy(accelerometer_data_temp+20*save_pack_temp, save_data_temp, 20);
    save_pack_temp++;
    if(50 <= save_pack_temp)
    {
        if(0 == (system_state.flash_data.flash_data_current_page%4)){
            status = flash_sector_erase((FLASH_DATA_START+system_state.flash_data.flash_data_current_page)*FLASH_PAGE_LEN);
        }
        status = flash_write_data((FLASH_DATA_START+system_state.flash_data.flash_data_current_page)*FLASH_PAGE_LEN, (char *)accelerometer_data_temp, FLASH_WRITE_BUFF_LEN);
        if (status == OK){
            ES_LOG_PRINT("write accelerometer flash data OK, addr%u\n", (FLASH_DATA_START+system_state.flash_data.flash_data_current_page)*FLASH_PAGE_LEN);
            
            if(system_state.flash_data.flash_data_current_page+1 == system_state.flash_data.flash_data_send_page){
                if(FLASH_DATA_END == system_state.flash_data.flash_data_send_page){
                    system_state.flash_data.flash_data_send_page = FLASH_DATA_START;
                }
                else{
                    system_state.flash_data.flash_data_send_page++;
                }
            }
            system_state.flash_data.flash_data_current_page++;
            if(FLASH_DATA_END < system_state.flash_data.flash_data_current_page){
                system_state.flash_data.flash_data_current_page = FLASH_DATA_START;
            }
            save_pack_temp = 0;
            
            status = flash_sector_erase(0);
            status = flash_write_data(0, (char *)(&system_state.flash_data), sizeof(flash_data_t));
            if (status == OK){
                ES_LOG_PRINT("write flash data OK!\n");
            }
        }
    }
    
    if(1 == system_state.system_flg.imu_data_flg){
        save_data_temp[3] = 0x01;
        sum = 0;
        for(i=0; i<19; i++){
            sum += save_data_temp[i];
        }
        save_data_temp[19] = sum;
        send_ble_data(save_data_temp, 20);
    }
}

int read_accelerometer_data(void)
{
    ald_status_t status;
    
    if(4 < send_page_temp){
        status = flash_read((FLASH_DATA_START+system_state.flash_data.flash_data_send_page+1)*FLASH_PAGE_LEN+(send_page_temp-5)*FLASH_READ_BUFF_LEN, (char *)(accelerometer_data_send_temp), FLASH_READ_BUFF_LEN);
        ES_LOG_PRINT("addr:%u, page_temp:%u\n", (FLASH_DATA_START+system_state.flash_data.flash_data_send_page+1)*FLASH_PAGE_LEN+(send_page_temp-9)*FLASH_READ_BUFF_LEN, send_page_temp);
    }
    else{
        status = flash_read((FLASH_DATA_START+system_state.flash_data.flash_data_send_page)*FLASH_PAGE_LEN+send_page_temp*FLASH_READ_BUFF_LEN, (char *)(accelerometer_data_send_temp), FLASH_READ_BUFF_LEN);
        ES_LOG_PRINT("addr:%u, page_temp:%u\n", (FLASH_DATA_START+system_state.flash_data.flash_data_send_page)*FLASH_PAGE_LEN+send_page_temp*FLASH_READ_BUFF_LEN, send_page_temp);
    }
    if (status == OK){
        ES_LOG_PRINT("read flash data OK\n");

        send_page_temp++;
        return 0;
    }
    
    return -1;
}

int save_flash_page_data(void)
{
    ald_status_t status;
    
    status = flash_sector_erase(0);
    status = flash_write_data(0, (char *)(&system_state.flash_data), sizeof(flash_data_t));
    if (status == OK){
        ES_LOG_PRINT("write flash data OK!\n");
        return 0;
    }
    
    return -1;
}
