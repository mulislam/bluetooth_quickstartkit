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
/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/
/** @file
 * @brief messages dispatcher module
 */
 
/** @defgroup lib_dispatcher lib_dispatcher
* @{
* @ingroup lib
*  
* @brief Message dispatcher module
* 
* The message dispatcher module allows an application to send messages and receive
* notifications when a message is dispatched. A message can be sent with 3 different levels of priority.
*
* To facilitate the use of this message dispatcher you can use the Dispatcher Tool in nRFgo Studio. 
* In this tool you can define handles (right click and 'add_handle'), name handles, and assign up 
* to 3 subscribers to each handle.
* 
* The subscribers are functions that are called when the corresponding handle is triggered.
* 
* The dispatcher must be initialized by a call to lib_dispatcher_init() and then the function
* lib_dispatcher_dispatch should be called in an infinite loop, as shown in the following example.
* @code
* void main(void)
* {
*   lib_dispatcher_init();
*   ENABLE_INTERRUPTS();
*   for (;;)
*   {
*     lib_dispatcher_dispatch();
*   }
* }
* @endcode
* 
* <h4>Requirements</h4>
* <h3>HAL</h3>
* This library uses functions from hal_power module for handling power management during idle periods:
*  - hal_power_sleep() which put the device in register_retention sleep, with timer if the timer has been started;
* <h3>LIB</h3>
* This library uses the @ref lib_traces library for @ref LIB_TRACES_LOG_INFO and @ref LIB_TRACES_LOG_ERROR macros as well as the @ref lib_assert library for @ref ASSERT macros.
* 
*/
 #ifndef DISPATCHER_H__
 #define DISPATCHER_H__

#include "hal_platform.h"

#include "dispatcher_config.h"
#include "system.h"

#define NB_MAX_EVT_PER_PRIORITY 20



typedef struct lib_dispatcher_data_t {
  uint8_t handle;
} lib_dispatcher_data_t;

/** @brief Message priorities of posted messages.*/
typedef enum lib_dispatcher_priority_t{
  HIGH_PRIORITY      = 0,
  NORMAL_PRIORITY,
  LOW_PRIORITY,
  NB_PRIORITIES
}lib_dispatcher_priority_t;

/** @brief Function to post an immediate message.
 *  @details Use this function to post a message without any delay. This message will be dispatched to all subscribers as soon as possible.
 *  @param msg_handle Message handle
 *  @param msg_priority Message priority.
 */
void lib_dispatcher_post_msg(uint8_t msg_handle, lib_dispatcher_priority_t msg_priority) KEYWORD_REENTRANT;

/** @brief Main dispatcher function.
 *  @details This function should be called in a permanent loop. It handles the dispatching of all pending messages.
 * When no more messages are pending, it goes in register retention sleep mode (@ref hal_power).
 */
void lib_dispatcher_dispatch(void);

/** Function to initialize the dispatcher.*/
void lib_dispatcher_init(void);

#endif // DISPATCHER_H__
/** @} */
