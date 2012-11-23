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
 * @brief My Application header file
*/

/** @defgroup my_project my_project
@{
@ingroup projects
@brief Empty project without lib_dispatcher that can be used as a template for new projects.

@details
This project is a firmware template for new projects without using the lib_dispatcher. 
The project will run correctly in its current state, but does nothing. 
With this project you have a starting point for adding your own application functionality.

The following instructions describe how to set up the nRFgo Motherboard (the names in 
bold refer to the silk print on the Motherboard):
 -# Mount the carrier board and SCC module on the nRFgo Motherboard as depicted in the
    figure below.
 -# Connect the I/O port headers to the Button and LED port headers using the 2-lead 
    patch cables.
    - <b>P1.2 - Button 0:</b> Wake-up from deep-sleep. (The sleep time is 10 seconds, so this 
      can be used for immediate wake-up.)
    - <b>P1.0 - LED 0:</b> Continuously lit when advertising, blinks while connected, 
      otherwise it is off.
    - <b>P1.1 - LED 1:</b> Off when there is no alert. Blinking signal when mild alert. 
      When high alert the LED is continuously lit.
 -# Program the nRF8200 on the carrier board (nRF2735) with the My_project application; 
    the HEX file is called my_project_nrf8200.hex and is located of the SDK's 
    precompiled HEX folder. Use nRFgo Studio for programming.
 -# Install the Master Control Panel on your computer. Connect the Master Emulator 
    (nRF2739) and make sure the hardware drivers are installed.
    
@image html sdk_userguide_proximity_example.jpg The nRFgo Motherboard with the carrier board and SCC module mounted

 */


#ifndef MY_APPLICATION_H__
#define MY_APPLICATION_H__

/** Application states */
typedef enum my_project_app_state_t {
  APP_INIT,               /**< Initialization */
  APP_SLEEP,              /**< Wake-up from Sleep state */
  APP_SETUP,              /**< Send configuration upload */
  APP_SETUP_FINISHED,     /**< Wait for event device_started in standby*/
  APP_STANDBY,            /**< Start advertising */
  APP_RUN,                /**< In Run state */
  APP_GOTO_SLEEP          /**< Go to Sleep*/
} my_project_app_state_t;

extern my_project_app_state_t app_state;

#define APP_RUN_ADV_PERIOD   600  /**< advertising period in multiple of 0.625 msec, must be between 0x0020 and 0x4000.*/
#define APP_ADVERTISING_TIME  30  /**< time during which the radio will advertise, in seconds */
#define APP_TIME_SLEEP        10  /**< time during which the device will stay in deep_sleep, in seconds */

/** @brief main procesing function.
 */
void on_process_app(void);

void radio_evt_error(void);

 
 
#endif // MY_APPLICATION_H__

//@}
