/* Copyright (c) 2010 Nordic Semiconductor. All Rights Reserved.
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
/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/
/** @file
 * @brief timed service library
 */
 
/** @defgroup lib_timed_services lib_timed_services
* @{
* @ingroup lib
*  
* @brief library to provide timed services.
* @details 
* This library uses @ref hal_power timer functionalities to provide an easy way of
* handling delayed events.
* It can handle several delayed events each with a given timing. The delay of each event is given in tick mutliples.
* The default tick is 20&nbsp;ms (LIB_TIMED_SERVICE_TICK), and can be modified
* using the function @ref lib_timed_service_set_tick or @ref lib_timed_service_set_tick_ms. 
* To trigger an event after a given delay, you can use the function @ref lib_timed_service_trig_delayed_event.
* when the given delay is expired, lib_timed_service calls the hook function @ref lib_timed_service_hook.
*/

 #ifndef LIB_TIMED_SERVICES_H__
 #define LIB_TIMED_SERVICES_H__

#include "hal_platform.h"
#include "hal_power.h"

#define LIB_TIMED_SERVICE_TICK SLEEP_20_MS /**Base interval, all events delays will be multiple of this time.*/
#define NB_MAX_DELAYED_MSG 20              /**number of simultaneous delayed message.*/


/** @brief Function to retrieve the current value of the tick.
 *  @return Current tick value.
 */
hal_power_time_t lib_timed_service_get_tick();

/** @brief Function to retrieve the current value of the tick in milliseconds.
 *  @return A 16-bit value corresponding to the number of milliseconds of the current tick value.
 */
uint16_t lib_timed_service_get_tick_ms();
 
/** @brief Function to set the value of the tick.
 *  @param tick Tick value to set.
 */
void lib_timed_service_set_tick(hal_power_time_t tick);

/** @brief Function to set the value of the tick.
 *  @param tick_in_ms Tick value in milliseconds to set.
 */
void lib_timed_service_set_tick_ms(uint16_t tick_in_ms);

/** @brief Event triggered hook function.
 *  @details A hook function that must be implemented by the client of this module. 
 *  The function is called for each delayed event after the delay has expired.
 *  @param identifier The event identifier which was given by the call to lib_timed_service_trig_delayed_event.
 *  @param extra_info Extra information byte (For example, it can be used for priority information).
 */
void lib_timed_service_hook(uint8_t identifier, uint8_t extra_info);

/** @brief Function to trigger a delayed event.
 *  @details Use this function to trigger an event after a given period of time (a multiple of the current tick set by
 *  lib_timed_service_set_tick or lib_timed_service_set_tick_ms). The lib_timed_service_hook will be called with the 
 *  corresponding parameter after the given number of tick.
 *  @param delay_nb_tick Number of tick after which the event is trigged.
 *  @param delayed_event_id Identifier of the event.
 *  @param extra_info Extra information byte (For example, it can be used for priority information).
 */
void lib_timed_service_trig_delayed_event(uint16_t delay_nb_tick, uint8_t delayed_event_id, uint8_t extra_info);

/** @brief Function to be called on timer interrupt.*/
void lib_timed_service_tick_wakeup();

/** Function to be initialize the timed_service.*/
void lib_timed_service_init();

/** Function to cancel a delayed event.*/
void lib_timed_service_cancel_delayed_event(uint8_t delayed_event_id);


#endif // LIB_TIMED_SERVICES_H__
/** @} */
