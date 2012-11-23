/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
*
* The information contained herein is property of Nordic Semiconductor ASA.
* Terms and conditions of usage are described in detail in NORDIC
* SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
*
* Licensees are granted free, non-transferable use of the information. NO
* WARRENTY of ANY KIND is provided. This heading must NOT be removed from
* the file.
*
* $LastChangedRevision: 4808 $
*/ 
/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/
/** @file
@brief Implementation of the UART library.
*/

#include "hal_platform.h"
#include "hal_uart.h"
#include "lib_uart.h"
#include "assertions.h"


static hal_uart_data_t uart_rcv_buffer[LIB_UART_NB_BUFFERS];
static uint8_t buf_write_idx;
static uint8_t buf_read_idx;

void lib_uart_init()
{
  for (buf_write_idx = LIB_UART_NB_BUFFERS; buf_write_idx != 0; buf_write_idx--)
  {
    uart_rcv_buffer[buf_write_idx-1].buffer_id = 0xFF;
  }
  buf_write_idx = 0;
  buf_read_idx = 0;
  uart_rcv_buffer[buf_write_idx].buffer_id = buf_write_idx;
  hal_uart_set_buffer(&(uart_rcv_buffer[buf_write_idx]));
}

uint8_t lib_uart_get_msg(lib_uart_rcv_msg_t *p_out_data)
{
  uint8_t idx;
  uint8_t cmde;
  hal_uart_data_t *p_read_buffer = &uart_rcv_buffer[buf_read_idx];
  if( 0xFF == p_read_buffer->buffer_id)
  {
    return(0xFF);
  }
  buf_read_idx ++;
  if (buf_read_idx >= LIB_UART_NB_BUFFERS)
  {
    buf_read_idx = 0;
  }
  cmde = p_read_buffer->buffer[0];
  if (NULL != p_out_data)
  {
    p_out_data->length = p_read_buffer->msg_length-1;
    p_out_data->cmd    = cmde;
    for (idx = 0; idx < p_out_data->length; idx ++)
    {
      p_out_data->data_buffer[idx] = p_read_buffer->buffer[idx+1];
    }
  }
  p_read_buffer->buffer_id = 0xFF;
  if (0xFF == uart_rcv_buffer[buf_write_idx].buffer_id)
  {
    uart_rcv_buffer[buf_write_idx].buffer_id = buf_write_idx;
    hal_uart_set_buffer(&(uart_rcv_buffer[buf_write_idx]));
  }  
  return (cmde);
}

void hal_uart_msg_rcv_hook(hal_uart_data_t *p_rcvd_msg)
{
  p_rcvd_msg = NULL;
  buf_write_idx++;
  if (buf_write_idx >= LIB_UART_NB_BUFFERS)
  {
    buf_write_idx = 0;
  }
  if (0xFF == uart_rcv_buffer[buf_write_idx].buffer_id)
  {
    uart_rcv_buffer[buf_write_idx].buffer_id = buf_write_idx;
    hal_uart_set_buffer(&(uart_rcv_buffer[buf_write_idx]));  
  }
  lib_uart_post_msg_hook();
}

/** @} */
