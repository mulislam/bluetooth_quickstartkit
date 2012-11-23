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
@brief Implementation of the UART module
*/
#include "hal_platform.h"


#include "hal_uart.h"
#include "system.h"

#define BAUD_57K6   1015  // = Round(1024 - (2*16e6)/(64*57600))
#define BAUD_38K4   1011  // = Round(1024 - (2*16e6)/(64*38400))
#define BAUD_19K2    998  // = Round(1024 - (2*16e6)/(64*19200))
#define BAUD_9K6     972  // = Round(1024 - (2*16e6)/(64*9600))

static uint8_t nb_rcvd_bytes;

void hal_uart_init() 
{
  uint16_t temp;
  bool is_it_enabled;

  P0DIR &= ~0x08;                                     //P0.3 as output
  is_it_enabled = ES0;
  ES0 = 0;                      // Disable UART0 interrupt while initializing
  REN0 = 1;                     // Enable receiver
  SM0 = 0;                      // Mode 1..
  SM1 = 1;                      // ..8 bit variable baud rate
  PCON |= 0x80;                 // SMOD = 1
  ADCON |= 0x80;                // Select internal baud rate generator
  temp = BAUD_57K6;
  S0RELL = (uint8_t)temp;
  S0RELH = (uint8_t)(temp >> 8);

  TI0 = 0;
  RI0 = 0;
  ES0 = is_it_enabled;
}


void hal_uart_send_msg(char *p_msg)
{
  uint8_t byte_idx = 0;
  
  do{
    TI0 = 0;
    S0BUF = p_msg[byte_idx];
    while (TI0 == 0);
  }while(p_msg[++byte_idx] != '\0');
  TI0 = 0;
  RI0 = 0;
}

void hal_uart_init_send(char *p_msg)
{
  bool previous_ea;
  previous_ea = ARE_INTERRUPTS_ENABLED();
  DISABLE_INTERRUPTS();
  hal_uart_init();
  hal_uart_send_msg(p_msg);
  if (previous_ea)
  {
    ENABLE_INTERRUPTS();
  }
}


void hal_uart_init_send_packet(uint8_t *p_msg_to_send)
{
  bool previous_ea;
  uint8_t byte_idx;
  previous_ea = ARE_INTERRUPTS_ENABLED();
  DISABLE_INTERRUPTS();
  hal_uart_init();
  
  for (byte_idx = 0; byte_idx < HAL_UART_PACKET_SIZE; byte_idx++)
  {
    TI0 = 0;
    S0BUF = (uint8_t)(p_msg_to_send[byte_idx]);
    while (TI0 == 0);
  }
  TI0 = 0;
  RI0 = 0;
  if (previous_ea)
  {
    ENABLE_INTERRUPTS();
  }
}

void hal_uart_send_data(uint8_t *p_msg_to_send, uint8_t size)
{
  uint8_t byte_idx;
  for (byte_idx = 0; byte_idx < size; byte_idx++)
  {
    TI0 = 0;
    S0BUF = (uint8_t)(p_msg_to_send[byte_idx]);
    while (TI0 == 0);
  }
  TI0 = 0;
  RI0 = 0;
}

void hal_uart_init_send_data(uint8_t *p_msg_to_send, uint8_t size) KEYWORD_REENTRANT
{
  bool previous_ea;
  uint8_t byte_idx;
  previous_ea = ARE_INTERRUPTS_ENABLED();
  DISABLE_INTERRUPTS();
  hal_uart_init();

  for (byte_idx = 0; byte_idx < size; byte_idx++)
  {
    TI0 = 0;
    S0BUF = (uint8_t)(p_msg_to_send[byte_idx]);
    while (TI0 == 0);
  }
  TI0 = 0;
  RI0 = 0;
  if (previous_ea)
  {
    ENABLE_INTERRUPTS();
  }
}

uint8_t hal_uart_read_byte()
{
  uint8_t read_byte;
  
  while(0 == RI0)
    ;
  RI0 = 0;
  read_byte = S0BUF;
  return(read_byte);
}

#ifdef ENABLE_UART_INTERUPT_RCV
static hal_uart_data_t *p_received_msg = NULL;
void hal_uart_enable_rcv_interrupt()
{
  p_received_msg->msg_length = 0;
  nb_rcvd_bytes = HAL_UART_MAX_LEN;
  RI0 = 0;
  ES0 = 1;
}

void hal_uart_disable_rcv_interrupt()
{
  ES0 = 0;
}

void hal_uart_set_buffer(hal_uart_data_t *p_in_buffer)
{
  p_received_msg = p_in_buffer;
  hal_uart_enable_rcv_interrupt();
}

void read_msg_length(uint8_t rcv_byte)
{
  p_received_msg->msg_length = rcv_byte;
  if (p_received_msg->msg_length > HAL_UART_MAX_LEN )
  {
    p_received_msg->msg_length = 0;
    nb_rcvd_bytes = HAL_UART_MAX_LEN;
  }
  else
  {
    nb_rcvd_bytes = 0;
  }
}

void hal_uart_on_uart_isr()
{
  uint8_t received_byte;
  TI0 = 0;
  if (0 == RI0)
  {
    return;
  }
  if (NULL == p_received_msg)
  {
    hal_uart_disable_rcv_interrupt();
    RI0 = 0;
    return;
  }
  received_byte = S0BUF;
  RI0 = 0;
  //received_byte = hal_uart_read_byte();  
  if (HAL_UART_MAX_LEN == nb_rcvd_bytes)
  {
    read_msg_length(received_byte);
  }
  else
  {
    p_received_msg->buffer[nb_rcvd_bytes++] = received_byte;
    if (nb_rcvd_bytes == p_received_msg->msg_length)
    {
      nb_rcvd_bytes = HAL_UART_MAX_LEN;
      hal_uart_disable_rcv_interrupt();
      hal_uart_msg_rcv_hook(p_received_msg);
    }
  }
}
#endif
