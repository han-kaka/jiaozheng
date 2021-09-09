#ifndef __BSP_DX_BT24_T_H
#define __BSP_DX_BT24_T_H

#include "ald_conf.h"

#include "bsp_common.h"

//-----------��IO����--------------------------
#define UART0_TX_PORT             GPIOA
#define UART0_TX_PIN              GPIO_PIN_2
#define UART0_RX_PORT             GPIOA
#define UART0_RX_PIN              GPIO_PIN_3

#define BLE_REST_PORT             GPIOA
#define BLE_REST_PIN              GPIO_PIN_9
#define BLE_CONN_PORT             GPIOA
#define BLE_CONN_PIN              GPIO_PIN_11
#define BLE_INT_PORT              GPIOA
#define BLE_INT_PIN               GPIO_PIN_4

#define UARTX                     UART0

void uart_init(void);

void dx_bt24_t_init(void);

void send_ble_data(uint8_t *tx_buf, uint8_t tx_len);

void ble_test(void);
#endif


