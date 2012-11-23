/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 * $LastChangedRevision$
 */

#ifndef SYSTEM_H__
#define SYSTEM_H__

#include "hal_io.h"

/*defines for ACI I/O configuration (see hal_io.h)*/
#define HAL_IO_RADIO_CSN       HAL_IO_P00
#define HAL_IO_RADIO_ACTIVE    HAL_IO_P01
#define HAL_IO_RADIO_REQN      HAL_IO_P02
#define HAL_IO_RADIO_RDY       HAL_IO_P06
#define HAL_IO_RADIO_SCK       HAL_IO_P14
#define HAL_IO_RADIO_MOSI      HAL_IO_P15
#define HAL_IO_RADIO_MISO      HAL_IO_P16

#define HAL_IO_LED0            HAL_IO_P10
#define HAL_IO_LED1            HAL_IO_P11
#define HAL_IO_BUTTON_0        HAL_IO_P12
#define HAL_IO_BUZZER          HAL_IO_P13
#define HAL_IO_LED2            HAL_IO_P04
#define HAL_IO_LED3            HAL_IO_P05
#define HAL_IO_LED4            HAL_IO_P07

#define ENABLE_INTERRUPTS()       do { EA = 1; } while(0)
#define DISABLE_INTERRUPTS()      do { EA = 0; } while(0)
#define ARE_INTERRUPTS_ENABLED()  EA

void system_init(void);

#endif // SYSTEM_H__
