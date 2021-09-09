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

/** @addtogroup Projects_Examples_ALD
  * @{
  */

/** @addtogroup Examples
  * @{
  */

/* Private Macros ------------------------------------------------------------ */
/* Private Variables --------------------------------------------------------- */
/* Public Variables ---------------------------------------------------------- */
/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

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
  * @brief  EXTI6 IRQ handler
  * @retval None
  */
void EXTI6_IRQHandler()
{
//    ald_gpio_exti_clear_flag_status(GPIO_PIN_6);
//    ald_gpio_toggle_pin(GPIOA, GPIO_PIN_7);
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
  * @}
  */
/**
  * @}
  */

/************* (C) COPYRIGHT Eastsoft Microelectronics *****END OF FILE****/
