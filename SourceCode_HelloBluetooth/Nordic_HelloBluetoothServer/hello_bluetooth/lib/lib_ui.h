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
 * @brief user interface module
*/

/** @defgroup lib_ui lib_ui
 * @{
 * @ingroup lib
 *  
 * @brief user interface module
 * @details 
 * This module uses the lib_dispatcher which allows interaction with up to four LEDs, one buzzer, and one button. 
 * The LEDs can be turned on for a set length of time, or blink for a set length of time, depending on your specifications. 
 * To access the LED control, you must define the handle HANDLE_LIB_UI_HANDLE_LED and set lib_ui_on_handle_led as subscriber. 
 *
 * In the dispatcher tool, it looks as shown below: 
 * <TABLE>
 * <TR><TD></TD><TD>Handle</TD><TD>Subscriber 0</TD><TD>Subscriber 1</TD><TD>Subscriber 2</TD></TR>
 * <TR><TD>ww</TD><TD>HANDLE_ww</TD><TD>ww</TD><TD>NULL</TD><TD>NULL</TD></TR>
 * <B>
 * <TR><TD>xx</TD><TD>HANDLE_LIB_UI_HANDLE_LED</TD><TD>lib_ui_on_handle_led</TD><TD>NULL</TD><TD>NULL</TD></TR>
 * </B>
 * <TR><TD>zz</TD><TD>HANDLE_zz</TD><TD>zz</TD><TD>NULL</TD><TD>NULL</TD></TR>
 * </TABLE>
 * 
 * The buttons are registered with the lib_ui_start_check_button. The input is a list of structures that
 * include the required number of ticks for a button press, and which function is called when that number of ticks occur. 
 * When the button is released, it calls button released {needs formatting to indicate command}, 
 * and performs the action related to the number of button presses. This module uses the lib_dispatcher. 
 * To access the button control, define the handle HANDLE_LIB_UI_HANDLE_BUTTON and indicate lib_ui_on_handle_button as a subscriber.  
 * Define HANDLE_EVT_BUTTON_RELEASED with a function of your choice as a subscriber for it. 
 * 
 * In the dispatcher tool, it looks as shown below:
 * <TABLE>
 * <TR><TD></TD><TD>Handle</TD><TD>Subscriber 0</TD><TD>Subscriber 1</TD><TD>Subscriber 2</TD></TR>
 * <TR><TD>ww</TD><TD>HANDLE_ww</TD><TD>ww</TD><TD>NULL</TD><TD>NULL</TD></TR>
 * <B>
 * <TR><TD>xx</TD><TD>HANDLE_LIB_UI_HANDLE_BUTTON</TD><TD>lib_ui_on_handle_button</TD><TD>NULL</TD><TD>NULL</TD></TR>
 * <TR><TD>xx</TD><TD>HANDLE_EVT_BUTTON_RELEASED</TD><TD>on_button_released</TD><TD>NULL</TD><TD>NULL</TD></TR>
 * </B>
 * <TR><TD>zz</TD><TD>HANDLE_zz</TD><TD>zz</TD><TD>NULL</TD><TD>NULL</TD></TR>
 * </TABLE>
 * 
 * 
 * <h4>Requirements</h4>
 * <h3>HAL</h3>
 * This library uses @ref hal_io macro to act upon outputs:
 *  - HAL_IO_SET_STATE(io_name, io_state) which set the given output to the corresponding level;
 * 
 * <h3>LIB</h3>
 * This library uses the @ref lib_dispatcher to post messages (@ref lib_dispatcher_post_msg) and has callback for handle HANDLE_LIB_UI_HANDLE_LED.
*/

#ifndef LIB_UI_H__
#define LIB_UI_H__
 
#include "hal_platform.h"

/** Number of ticks between each check of center button state for long press detection(multiple of tick defined in @ref lib_timed_services - 20&nbsp;ms by default).*/    
#define BUTTON_CHECK_INTERVAL 10

