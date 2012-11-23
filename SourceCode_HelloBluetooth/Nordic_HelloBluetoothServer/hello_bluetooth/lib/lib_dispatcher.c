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
#include "lib_dispatcher.h"
#include "assertions.h"
#include "system.h"

#define DELAY_FREE_MSG 0xFF

typedef struct lib_dispatcher_queue_t{
  uint8_t msg_handles[NB_MAX_EVT_PER_PRIORITY];
  uint8_t nb_msg_in_queue;
  uint8_t index_next_msg;
  uint8_t index_free_msg;
}lib_dispatcher_queue_t;

static volatile bool has_new_msg;


static lib_dispatcher_queue_t msg_queues[NB_PRIORITIES];

void lib_dispatcher_post_msg(uint8_t msg_handle, lib_dispatcher_priority_t msg_priority) KEYWORD_REENTRANT
{
  lib_dispatcher_queue_t *p_queue;
  bool previous_ea;

#if (DEBUG_ENABLE!=NO_TRACES)
  if (msg_handle >= NB_MAX_MSG)
  {
    LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_LIB_DISPATCHER_INVALID_HANLER, msg_handle);
  }
#endif
  ASSERT(ERROR_CODE_LIB_DISPATCHER_INVALID_HANDLE, (msg_handle < NB_MAX_MSG));

  if (msg_handle < NB_MAX_MSG)
  {
    has_new_msg = true;
    p_queue = &(msg_queues[msg_priority]);
    ASSERT(ERROR_CODE_LIB_DISPATCHER_MSG_OVERFLOW, (p_queue->nb_msg_in_queue < NB_MAX_EVT_PER_PRIORITY));

    previous_ea = ARE_INTERRUPTS_ENABLED();
    DISABLE_INTERRUPTS();                           /*disable interrupts to protects the modification of index_free_msg*/
    p_queue->msg_handles[p_queue->index_free_msg] = msg_handle;
    p_queue->index_free_msg ++;
    if (previous_ea)
    {
      ENABLE_INTERRUPTS();                         /*eventually re-enable the interrupts if they were enabled*/
    }

    if (p_queue->index_free_msg == NB_MAX_EVT_PER_PRIORITY)
    {
      p_queue->index_free_msg = 0;
    }
    p_queue->nb_msg_in_queue ++;
  }
}



static void dispatch_msg(lib_dispatcher_queue_t *p_queue_to_dispatch) reentrant
{
  uint8_t cur_msg_handle;
  cur_msg_handle = p_queue_to_dispatch->msg_handles[p_queue_to_dispatch->index_next_msg];
  p_queue_to_dispatch->index_next_msg ++;
  if (p_queue_to_dispatch->index_next_msg == NB_MAX_EVT_PER_PRIORITY)
  {
    p_queue_to_dispatch->index_next_msg = 0;
  }
  p_queue_to_dispatch->nb_msg_in_queue --;

  ASSERT(ERROR_CODE_LIB_DISPATCHER_INVALID_HANDLE, (cur_msg_handle < NB_MAX_MSG));

  if (cur_msg_handle < NB_MAX_MSG)
  {
    dispatch_msg_macro();
  }
}

void lib_dispatcher_init()
{
  uint8_t queue_idx, handle_idx;
  
  for(queue_idx = 0; queue_idx < NB_PRIORITIES; queue_idx++)
  {
    for (handle_idx = NB_MAX_EVT_PER_PRIORITY-1; handle_idx > 0; handle_idx--)
    {
      msg_queues[queue_idx].msg_handles[handle_idx] = 0;
    }
    msg_queues[queue_idx].index_next_msg = 0;
    msg_queues[queue_idx].nb_msg_in_queue = 0;
    msg_queues[queue_idx].index_free_msg = 0;
  }
  has_new_msg = false;
}

void lib_dispatcher_dispatch(void)
{
  uint8_t queue_idx;
  for(queue_idx = 0; queue_idx < NB_PRIORITIES; queue_idx++)
  {
    if (0 != msg_queues[queue_idx].nb_msg_in_queue)
    {
      has_new_msg = false;
      dispatch_msg(&(msg_queues[queue_idx]));
      return;
    }
  }
  DISABLE_INTERRUPTS();
  if (!has_new_msg)
  {
    hal_power_sleep();
  }
  else
  {
    ENABLE_INTERRUPTS();
  }
  /* After the sleep, the device will wake-up here */
}

