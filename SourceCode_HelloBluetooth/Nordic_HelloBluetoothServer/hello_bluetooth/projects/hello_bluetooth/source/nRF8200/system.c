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

#include "hal_platform.h"
#include "system.h"
#include "hal_aci_tl.h"
#include "hal_power.h"


void system_init(void)
{
  hal_pltf_config_clk_ctrl_rc_oscillator();
  hal_aci_tl_io_config();

  HAL_IO_CONFIG(HAL_IO_LED0, HAL_IO_OUTPUT, HAL_IO_OUTPUT_NORMAL_STRENGTH);
  HAL_IO_CONFIG(HAL_IO_LED1, HAL_IO_OUTPUT, HAL_IO_OUTPUT_NORMAL_STRENGTH);

  HAL_IO_CONFIG(HAL_IO_BUTTON_0, HAL_IO_INPUT, HAL_IO_INPUT_BUF_ON_NO_PULL);

  HAL_IO_SET_STATE(HAL_IO_LED0, 0);
  HAL_IO_SET_STATE(HAL_IO_LED1, 0);
  
  hal_pltf_config_wkup_pin_low_polarity();
  hal_pltf_enable_wkup_rdy_line();
  hal_pltf_enable_wkup_p12();  
}


HAL_PLTF_INTERRUPT_HANDLER(pin_wakeup_isr, INTERRUPT_WUOPIRQ)
{
}


HAL_PLTF_INTERRUPT_HANDLER(tick_wakeup_isr, INTERRUPT_TICK)
{
  hal_power_on_timer_isr();
}
