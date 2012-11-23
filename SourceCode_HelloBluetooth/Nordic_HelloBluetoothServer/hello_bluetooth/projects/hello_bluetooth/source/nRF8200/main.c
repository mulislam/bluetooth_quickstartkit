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
#include "hal_power.h"
#include "lib_aci.h"
#include "lib_traces.h"
#include "my_application.h"
#include "system.h"
#include "services.h"

void lib_aci_error_hook()
{
  radio_evt_error();
}


void main(void)
{
  bool reset_from_deep_sleep = false;

  system_init();

  app_state = APP_INIT;
  lib_aci_init();

  if ((0x00 == HAL_IO_READ(HAL_IO_BUTTON_0)))
  {
    LIB_TRACES_INIT();
    reset_from_deep_sleep = true;
    LIB_TRACES_LOG_INFO(INFO_CODE_WKUP_BUTTON);
  }
  else
  {
    reset_from_deep_sleep = hal_power_check_wakeup();
    LIB_TRACES_INIT();
  }

  ENABLE_INTERRUPTS();
  
  if (reset_from_deep_sleep)
  {
    app_state = APP_SLEEP;
    on_process_app();
  }
  else
  {
    LIB_TRACES_LOG_INFO(INFO_CODE_WKUP_TIMER);
  }
  
  g_hal_power_startup_source = START_FROM_RESET;

  LIB_TRACES_LOG_INFO(INFO_CODE_MAIN_STARTED);
  
  for (;;)
  {
    hal_aci_tl_poll_rdy_line();
    hal_power_sleep();
  }
}
