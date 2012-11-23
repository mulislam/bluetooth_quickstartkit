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
 * $LastChangedRevision: 4808 $
 */ 

/** @file
@brief Implementation of the power management module
*/
#include "hal_platform.h"
#include "hal_power.h"
#include "assertions.h"
#include "system.h"

/* This variable must be placed in register retention memory */
HAL_PLTF_MEM_RETENTION(volatile uint16_t, wakeup_counter, 0x0000)

/* This variable must be placed in register retention memory */
HAL_PLTF_MEM_RETENTION(volatile start_source_t, g_hal_power_startup_source, 0x0002)


static volatile bool timer_started = false;
sfr16 RTC2CMP = 0xb4;

static uint8_t m_timer_it_enable;

void hal_power_disable_timer_it()
{
  m_timer_it_enable = hal_pltf_get_timer_it_enable_state();
  hal_pltf_set_timer_it_enable_state(0);
}

void hal_power_restore_timer_it()
{
  hal_pltf_set_timer_it_enable_state(m_timer_it_enable);
}

static void hal_power_goto_deep_sleep(bool is_with_timer)
{
  timer_started = false;
  if (is_with_timer)
  {
    hal_pltf_enable_wakup_tick_pin();     //Enable wake-up on TICK and pin
    hal_power_start_timer(SLEEP_2000_MS);
  }
  else
  {
    hal_pltf_enable_wakup_pin();               //Enable wake-up on pin only
  }
  hal_pltf_sleep_memory_retention();               //Retention latch locked
                                                   //will reset on wake-up
}



void hal_power_on_timer_isr(void)
{
  hal_pltf_disable_timer();
  timer_started = false;
}

void hal_power_start_timer(const hal_power_time_t sleep_time)
{
  hal_power_disable_timer_it();
  if(!timer_started)
  {
    hal_pltf_disable_timer();
    timer_started = true;
    // Set the compare value
    hal_pltf_load_timer_compare_val(sleep_time);
    hal_pltf_set_timer_mode_it_enabled_reset_timer_on_it();   //set compare mode to -> rtc it enabled, timer reset on timer interrupt.
      /* Set timer wake-up */
    hal_pltf_set_timer_it_enable_state(1);
    hal_pltf_enable_timer();
  }
  else
  {
    hal_power_restore_timer_it();
  }
}

void hal_power_reset_timer(void)
{
  hal_pltf_set_timer_it_enable_state(0);
  hal_pltf_disable_timer();
  hal_pltf_load_timer_compare_val(0);
  timer_started = false;
  hal_pltf_set_timer_it_enable_state(0);
}

void hal_power_sleep()
{
  DISABLE_INTERRUPTS();
  if (timer_started)
  {
    hal_pltf_enable_wakup_tick_pin();     //Enable wake-up on TICK and pin
  }
  else
  {
    hal_pltf_enable_wakup_pin();               //Enable wake-up on pin only
  }
  ENABLE_INTERRUPTS();
  hal_pltf_sleep_register_retention();
}

void hal_power_long_sleep(const uint16_t sleep_time_s)
{
  g_hal_power_startup_source = START_FROM_LONG_SLEEP;

  hal_pltf_disable_wakeup_from_ready_line();
  
  if (0 != sleep_time_s)
  {
    wakeup_counter = sleep_time_s>>1;
    if (0 == wakeup_counter)
    {
      wakeup_counter = 1;
    }
    hal_power_goto_deep_sleep(true);
  }
  else
  {
    hal_power_goto_deep_sleep(false);
  }
}

bool hal_power_check_wakeup()
{
  hal_pltf_wake_up_delay();
  timer_started = false;
  if (START_FROM_LONG_SLEEP != g_hal_power_startup_source)
  {
    return(false);
  }
  if (0 != --wakeup_counter)
  {
    hal_power_goto_deep_sleep(true);
  }
  return(true);
}
