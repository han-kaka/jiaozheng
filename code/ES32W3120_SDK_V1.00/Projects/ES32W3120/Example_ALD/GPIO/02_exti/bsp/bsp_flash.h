#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

#include "ald_conf.h"
#include "md_conf.h"

#include "bsp_system.h"
#include "bsp_common.h"

//-----------¸÷IO¶¨Òå--------------------------

#define SPI_NSS_PORT                          GPIOB
#define SPI_NSS_PIN                           GPIO_PIN_13
#define SPI_SCK_PORT                          GPIOB
#define SPI_SCK_PIN                           GPIO_PIN_12
#define SPI_MOSI_PORT                         GPIOB
#define SPI_MOSI_PIN                          GPIO_PIN_15
#define SPI_MISO_PORT                         GPIOB
#define SPI_MISO_PIN                          GPIO_PIN_14

#define PWR_FLASH_PORT                        GPIOA
#define PWR_FLASH_PIN                         GPIO_PIN_15

#define FLASH_BUSY_TIMEOUT                    (50)

#define FLASH_DATA_PAGE                       0
#define FLASH_PAGE_LEN                        1024
#define FLASH_WRITE_BUFF_LEN                  1000

#define FLASH_READ_BUFF_LEN                   200

#define FLASH_DATA_START                      4
#define FLASH_DATA_END                        2047

typedef struct {
    uint8_t shake_fre;
    uint8_t wxid[4];
    uint8_t mpu6050_correct_flag;
    short correct_ax;
    short correct_ay;
    short correct_az;
    uint8_t reserve[116];
    
} system_info_t;

void flash_init(void);

void init_system_info(system_state_t *system_state);

int save_system_info(void);

void save_accelerometer(uint16_t ax, uint16_t ay, uint16_t az);

ald_status_t flash_write_data(uint32_t addr, char *buf, uint16_t size);

ald_status_t flash_sector_erase(uint32_t addr);

ald_status_t flash_read(uint32_t addr, char *buf, uint16_t size);

int read_accelerometer_data(void);

int save_flash_page_data(void);
#endif


