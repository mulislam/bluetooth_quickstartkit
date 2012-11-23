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
/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/
/** @file
@brief Implementation of the User interface library.
*/

#include "hal_platform.h"
#include "hal_io.h"
#include "lib_ui.h"
#include "lib_traces.h"
#include "lib_timed_service.h"
#include "lib_dispatcher.h"
#include "system.h"
#include "assertions.h"

//static uint8_t lib_ui_button_tick_counter;
typedef enum lib_ui_led_index_onoff_t{
  LIB_UI_INDEX_NB_TICKS_OFF,
  LIB_UI_INDEX_NB_TICKS_ON
}lib_ui_led_index_onoff_t;

typedef struct lib_ui_led_info_t{
  uint8_t nb_ticks[2];
  bool is_on;
  uint8_t delay_before_next_action;
}lib_ui_led_info_t;

static lib_ui_led_info_t leds_array[LIB_UI_NB_LEDS];
static uint8_t nb_active_leds;


#ifndef HANDLE_LIB_UI_HANDLE_LED_IS_DEFINED
#define HANDLE_LIB_UI_HANDLE_LED (NB_MAX_MSG + 40)
#endif


static void lib_ui_handle_led(lib_ui_led_t led)
{
  switch (led)
  {
    case LIB_UI_LED_0 :
      HAL_IO_SET_STATE(HAL_IO_LED0, leds_array[led].is_on);
    break;
#ifdef HAL_IO_LED1    
    case LIB_UI_LED_1 :
      HAL_IO_SET_STATE(HAL_IO_LED1, leds_array[led].is_on);
    break;
#endif //HAL_IO_LED1    
#ifdef HAL_IO_LED2    
    case LIB_UI_LED_2 :
      HAL_IO_SET_STATE(HAL_IO_LED2, leds_array[led].is_on);
    break;
#endif //HAL_IO_LED2    
#ifdef HAL_IO_LED3    
    case LIB_UI_LED_3 :
      HAL_IO_SET_STATE(HAL_IO_LED3, leds_array[led].is_on);
    break;
#endif //HAL_IO_LED3    
    case LIB_UI_BUZZER :
#ifdef HAL_IO_BUZZER
      HAL_IO_SET_STATE(HAL_IO_BUZZER, leds_array[led].is_on);
#endif
    break;
  }
}

static void lib_ui_trig_next_led_action(uint8_t led, uint8_t nb_tick)
{
  if (0 != nb_tick)
  {
    if (0 == nb_active_leds)
    {
      lib_ui_trig_delayed_event_hook(1, HANDLE_LIB_UI_HANDLE_LED, LOW_PRIORITY);
    }
    if (0 == leds_array[led].delay_before_next_action)
    {
      nb_active_leds++;
    }
    leds_array[led].delay_before_next_action = nb_tick;
  }
}

void lib_ui_init(void)
{
  uint8_t led_idx;
  for(led_idx = 0; led_idx < LIB_UI_NB_LEDS; led_idx ++)
  {
    leds_array[led_idx].nb_ticks[LIB_UI_INDEX_NB_TICKS_OFF] = 0;
    leds_array[led_idx].nb_ticks[LIB_UI_INDEX_NB_TICKS_ON]  = 0;
    leds_array[led_idx].is_on                               = false;
    leds_array[led_idx].delay_before_next_action            = 0;
  }
  nb_active_leds = 0;
}

void lib_ui_turnon_led(lib_ui_led_t led, uint8_t nb_ticks_on)
{
  leds_array[led].nb_ticks[LIB_UI_INDEX_NB_TICKS_OFF] = 0;
  leds_array[led].nb_ticks[LIB_UI_INDEX_NB_TICKS_ON]  = 0;
  leds_array[led].is_on                               = true;
  if (0 != leds_array[led].delay_before_next_action)
  {
    leds_array[led].delay_before_next_action = 0;
    nb_active_leds--;
  }
  lib_ui_handle_led(led);
  lib_ui_trig_next_led_action(led, nb_ticks_on);
}

void lib_ui_turnoff_led(lib_ui_led_t led)
{
  leds_array[led].nb_ticks[LIB_UI_INDEX_NB_TICKS_OFF] = 0;
  leds_array[led].nb_ticks[LIB_UI_INDEX_NB_TICKS_ON]  = 0;
  leds_array[led].is_on = false;
  lib_ui_handle_led(led);
  if (0 != leds_array[led].delay_before_next_action)
  {
    nb_active_leds --;
    leds_array[led].delay_before_next_action = 0;
  }
}

