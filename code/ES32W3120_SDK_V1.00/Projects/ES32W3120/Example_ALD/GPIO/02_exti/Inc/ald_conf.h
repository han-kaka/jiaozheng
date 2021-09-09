/**********************************************************************************
 *
 * @file    ald_conf.h
 * @brief   Enable/Disable the peripheral module.
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


#ifndef __ALD_CONF_H__
#define __ALD_CONF_H__

/* Includes ------------------------------------------------------------------ */

/* #define ALD_ADC */
#define ALD_BKPC
/* #define ALD_CALC */
#define ALD_CMU
/* #define ALD_CRC */
/* #define ALD_DAC */
/* #define ALD_DMA */
/* #define ALD_FLASH */
#define ALD_GPIO
/* #define ALD_I2C */
/* #define ALD_I2S */
/* #define ALD_LPTIM */
/* #define ALD_PIS */
#define ALD_PMU
/* #define ALD_QSPI */
/* #define ALD_RMU */
/* #define ALD_RTC */
/* #define ALD_SPI */
#define ALD_SYSCFG
/* #define ALD_TIMER */
/* #define ALD_UART */
/* #define ALD_WDT */
/* #define ALD_TYPE */
#define ALD_UTILS

#ifdef  ALD_ADC
    #include    "ald_adc.h"
#endif /* ALD_ADC */

#ifdef  ALD_BKPC
    #include    "ald_bkpc.h"
#endif /* ALD_BKPC */

#ifdef  ALD_CALC
    #include    "ald_calc.h"
#endif /* ALD_CALC */

#ifdef  ALD_CMU
    #include    "ald_cmu.h"
#endif /* ALD_CMU */

#ifdef  ALD_CRC
    #include    "ald_crc.h"
#endif /* ALD_CRC */

#ifdef  ALD_DAC
    #include    "ald_dac.h"
#endif /* ALD_DAC */

#ifdef  ALD_DMA
    #include    "ald_dma.h"
#endif /* ALD_DMA */

#ifdef  ALD_FLASH
    #include    "ald_flash.h"
#endif /* ALD_FLASH */

#ifdef  ALD_GPIO
    #include    "ald_gpio.h"
#endif /* ALD_GPIO */

#ifdef  ALD_I2C
    #include    "ald_i2c.h"
#endif /* ALD_I2C */

#ifdef  ALD_I2S
    #include    "ald_i2s.h"
#endif /* ALD_I2S */

#ifdef  ALD_LPTIM
    #include    "ald_lptim.h"
#endif /* ALD_LPTIM */

#ifdef  ALD_PIS
    #include    "ald_pis.h"
#endif /* ALD_PIS */

#ifdef  ALD_PMU
    #include    "ald_pmu.h"
#endif /* ALD_PMU */

#ifdef  ALD_QSPI
    #include    "ald_qspi.h"
#endif /* ALD_QSPI */

#ifdef  ALD_RMU
    #include    "ald_rmu.h"
#endif /* ALD_RMU */

#ifdef  ALD_RTC
    #include    "ald_rtc.h"
#endif /* ALD_RTC */

#ifdef  ALD_SPI
    #include    "ald_spi.h"
#endif /* ALD_SPI */

#ifdef  ALD_SYSCFG
    #include    "ald_syscfg.h"
#endif /* ALD_SYSCFG */

#ifdef  ALD_TIMER
    #include    "ald_timer.h"
#endif /* ALD_TIMER */

#ifdef  ALD_UART
    #include    "ald_uart.h"
#endif /* ALD_UART */

#ifdef  ALD_WDT
    #include    "ald_wdt.h"
#endif /* ALD_WDT */

#ifdef  ALD_TYPE
    #include    "type.h"
#endif /* ALD_TYPE */

#ifdef  ALD_UTILS
    #include    "utils.h"
#endif /* ALD_UTILS */

/* Exported Types ------------------------------------------------------------ */

/* Exported Macros ----------------------------------------------------------- */

#define TICK_INT_PRIORITY   3

/* Exported Variables -------------------------------------------------------- */

/* Exported Constants -------------------------------------------------------- */

/* Exported Functions -------------------------------------------------------- */

#endif /* _ALD_CONF_H */


/************* (C) COPYRIGHT Eastsoft Microelectronics *****END OF FILE****/