typedef enum lib_ui_led_t{
  LIB_UI_LED_0,
  LIB_UI_LED_1,
  LIB_UI_LED_2,
  LIB_UI_LED_3,
  LIB_UI_BUZZER,
  LIB_UI_NB_LEDS
}lib_ui_led_t;

/** @brief Trig delayed event hook function.
 *  @details A trig delayed event hook function that must be implemented by the client of this module.
 * The function is expected to trigger an event after a given period of time. On this event, the
 * lib_ui_on_handle_led has to be called.
 *  @param delay_nb_tick Delay after which the event should be trigged.
 *  @param event_id Unique identifier.
 *  @param extra_info Event priority.
 */
extern void lib_ui_trig_delayed_event_hook(uint8_t delay_nb_tick, uint8_t event_id, uint8_t extra_info);

/** Function to initialize lib_ui.*/
void lib_ui_init(void);


/**@brief Turn on a LED.
 *  @details
 *  Call this function to turn on either LIB_UI_LED_0 or LIB_UI_LED_1 for a given period of time.
 *  @param led LED to turn on (LIB_UI_LED_0 or LIB_UI_LED_1).
 *  @param nb_ticks_on Number of dispatcher_tick before turning off the LED. If 0, the LED will stay on permanently.
*/
void lib_ui_turnon_led(lib_ui_led_t led, uint8_t nb_ticks_on);

/**@brief Turn off a LED.
 *  @details
 *  Call This function to turn off either LIB_UI_LED_0 or LIB_UI_LED_1.
 *  @param led LED to turn on (LIB_UI_LED_0 or LIB_UI_LED_1).
*/
void lib_ui_turnoff_led(lib_ui_led_t led);

/**@brief Blink a LED.
 *  @details
 *  Call this function to blink either LIB_UI_LED_0 or LIB_UI_LED_1. The blinking will be canceled by a call to lib_ui_turnon_led
 *  or to lib_ui_turnoff_led.
 *  @param led LED to turn on (LIB_UI_LED_0 or LIB_UI_LED_1).
 *  @param nb_ticks_on Number of dispatcher_tick during which the LED will be on.
 *  @param nb_tick_off Number of dispatcher_tick during which the LED will be off.
*/
void lib_ui_blink_led(lib_ui_led_t led, uint8_t nb_ticks_on, uint8_t nb_tick_off);

void lib_ui_on_handle_led();


/** Structure to keep track of number of ticks to trigger an action. */
typedef struct _lib_ui_button_tick_callback_t {
    uint8_t num_ticks;
    uint8_t function_handle;
} lib_ui_button_tick_callback_t;

/** Enum of buttons available.*/
typedef enum lib_ui_button_t{
  LIB_UI_BUTTON_0,
  LIB_UI_NB_BUTTONS
}lib_ui_button_t;

/** Structure to keep track of how many ticks a button have been pressed and if it is pressed. the pos is for future use of optimizing the algorithm.*/
typedef struct lib_ui_buttons_info_t{
  uint8_t nb_ticks;
  bool is_pressed;
  uint8_t pos;
}lib_ui_buttons_info_t;

/** Array that holds the buttons structures.*/
static lib_ui_buttons_info_t buttons_array[LIB_UI_NB_BUTTONS];

/**@brief Check the state of the button.
 * @details
 * Call this function to check the state of the button.
 * @param button The button having its state checked.
*/
bool lib_ui_read_handle_button(lib_ui_button_t button);
/**@brief Configure and start the button check.
 *  @details
 *  Call this function to start checking for button press the buttons LIB_UI_BUTTON_0. 
 *  @param p_callback_list Structure with the number of ticks to trigger and the function to run.
 *  @param size Number of configured callbacks.
*/
void lib_ui_start_check_button(lib_ui_button_tick_callback_t *p_callback_list, uint8_t size);

void lib_ui_on_check_button(void);

/** @} */

#endif