void lib_ui_blink_led(lib_ui_led_t led, uint8_t nb_ticks_on, uint8_t nb_tick_off)
{
  leds_array[led].nb_ticks[LIB_UI_INDEX_NB_TICKS_ON] = nb_ticks_on;
  leds_array[led].nb_ticks[LIB_UI_INDEX_NB_TICKS_OFF] = nb_tick_off;
  leds_array[led].is_on = true;
  lib_ui_handle_led(led);
  lib_ui_trig_next_led_action(led, nb_ticks_on);
}

void lib_ui_on_handle_led()
{
  uint8_t led_idx;

  if (0 != nb_active_leds)
  {
    for(led_idx = 0; led_idx < LIB_UI_NB_LEDS; led_idx ++)
    {
      if (0 != leds_array[led_idx].delay_before_next_action)
      {
        leds_array[led_idx].delay_before_next_action--;
        if (0 == leds_array[led_idx].delay_before_next_action)
        {
          nb_active_leds--;
          if (leds_array[led_idx].is_on)
          {
            leds_array[led_idx].is_on = false;
            leds_array[led_idx].delay_before_next_action = leds_array[led_idx].nb_ticks[LIB_UI_INDEX_NB_TICKS_OFF];
          }
          else
          {
            leds_array[led_idx].is_on = true;
            leds_array[led_idx].delay_before_next_action = leds_array[led_idx].nb_ticks[LIB_UI_INDEX_NB_TICKS_ON];
          }
          lib_ui_handle_led((lib_ui_led_t)led_idx);
          if (0 != leds_array[led_idx].delay_before_next_action)
          {
            nb_active_leds++;
          }
        }
      }
    }
    if (0 != nb_active_leds)
    {
      lib_ui_trig_delayed_event_hook(1, HANDLE_LIB_UI_HANDLE_LED, LOW_PRIORITY);
    }
  }
}

bool lib_ui_read_handle_button(lib_ui_button_t button)
{
  uint8_t state;
  switch (button)
  {
    case LIB_UI_BUTTON_0 :
      state = HAL_IO_READ(HAL_IO_BUTTON_0);
      if (state == 1)
      {
        buttons_array[button].is_pressed = false;
      }
      else
      {
        buttons_array[button].is_pressed = true;
      }
    break;
  }
  return state;
}

#ifndef HANDLE_LIB_UI_HANDLE_BUTTON_IS_DEFINED
#define HANDLE_LIB_UI_HANDLE_BUTTON (NB_MAX_MSG + 41)
#endif

#ifndef HANDLE_EVT_BUTTON_RELEASED_IS_DEFINED
#define HANDLE_EVT_BUTTON_RELEASED (NB_MAX_MSG +42)
#endif

lib_ui_button_tick_callback_t *p_stored_callback_list;
uint8_t callback_size;
void lib_ui_start_check_button(lib_ui_button_tick_callback_t *p_callback_list, uint8_t size)
{
  uint8_t current_button = 0;
  callback_size = size;
  p_stored_callback_list = p_callback_list;
  while (current_button < LIB_UI_NB_BUTTONS)
  {
    buttons_array[current_button].nb_ticks = 0;
    buttons_array[current_button].pos = 0;
    lib_ui_read_handle_button((lib_ui_button_t)current_button);
    current_button++;
  }
  lib_dispatcher_post_msg((uint8_t)HANDLE_LIB_UI_HANDLE_BUTTON, NORMAL_PRIORITY);
}

void lib_ui_on_handle_button()
{
  uint8_t current_button = 0;
  while (current_button < LIB_UI_NB_BUTTONS)
  {
    if (buttons_array[current_button].is_pressed == false)
    {
      lib_ui_read_handle_button((lib_ui_button_t)current_button);
      if (buttons_array[current_button].is_pressed == true)
      {
        buttons_array[current_button].nb_ticks = 0;
        buttons_array[current_button].pos = 0;
      }
    }
    else
    {
      lib_ui_read_handle_button((lib_ui_button_t)current_button);
      
      if (buttons_array[current_button].is_pressed == true)
      {
        buttons_array[current_button].nb_ticks += 1;
        if (buttons_array[current_button].pos+1 > callback_size)
        {
        
        }
        else
        {
          if (buttons_array[current_button].nb_ticks >= p_stored_callback_list[buttons_array[current_button].pos].num_ticks)
          {
            lib_dispatcher_post_msg(p_stored_callback_list[buttons_array[current_button].pos].function_handle,NORMAL_PRIORITY);
            buttons_array[current_button].pos++;
          }
        }
      }
      else
      {
        lib_dispatcher_post_msg((uint8_t)HANDLE_EVT_BUTTON_RELEASED,NORMAL_PRIORITY);
      }
    }
    current_button++;
  }
  lib_timed_service_trig_delayed_event(BUTTON_CHECK_INTERVAL, HANDLE_LIB_UI_HANDLE_BUTTON, NORMAL_PRIORITY);
}
