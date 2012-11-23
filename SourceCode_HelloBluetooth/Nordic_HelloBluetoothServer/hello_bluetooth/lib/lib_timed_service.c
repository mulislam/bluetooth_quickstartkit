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
@brief Implementation of a message dispatcher
*/

#include "hal_platform.h"
#include "hal_power.h"
#include "lib_timed_service.h"
#include "assertions.h"
#include "system.h"

#define DELAY_FREE_MSG 0xFFFF

typedef struct lib_timed_service_delayed_msg_t{
  uint8_t delayed_evt_id;
  uint8_t extra_info;
  uint16_t tick_delay;
}lib_timed_service_delayed_msg_t;

typedef struct lib_timed_service_delay_queue_t{
  lib_timed_service_delayed_msg_t msg_queue[NB_MAX_DELAYED_MSG];
  uint8_t nb_delayed_msg;
}lib_timed_service_delay_queue_t;

/*static code uint16_t time_convert_table_to_ms[] = {
  1   ,
  2   ,
  5   ,
  10  ,
  20  ,
  50  ,
  100 ,
  200 ,
  250 ,
  500 ,
  1000,
  1500,
  2000,
};*/

static lib_timed_service_delay_queue_t delayed_msg_queue;

static hal_power_time_t cur_tick;

void lib_timed_service_init()
{
  uint8_t index_queue;
  
  for (index_queue = 0; index_queue < NB_MAX_DELAYED_MSG; index_queue ++)
  {
    delayed_msg_queue.msg_queue[index_queue].tick_delay = DELAY_FREE_MSG;
    delayed_msg_queue.msg_queue[index_queue].delayed_evt_id = 0;
  }
  delayed_msg_queue.nb_delayed_msg = 0;
  cur_tick = LIB_TIMED_SERVICE_TICK;
}

hal_power_time_t lib_timed_service_get_tick()
{
  return(cur_tick);
}

uint16_t lib_timed_service_get_tick_ms()
{
  switch (cur_tick)
  {
  case SLEEP_1_MS    :
    return(1);
  case SLEEP_2_MS    :
    return(2);
  case SLEEP_5_MS    :
    return(5);
  case SLEEP_10_MS   :
    return(10);
  case SLEEP_20_MS   :
    return(20);
  case SLEEP_50_MS   :
    return(50);
  case SLEEP_100_MS  :
    return(100);
  case SLEEP_200_MS  :
    return(200);
  case SLEEP_250_MS  :
    return(250);
  case SLEEP_500_MS  :
    return(500);
  case SLEEP_1000_MS :
    return(1000);
  case SLEEP_1500_MS :
    return(1500);
  case SLEEP_2000_MS :
    return(2000);
  }
  return(0);
}

void lib_timed_service_set_tick_ms(uint16_t tick_in_ms)
{
  if (tick_in_ms < 2)
  {
    cur_tick = SLEEP_1_MS;
  }
  else if (tick_in_ms < 5)
  {
    cur_tick = SLEEP_2_MS;
  }
  else if (tick_in_ms < 10  )
  {
    cur_tick = SLEEP_5_MS;
  }
  else if (tick_in_ms < 20  )
  {
    cur_tick = SLEEP_10_MS;
  }
  else if (tick_in_ms < 50  )
  {
    cur_tick = SLEEP_20_MS;
  }
  else if (tick_in_ms < 100 )
  {
    cur_tick = SLEEP_50_MS;
  }
  else if (tick_in_ms < 200 )
  {
    cur_tick = SLEEP_100_MS;
  }
  else if (tick_in_ms < 250 )
  {
    cur_tick = SLEEP_200_MS;
  }
  else if (tick_in_ms < 500 )
  {
    cur_tick = SLEEP_250_MS;
  }
  else if (tick_in_ms < 1000)
  {
    cur_tick = SLEEP_500_MS;
  }
  else if (tick_in_ms < 1500)
  {
    cur_tick = SLEEP_1000_MS;
  }
  else if (tick_in_ms < 2000)
  {
    cur_tick = SLEEP_1500_MS;
  }
  else 
  {
   cur_tick = SLEEP_2000_MS;
  }
}

void lib_timed_service_set_tick(hal_power_time_t tick)
{
  cur_tick = tick;
}


void lib_timed_service_trig_delayed_event(uint16_t delay_nb_tick, uint8_t delayed_event_id, uint8_t extra_info)
{
  uint8_t delay_idx;

  ASSERT(ERROR_CODE_LIB_DISPATCHER_DELAYED_MSG_OVERFLOW, (delayed_msg_queue.nb_delayed_msg < NB_MAX_DELAYED_MSG));
  for (delay_idx = 0; delay_idx < NB_MAX_DELAYED_MSG; delay_idx ++)
  {
    if (DELAY_FREE_MSG == delayed_msg_queue.msg_queue[delay_idx].tick_delay)
    {
      delayed_msg_queue.msg_queue[delay_idx].delayed_evt_id = delayed_event_id;
      delayed_msg_queue.msg_queue[delay_idx].extra_info = extra_info;
      hal_power_disable_timer_it();       /*prevent the timer interrupt to mess up with the update of tick_delay and nb_delayed_msg*/
      delayed_msg_queue.msg_queue[delay_idx].tick_delay = delay_nb_tick;
      delayed_msg_queue.nb_delayed_msg ++;
      hal_power_restore_timer_it();       /*end of timer interrupt disabling*/
      hal_power_start_timer(cur_tick);
      return;
    }
  }
  LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_LIB_TIMED_SERVICE_DELAYED_QUEUE_CORRUPTED, delayed_event_id);
}


void lib_timed_service_tick_wakeup()
{
  uint8_t delay_idx;

  for (delay_idx = 0; ((delay_idx < NB_MAX_DELAYED_MSG)&&(0 != delayed_msg_queue.nb_delayed_msg)); delay_idx ++)
  {
    if (DELAY_FREE_MSG != delayed_msg_queue.msg_queue[delay_idx].tick_delay)
    {
      delayed_msg_queue.msg_queue[delay_idx].tick_delay--;
      if (0 == delayed_msg_queue.msg_queue[delay_idx].tick_delay)
      {
        delayed_msg_queue.msg_queue[delay_idx].tick_delay = DELAY_FREE_MSG;
        delayed_msg_queue.nb_delayed_msg--;
        lib_timed_service_hook(delayed_msg_queue.msg_queue[delay_idx].delayed_evt_id, 
                                delayed_msg_queue.msg_queue[delay_idx].extra_info);
      }
    }
  }
  if (0 != delayed_msg_queue.nb_delayed_msg)
  {
    hal_power_start_timer(cur_tick);
  }
}

void lib_timed_service_cancel_delayed_event(uint8_t delayed_event_id)
{
  uint8_t delay_idx;

  for (delay_idx = 0; delay_idx < NB_MAX_DELAYED_MSG; delay_idx ++)
  {
    if ((delayed_msg_queue.msg_queue[delay_idx].delayed_evt_id == delayed_event_id)&&(DELAY_FREE_MSG != delayed_msg_queue.msg_queue[delay_idx].tick_delay))
    {
      hal_power_disable_timer_it();       /*prevent the timer interrupt to mess up with the update of tick_delay and nb_delayed_msg*/
      delayed_msg_queue.msg_queue[delay_idx].tick_delay = DELAY_FREE_MSG;
      delayed_msg_queue.nb_delayed_msg--;
      delayed_msg_queue.msg_queue[delay_idx].delayed_evt_id = 0;
      hal_power_restore_timer_it();       /*end of timer interrupt disabling*/
    }
  }
}
