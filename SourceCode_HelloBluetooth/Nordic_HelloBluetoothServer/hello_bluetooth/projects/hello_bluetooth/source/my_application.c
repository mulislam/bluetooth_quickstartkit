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

#include "hal_platform.h"
#include "system.h"
#include "my_application.h"
#include "services.h"
#include "assertions.h"
#include "lib_aci.h"
#include "lib_traces.h"
#include "hal_power.h"


my_project_app_state_t app_state;

static bool is_connected = false;
static my_project_app_state_t app_next_state;

static xdata char hello_bluetooth_name[20] = "John Doe";
static xdata uint8_t hello_bluetooth_name_length = 8;

#ifdef BONDING
HAL_PLTF_MEM_RETENTION( bool, is_bonded, 0x000B)
#endif

void on_process_app(void)
{
  switch(app_state)
  {
  case APP_SLEEP:
    app_next_state = APP_STANDBY;
    lib_aci_device_wakeup();
    LIB_TRACES_LOG_INFO(INFO_CODE_WAKEUP_RADIO);
    break;
  case APP_SETUP:
    LIB_TRACES_LOG_INFO(INFO_CODE_CONFIG_HOST);
    app_next_state = APP_SETUP;
#ifdef BONDING
    is_bonded = false;
#endif
    /* lib_aci_send_setup_msg() returns true when all messages are sent */
    if (lib_aci_send_setup_msg())
    {
      app_next_state = APP_SETUP_FINISHED;
    } 
    break;
  case APP_STANDBY:
    lib_aci_set_local_data(PIPE_HELLO_BLUETOOTH_HELLO_BLUETOOTH_NAME_SET, hello_bluetooth_name, hello_bluetooth_name_length);
    break;
  case APP_RUN:
    app_next_state = APP_RUN;
    break;
  case APP_GOTO_SLEEP:
    LIB_TRACES_LOG_INFO(INFO_CODE_DEEP_SLEEP);
    HAL_IO_SET_STATE(HAL_IO_LED0, 0);
    HAL_IO_SET_STATE(HAL_IO_LED1, 0);
    hal_power_long_sleep(APP_TIME_SLEEP);

    /* nRF8200 will wake on reset */
    app_state = APP_SLEEP;
    break;
  }
}

void lib_aci_transaction_finished_hook(void)
{
  app_state = app_next_state;
  if (APP_SETUP_FINISHED == app_state)
  {
    /* State transition occurs on DeviceStartedEvent */
    app_next_state = APP_SETUP_FINISHED;
    LIB_TRACES_LOG_INFO(INFO_CODE_WAIT_STD_BY);
    /* Check that the CommandResponseEvent is received with status transaction complete for last setup message */
    ASSERT(ERROR_CODE_SETUP_ERROR, (lib_aci_is_host_setup_complete()));
  }
  else
  {
    on_process_app();
  }
}


void lib_aci_started_test_hook(aci_evt_t *p_rcvd_evt_data)
{
  /* Start Place holder code - Add code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */

  LIB_TRACES_LOG_ERROR(ERROR_CODE_UNEXPECTED_STARTED_TEST_EVT);
}

void lib_aci_started_setup_hook(aci_evt_t *p_rcvd_evt_data)
{
#if DEBUG_ENABLE
  aci_evt_params_device_started_t *p_device_started_data;
  p_device_started_data = &(p_rcvd_evt_data->params.device_started);
  if (ACI_BOND_STATUS_SUCCESS != p_device_started_data->hw_error)
  {
    LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_STDBY_EVT_WITH_HW_ERROR, p_device_started_data->hw_error);
  }
  LIB_TRACES_LOG_INFO(INFO_CODE_SETUP);
#endif
  /* Start Place holder code - Add code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */
  app_state = APP_SETUP;
  on_process_app();
}

