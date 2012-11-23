/* Copyright (c) 2010 Nordic Semiconductor. All Rights Reserved.
*
* The information contained herein is property of Nordic Semiconductor ASA.
* Terms and conditions of usage are described in detail in NORDIC
* SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
*
* Licensees are granted free, non-transferable use of the information. NO
* WARRANTY of ANY KIND is provided. This heading must NOT be removed from
* the file.
*/

/**
* This file is autogenerated by 1.12.3.2177 
*/

#include "services.h"
#include "lib_aci.h"

static bool is_hello_bluetooth_hello_bluetooth_name_set_requested = false;

static void *hello_bluetooth_hello_bluetooth_name_set;
static uint8_t hello_bluetooth_hello_bluetooth_name_set_size;

static bool hello_bluetooth_hello_bluetooth_name_update_set(void)
{
  return(lib_aci_set_local_data(PIPE_HELLO_BLUETOOTH_HELLO_BLUETOOTH_NAME_SET, (void*)hello_bluetooth_hello_bluetooth_name_set, hello_bluetooth_hello_bluetooth_name_set_size));
}

void services_set_hello_bluetooth_hello_bluetooth_name(void *src, int size)
{
  hello_bluetooth_hello_bluetooth_name_set_size = size;
  hello_bluetooth_hello_bluetooth_name_set = src;
  is_hello_bluetooth_hello_bluetooth_name_set_requested = true;
  services_update_pipes();
}

void services_update_pipes(void)
{
  if(is_hello_bluetooth_hello_bluetooth_name_set_requested)
  {
    is_hello_bluetooth_hello_bluetooth_name_set_requested = !(hello_bluetooth_hello_bluetooth_name_update_set());
  }
}