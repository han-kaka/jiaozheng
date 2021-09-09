#ifndef __BSP_DX_BT24_T_H
#define __BSP_DX_BT24_T_H

#include "ald_conf.h"

#include "bsp_common.h"

//-----------��IO����--------------------------
#define UART0_TX_PORT   GPIOA
#define UART0_TX_PIN    GPIO_PIN_2
#define UART0_RX_PORT   GPIOA
#define UART0_RX_PIN    GPIO_PIN_3

#define UARTX           UART0

void uart_init(void);

void send_ble_data(uint8_t *tx_buf, uint8_t tx_len);
#endif


