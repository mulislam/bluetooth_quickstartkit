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

/** @file
 * @brief Interface for hal_power
 */
 
/** @defgroup hal_power hal_power
@{
@ingroup hal
 
@brief Functions for entering power down mode and handling timer (RTC2 on nRF8200)

*/

#ifndef HAL_POWER_H__
#define HAL_POWER_H__

#include "hal_platform.h"

typedef enum start_source_t{
  START_FROM_RESET = 0x00,
  START_FROM_LONG_SLEEP = 0x2E
}start_source_t;


extern volatile start_source_t g_hal_power_startup_source;


typedef enum hal_power_time_t{
  SLEEP_1_MS      = 0x001F,
  SLEEP_2_MS      = 0x0040,
  SLEEP_5_MS      = 0x00A2,
  SLEEP_10_MS     = 0x0146,
  SLEEP_20_MS     = 0x028C,
  SLEEP_50_MS     = 0x0665,
  SLEEP_100_MS    = 0x0CCB,
  SLEEP_200_MS    = 0x1998,
  SLEEP_250_MS    = 0x1FFF,
  SLEEP_500_MS    = 0x3FFF,
  SLEEP_1000_MS   = 0x7FFF,
  SLEEP_1500_MS   = 0xBFFF,
  SLEEP_2000_MS   = 0xFFFF,
  SLEEP_NO_TIMER  = 0x0000
}hal_power_time_t;

typedef enum hal_power_wdt_time_t{
  HAL_PWR_WDT_1_SEC  ,
  HAL_PWR_WDT_250_MS ,
  HAL_PWR_WDT_16_MS  ,
  HAL_PWR_WDT_2_MS   
}hal_power_wdt_time_t;

/** @brief Timer interrupt subroutine.
 *  @details
 * This function stops/clears the timer (RTC2 on nRF8200). t has to be called on the timer interrupt vector.
 */
void hal_power_on_timer_isr(void);


/**@brief Start timer function.
 *  @details
 * Call this function to start the timer (RTC2 on nRF8200). After the given time, 
 * the timer will generate a timer (RTC2 on nRF8200) interrupt.
 * @param sleep_time Time after which the timer (RTC2 on nRF8200) will expire.
*/
void hal_power_start_timer(const hal_power_time_t sleep_time);

/**@brief Reset timer function.
 *  @details
 * Call this function to stop and reset the timer.
*/
void hal_power_reset_timer(void);

/**@brief Puts the MCU in sleep mode (register retention).
 *  @details
 * The function supports pin and timer wake-up. Pin wake-up is always enabled.
 * To sleep for a specific time use the hal_power_start_timer() function to start the timer, (RTC2 on nRF8200)
 * then hal_power_sleep will also enable wake-up from timer.
*/
void hal_power_sleep();

/**@brief Long sleep.
 *  @details
 * Call this function to put the device in memory retention mode. The device will wake-up either
 * after the given period of time or on a wake-up from pin. On wake-up, the device starts from reset.
 * @param sleep_time Number of seconds after which the device should wake-up. If set to 0, then no timer is set, and the device will wake-up only on a wake-up from pin event.
*/
void hal_power_long_sleep(const uint16_t sleep_time);

/**@brief Check wake-up source.
 *  @details
 * Call this function after a reset to check if the wake-up is from a hard reset or
 * from memory retention mode. If the reset is from memory retention mode, this funcion checks
 * if the delay for wake-up has expired.
 * @return True if the wake-up was from deep_sleep, false otherwise.
*/
bool hal_power_check_wakeup();

/**@brief Set up a timout using the watchdog.
 *  @details
 * Call this function to start a timeout using the watchdog functionality.
 * @param timeout duration (can be either HAL_PWR_WDT_1_SEC, HAL_PWR_WDT_250_MS, HAL_PWR_WDT_16_MS or HAL_PWR_WDT_2_MS).
*/
void hal_power_start_wdt_timeout(hal_power_wdt_time_t timeout);

/**@brief Stops and cancels the current running timeout watchdog.
 *  @details
 * Call this function to stop a timeout that has previously been started using hal_power_start_wdt_timeout().
*/
void hal_power_stop_wdt_timeout(void);

/**@brief Temporarily disable the timer interrupt.
 *  @details
 * This function memorizes the current enabling/disabling state of the timer interrupt and then disables it.
 * To restore the state of the timer interrupt, call hal_power_restore_timer_it().
*/
void hal_power_disable_timer_it();

/**@brief Restores the previous enabled/disabled state of the timer interrupt.
 *  @details
 * This function restores the memorized enabled/disabled state of the timer interrupt 
 * memorized by the previous call to hal_power_disable_timer_it().
*/
void hal_power_restore_timer_it();

#endif // HAL_PWR_H__
/** @} */