void lib_aci_started_stdby_hook(aci_evt_t *p_rcvd_evt_data)
{
#if DEBUG_ENABLE
  aci_evt_params_device_started_t *p_device_started_data;
  p_device_started_data = &(p_rcvd_evt_data->params.device_started);

  if (ACI_BOND_STATUS_SUCCESS != p_device_started_data->hw_error)
  {
    LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_STDBY_EVT_WITH_HW_ERROR, p_device_started_data->hw_error);
  }
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_STARTED_STDBY, app_state);
#endif

  /* Start Place holder code - Add code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */

  switch(app_state)
  {
  case APP_SLEEP:
    //do nothing, the command response event will trig the next state
    break;
  case APP_INIT:
    LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_STDBY_EVT_NOT_EXPECTED, app_state);
    break;
  case APP_SETUP_FINISHED:
    app_state = APP_STANDBY;
    on_process_app();
    break;
  default :
    LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_STDBY_EVT_NOT_EXPECTED, app_state);
    break;
  }
}

void lib_aci_hw_error_hook(aci_evt_t *p_rcvd_evt_data, uint8_t data_len)
{
#if DEBUG_ENABLE
  uint8_t byte_index;
  aci_evt_params_hw_error_t *p_hw_error_data;
  p_hw_error_data = &(p_rcvd_evt_data->params.hw_error);
  
  LIB_TRACES_LOG_ERROR(ERROR_CODE_ERR_HANDLING_RADIO_HW_ERROR);
  LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_ERR_HANDLING_RADIO_HW_ERROR_LINE_NUM_MSB, (uint8_t)(p_hw_error_data->line_num>>8));
  LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_ERR_HANDLING_RADIO_HW_ERROR_LINE_NUM_LSB, (uint8_t)(p_hw_error_data->line_num));
  for(byte_index = 0; byte_index < data_len-2; byte_index++)
  {
    LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_ERR_HANDLING_RADIO_HW_ERROR_CONTENT, p_hw_error_data->file_name[byte_index]);
  }
#endif
  /* Start Place holder code - Add code here to process this Event data */
  p_rcvd_evt_data = NULL;
  data_len = 0;
  /* End Place holder code */

  app_state = APP_SETUP;
  app_next_state = APP_SETUP;
  on_process_app();
}

void lib_aci_connected_hook(aci_evt_t *p_rcvd_evt_data)
{
  /* Start Place holder code - Add code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */

  LIB_TRACES_LOG_INFO(INFO_CODE_CONNECTED);
  if (!is_connected)
  {
    is_connected = true;
  }
  HAL_IO_SET_STATE(HAL_IO_LED1, 1);

  // Use the timing as set up in Studio
  lib_aci_change_timing_GAP_PPCP();
}

void lib_aci_pipe_status_hook(aci_evt_t *p_rcvd_evt_data)
{
  /* Start Place holder code - Add code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */

  LIB_TRACES_LOG_INFO(INFO_CODE_PIPE_STATUS_EVT);
}

void lib_aci_disconnect_hook(aci_evt_t *p_rcvd_evt_data)
{
  aci_evt_params_disconnected_t *p_disconnect_data;
  p_disconnect_data = &(p_rcvd_evt_data->params.disconnected);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DISCONNECTED, p_disconnect_data->aci_status);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DISCONNECTED, p_disconnect_data->btle_status);

  is_connected = false;
  app_state = APP_STANDBY;
  app_next_state = APP_STANDBY;
  HAL_IO_SET_STATE(HAL_IO_LED1, 0);
  on_process_app();
}

void lib_aci_adv_timeout_hook(aci_evt_t *p_rcvd_evt_data)
{
  LIB_TRACES_LOG_INFO(INFO_CODE_ADV_TIMEOUT); 
  app_state = APP_GOTO_SLEEP;
  app_next_state = APP_GOTO_SLEEP;
  lib_aci_sleep();

  /* Start Place holder code - Edit code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */

}

void lib_aci_data_rcvd_hook(aci_evt_t *p_rcvd_evt_data, uint8_t data_len)
{
  aci_evt_params_data_received_t *p_data_rcvd_data;
  p_data_rcvd_data = &(p_rcvd_evt_data->params.data_received);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_PIPE_UPDATED_PIPE_NUM, p_data_rcvd_data->rx_data.pipe_number);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_PIPE_UPDATED_DATA0, p_data_rcvd_data->rx_data.aci_data[0]);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DATA_RECEIVED_LEN, data_len);

  /* Start Place holder code - Edit code here to process this Event data */
  data_len = 0;
  /* End Place holder code */
  

}

