/**********************************************************************************
 *
 * @file    main.c
 * @brief   Main file for DEMO
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
#include <string.h>
#include "main.h"
#include "eslog_init.h"

#include "global.h"

#include "bsp_system.h"

#include "task_common.h"

#include "bsp_dx_bt24_t.h"

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

/*************************************************************************************************/
/*
	Function	:
	Description	:
	Input		:
	Output		:
	Return		:
	Notes		:
*/
/*************************************************************************************************/
static void log_init(void)
{
    ESLOG_DEFAULT_INIT();
}

/**
  * @brief  Test main function
  * @retval Status.
  */
int main()
{
    /* Initialize ALD */
    ald_cmu_init();
    /* Configure system clock */
    ald_cmu_pll1_config(32);
    ald_cmu_clock_config(CMU_CLOCK_PLL1, 48000000);

    ald_cmu_perh_clock_config(CMU_PERH_ALL, ENABLE);

    log_init();

    /* 初始化IO */
    init_system();
    /* 开启一些初始任务 */
    start_init_task();
    
    while(1)
    {
        /* 主回路任务处理 */
        while(g_Maintask)
        {
            uint8_t m_temp = ga_TaskMapTable[g_Maintask];
            Task_Struct[m_temp].function(m_temp);
        }
        
        ble_test();
    }

//    gpio_init_t x;
//    exti_init_t exti;

//    memset(&exti, 0, sizeof(exti));
//    memset(&x, 0, sizeof(x));

//    /* Initialize ALD */
//    ald_cmu_init();
//    /* Configure system clock */
//    ald_cmu_pll1_config(32);
//    ald_cmu_clock_config(CMU_CLOCK_PLL1, 48000000);

//    ald_cmu_perh_clock_config(CMU_PERH_ALL, ENABLE);

//    /* Initialize GPIO_PIN_0 as output */
//    x.mode = GPIO_MODE_OUTPUT;
//    x.odos = GPIO_PUSH_PULL;
//    x.pupd = GPIO_PUSH_UP;
//    x.odrv = MD_GPIO_OUT_DRIVE_NORMAL;
//    x.flt  = GPIO_FILTER_DISABLE;
//    x.type = GPIO_TYPE_CMOS;
//    x.func = GPIO_FUNC_1;
//    ald_gpio_init(GPIOA, GPIO_PIN_7, &x);

//    x.mode = GPIO_MODE_INPUT;
//    x.odos = GPIO_PUSH_PULL;
//    x.pupd = GPIO_PUSH_DOWN;
//    x.odrv = MD_GPIO_OUT_DRIVE_NORMAL;
//    x.flt  = GPIO_FILTER_DISABLE;
//    x.type = GPIO_TYPE_CMOS;
//    x.func = GPIO_FUNC_1;
//    ald_gpio_init(GPIOA, GPIO_PIN_6, &x);

//    exti.filter      = ENABLE;
//    exti.cks         = EXTI_FILTER_CLOCK_10K;
//    exti.filter_time = 10;
//    ald_gpio_exti_init(GPIOA, GPIO_PIN_6, &exti);

//    /* Clear interrupt flag */
//    ald_gpio_exti_clear_flag_status(GPIO_PIN_6);
//    /* Configure interrupt */
//    ald_gpio_exti_interrupt_config(GPIO_PIN_6, EXTI_TRIGGER_RISING_EDGE, ENABLE);

//    __NVIC_EnableIRQ(EXTI6_IRQn);

//    while (1)
//    {
//        ald_delay_ms(10);
//    }
}

/**
  * @}
  */
/**
  * @}
  */

/************* (C) COPYRIGHT Eastsoft Microelectronics *****END OF FILE****/
