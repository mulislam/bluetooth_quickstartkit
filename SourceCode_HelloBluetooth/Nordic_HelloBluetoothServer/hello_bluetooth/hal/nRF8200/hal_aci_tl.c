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
@brief Implementation of the ACI transport layer module
*/

#include "hal_platform.h"
#include "hal_aci_tl.h"
#include "hal_io.h"
#include "system.h"
#include "assertions.h"
#include "lib_mem.h"

static hal_aci_data_t received_data;
static volatile hal_aci_data_t data_to_send;
static volatile bool spi_transmit_requested;
static uint8_t spi_readwrite(uint8_t aci_byte); 

void hal_aci_tl_io_config()
{
  HAL_IO_CONFIG(HAL_IO_RADIO_REQN, HAL_IO_OUTPUT, HAL_IO_OUTPUT_NORMAL_STRENGTH);
  HAL_IO_SET_STATE(HAL_IO_RADIO_REQN, 1);
  HAL_IO_CONFIG(HAL_IO_RADIO_SCK, HAL_IO_OUTPUT, HAL_IO_OUTPUT_NORMAL_STRENGTH);
  HAL_IO_SET_STATE(HAL_IO_RADIO_SCK, 0);
  HAL_IO_CONFIG(HAL_IO_RADIO_MOSI, HAL_IO_OUTPUT, HAL_IO_OUTPUT_NORMAL_STRENGTH);
  
  HAL_IO_CONFIG(HAL_IO_RADIO_ACTIVE, HAL_IO_INPUT, HAL_IO_INPUT_BUF_ON_NO_PULL);
  HAL_IO_CONFIG(HAL_IO_RADIO_RDY, HAL_IO_INPUT, HAL_IO_INPUT_BUF_ON_PULL_UP);
  HAL_IO_CONFIG(HAL_IO_RADIO_MISO, HAL_IO_INPUT, HAL_IO_INPUT_BUF_ON_PULL_UP);
  
}

void hal_aci_tl_init()
{
  received_data.buffer[0] = 0;

  hal_pltf_configure_spi_for_aci();
  hal_pltf_disable_spi();  
  hal_pltf_config_ready_line();
  HAL_IO_SET_STATE(HAL_IO_RADIO_REQN, 1);
  spi_transmit_requested = false;
  data_to_send.buffer[0] = 0;
}

bool hal_aci_tl_send(hal_aci_data_t *p_aci_cmd)
{
  uint8_t length = p_aci_cmd->buffer[0];
  if (!spi_transmit_requested)
  {
    ASSERT(ERROR_CODE_HAL_ACI_TL_OVERFLOW,(p_aci_cmd->buffer[0] <= HAL_ACI_MAX_LENGTH));
    if (length > HAL_ACI_MAX_LENGTH)
    {
      return(false);
    }
    {
      bool  is_interrupt_enabled_before_send = ARE_INTERRUPTS_ENABLED();
      DISABLE_INTERRUPTS();                           /*disable interrupts to protects the modification of the buffer pointer*/
      lib_mem_copy(data_to_send.buffer, p_aci_cmd->buffer, length+1);
      spi_transmit_requested = true; // Request transmission
      if (is_interrupt_enabled_before_send)
      {
        ENABLE_INTERRUPTS();                         /*eventually re-enable the interrupts if they were enabled*/
      }
    }
    HAL_IO_SET_STATE(HAL_IO_RADIO_REQN, 0);
    return(true);
  }
  else
  {
    return(false);
  }
}

void hal_aci_tl_poll_rdy_line(void)
{
  uint8_t byte_cnt;
  uint8_t byte_sent_cnt;
  uint8_t max_bytes;
  bool is_transmit_finished = false;
  
  if(1 ==HAL_IO_READ(HAL_IO_RADIO_RDY))
  {
    return;
  }
  
  hal_pltf_enable_spi();  

  HAL_IO_SET_STATE(HAL_IO_RADIO_REQN, 0);

  
  // Send length, receive header
  byte_sent_cnt = 0;
  received_data.status_byte = spi_readwrite(data_to_send.buffer[byte_sent_cnt++]);
  // Send first byte, receive length from slave
  received_data.buffer[0] = spi_readwrite(data_to_send.buffer[byte_sent_cnt++]);
  if (0 == data_to_send.buffer[0])
  {
    max_bytes = received_data.buffer[0];
  }
  else
  {
    // Set the maximum to the biggest size. One command byte is already sent
    max_bytes = (received_data.buffer[0] > (data_to_send.buffer[0] - 1)) 
      ? received_data.buffer[0] : (data_to_send.buffer[0] - 1);
  }

  if (max_bytes > HAL_ACI_MAX_LENGTH)
  {
    max_bytes = HAL_ACI_MAX_LENGTH;
  }

  // Transmit/receive the rest of the packet 
  for (byte_cnt = 0; byte_cnt < max_bytes; byte_cnt++)
  {
    received_data.buffer[byte_cnt+1] =  spi_readwrite(data_to_send.buffer[byte_sent_cnt++]);
  }
  /* Release REQN */
  HAL_IO_SET_STATE(HAL_IO_RADIO_REQN, 1);

  hal_pltf_disable_spi();  

  ASSERT(ERROR_CODE_HAL_ACI_TL_STATUS_BYTE,(0 != received_data.status_byte));
  if (spi_transmit_requested)
  {
    data_to_send.buffer[0] = 0;
    spi_transmit_requested = false;
  }
  
  /* valid Rx available or transmit finished*/
  hal_aci_tl_msg_rcv_hook(&received_data);
}

static uint8_t spi_readwrite(const uint8_t aci_byte)
{
  hal_pltf_set_spim_data(aci_byte);      
  while(!hal_pltf_is_spim_data_ready())
    ;
  return hal_pltf_get_spim_data();
}