void lib_aci_pipe_error_hook(aci_evt_t *p_rcvd_evt_data)
{
  aci_evt_params_pipe_error_t *p_pipe_error_data;
  p_pipe_error_data = &(p_rcvd_evt_data->params.pipe_error);
  LIB_TRACES_LOG_ERROR_1VAR(ERROR_CODE_PIPE_ERROR, p_pipe_error_data->pipe_number);
}

void lib_aci_timing_hook(aci_evt_t *p_rcvd_evt_data)
{
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_TIMING_EVT, (uint8_t)(p_rcvd_evt_data->params.timing.conn_rf_interval));
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_TIMING_EVT, (uint8_t)(p_rcvd_evt_data->params.timing.conn_rf_interval>>8));
  
  /* Start Place holder code - Edit code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */

  // Having changed the timing, we can go on to the RUN state
  app_state = APP_RUN;
  on_process_app();

}

void lib_aci_data_ack_hook(aci_evt_t *p_rcvd_evt_data)
{
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DATA_ACK, p_rcvd_evt_data->params.data_ack.pipe_number);

  /* Start Place holder code - Edit code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */    
}

void lib_aci_bond_status_hook(aci_evt_t *p_rcvd_evt_data)
{
  aci_evt_params_bond_status_t *p_bond_status_data;
  p_bond_status_data = &(p_rcvd_evt_data->params.bond_status);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_BOND, p_bond_status_data->status_code);
  if (ACI_STATUS_SUCCESS == p_bond_status_data->status_code)
  {
#ifdef BONDING
    is_bonded = true;
#endif
    app_state = APP_RUN;
    app_next_state = APP_RUN;
    on_process_app();
  }
}

void lib_aci_key_request_hook(aci_evt_t *p_rcvd_evt_data)
{
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_KEY_REQUEST, p_rcvd_evt_data->params.key_request.key_type);

  /* Start Place holder code - Edit code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */      
}

void lib_aci_display_passkey_hook(aci_evt_t *p_rcvd_evt_data)
{
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DISPLAY_PASSKEY, p_rcvd_evt_data->params.display_passkey.passkey[0]-ASCII_VALUE_FOR_ZERO);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DISPLAY_PASSKEY, p_rcvd_evt_data->params.display_passkey.passkey[1]-ASCII_VALUE_FOR_ZERO);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DISPLAY_PASSKEY, p_rcvd_evt_data->params.display_passkey.passkey[2]-ASCII_VALUE_FOR_ZERO);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DISPLAY_PASSKEY, p_rcvd_evt_data->params.display_passkey.passkey[3]-ASCII_VALUE_FOR_ZERO);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DISPLAY_PASSKEY, p_rcvd_evt_data->params.display_passkey.passkey[4]-ASCII_VALUE_FOR_ZERO);
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DISPLAY_PASSKEY, p_rcvd_evt_data->params.display_passkey.passkey[5]-ASCII_VALUE_FOR_ZERO);

  /* Start Place holder code - Edit code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */
}

void lib_aci_credit_hook(aci_evt_t *p_rcvd_evt_data)
{
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_CREDIT, p_rcvd_evt_data->params.data_credit.credit);

  /* Start Place holder code - Edit code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */
  
}

void lib_aci_device_version_hook(aci_evt_t *p_rcvd_evt_data)
{
  LIB_TRACES_LOG_INFO_1VAR(INFO_CODE_DEVICE_VERSION_CONFIG_ID, p_rcvd_evt_data->params.cmd_rsp.params.get_device_version.configuration_id);

  /* Start Place holder code - Edit code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */
}

void lib_aci_cmd_response_hook(aci_evt_t *p_rcvd_evt_data)
{
  // if we have set the local data, we can go on and connect
  if(p_rcvd_evt_data->params.cmd_rsp.cmd_opcode == ACI_CMD_SET_LOCAL_DATA) 
  {
    LIB_TRACES_LOG_INFO(INFO_CODE_CONNECT);
    lib_aci_connect(APP_ADVERTISING_TIME, APP_RUN_ADV_PERIOD); 
    HAL_IO_SET_STATE(HAL_IO_LED0, 1);
    app_next_state = APP_RUN;
  }

}

void lib_aci_evt_echo_hook(aci_evt_t *p_rcvd_evt_data)
{
  /* Start Place holder code - Edit code here to process this Event data */
  p_rcvd_evt_data = NULL;
  /* End Place holder code */
}