/**********************************************************************************
 *
 * @file    irq.c
 * @brief   Interrupt handler
 *
 * @date    30 Apri 2021
 * @author  AE Team
 * @note
 *          Change Logs:
 *          Date            Author          Notes
 *          30 Apri 2021    yanght          the first version
 *
 * Copyright (C) Shanghai Eastsoft Microelectronics Co. Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 **********************************************************************************
 */

/* Includes ------------------------------------------------------------------ */
#include "main.h"
#include "eslog_init.h"

#include "bsp_dx_bt24_t.h"
#include "bsp_system.h"
#include "bsp_power.h"
#include "bsp_time.h"

#include "app_common.h"

#include "task_common.h"

/** @addtogroup Projects_Examples_ALD
  * @{
  */

/** @addtogroup Examples
  * @{
  */

/* Exported Types ------------------------------------------------------------ */

/* Exported Macros ----------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern i2c_handle_t g_h_i2c;
extern timer_handle_t g_ad16c4t_init;
extern uart_handle_t g_h_uart;
extern adc_handle_t g_h_adc;
extern uint32_t g_adc_result;
extern system_state_t system_state;
extern timer_flg_t time_flg;
extern timer_cnt_t time_cnt;

/* Exported Constants -------------------------------------------------------- */

/* Exported Functions -------------------------------------------------------- */

/**
  * @brief  NMI IRQ handler
  * @retval None
  */
void NMI_Handler(void)
{
    /* Added Emergency operation */
    return;
}

/**
  * @brief  Hardfault IRQ handler
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Added debug information */
    while (1)
        ;
}

/**
  * @brief  MemManage IRQ handler
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Added debug information */
    while (1)
        ;
}

/**
  * @brief  BusFault IRQ handler
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Added debug information */
    while (1)
        ;
}

/**
  * @brief  UsageFault IRQ handler
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Added debug information */
    while (1)
        ;
}

/**
  * @brief  Supervisor Call IRQ handler
  * @retval None
  */
void SVC_Handler(void)
{
    /* Added system callback */
    return;
}

/**
  * @brief  Debug Monitor IRQ handler
  * @retval None
  */
void DebugMon_Handler(void)
{
    /* Added debug operation */
    return;
}

/**
  * @brief  PendSV IRQ handler
  * @retval None
  */
void PendSV_Handler(void)
{
    /* Added thread switching operation */
    return;
}

/**
  * @brief  SysTick IRQ handler
  * @retval None
  */
void SysTick_Handler(void)
{
    ald_inc_tick();
    return;
}

/**
  * @brief  EXTI2 IRQ handler
  * @retval None
  */
void EXTI2_IRQHandler()
{
    ald_gpio_exti_clear_flag_status(GPIO_PIN_2);
    
    if(1 == ald_gpio_read_pin(CHARGE_Y_PORT, CHARGE_Y_PIN)){
        system_state.system_flg.charging_flg = 0;
    }
    else{
        system_state.system_flg.charging_flg = 1;
    }
}

/**
  * @brief  EXTI3 IRQ handler
  * @retval None
  */
void EXTI3_IRQHandler()
{
    ald_gpio_exti_clear_flag_status(GPIO_PIN_3);
    
    if(1 == ald_gpio_read_pin(CHARGE_G_PORT, CHARGE_G_PIN)){
        system_state.system_flg.charge_comp_flg = 0;
    }
    else{
        system_state.system_flg.charge_comp_flg = 1;
    }
}

/**
  * @brief  EXTI4 IRQ handler
  * @retval None
  */
void EXTI4_IRQHandler()
{
    ald_gpio_exti_clear_flag_status(GPIO_PIN_4);
    if(1 == ald_gpio_read_pin(BLE_INT_PORT, BLE_INT_PIN)){
        set_task(SG, CONNECT_MODE);
    }
    else{
        set_task(SG, ADV_MODE);
    }
}

/**
  * @brief  EXTI11 IRQ handler
  * @retval None
  */
void EXTI11_IRQHandler()
{
    ald_gpio_exti_clear_flag_status(GPIO_PIN_11);
    __NVIC_DisableIRQ(EXTI11_IRQn);
    time_cnt.key_cnt = 0;
    time_flg.key_flag = 1;
}

/**
  * @brief  EXTI13 IRQ handler
  * @retval None
  */
void EXTI13_IRQHandler()
{
    ald_gpio_exti_clear_flag_status(GPIO_PIN_13);
}


#ifdef ALD_DMA
/**
  * @brief  DMA IRQ#66 handler
  * @retval None
  */
void DMA_Handler(void)
{
    ald_dma_irq_handler();
}
#endif /* ALD_DMA */

/**
  * @brief  I2c Event IRQ handler
  * @retval None
  */
void I2C1_EV_IRQHandler(void)
{
    ald_i2c_ev_irq_handler(&g_h_i2c);
}

/**
  * @brief  I2c Error IRQ handler
  * @retval None
  */
void I2C1_ERR_IRQHandler(void)
{
    ald_i2c_er_irq_handler(&g_h_i2c);
}

/**
  * @brief  AD16C4T1_UP Handler
  * @retval None
  */
void AD16C4T1_UP_IRQHandler(void)
{
    ald_timer_irq_handler(&g_ad16c4t_init);
}

/**
  * @brief  Uart0 handler
  * @retval None
  */
void UART0_IRQHandler(void)
{
    ald_uart_irq_handler(&g_h_uart);
    return;
}

/**
  * @brief ADC_IRQHandler.
  * @param none
  * @retval none
  */
void ADC_IRQHandler(void)
{
    /* Handle adc interrupt */
    ald_adc_irq_handler(&g_h_adc);
    g_adc_result = ald_adc_normal_get_value(&g_h_adc) * 3320 * 2 / 4096;
    ES_LOG_PRINT("adc: %u mV\n", g_adc_result);
}
/**
  * @}
  */
/**
  * @}
  */

/************* (C) COPYRIGHT Eastsoft Microelectronics *****END OF FILE****/
