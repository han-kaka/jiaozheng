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

#define PWR_FLASH_PORT                        GPIOB
#define PWR_FLASH_PIN                         GPIO_PIN_15

typedef struct {
    uint8_t shake_fre;
    uint8_t wxid[4];
    uint8_t reserve[123];
    
} system_info_t;

void flash_init(void);

void init_system_info(system_state_t *system_state);

int save_system_info(void);

void save_accelerometer(uint16_t ax, uint16_t ay, uint16_t az);

//md_status_t flash_write(uint32_t addr, char *buf, uint8_t size);

//md_status_t flash_read(uint32_t addr, char *buf, uint16_t size);
#endif


