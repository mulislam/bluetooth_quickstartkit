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

#ifndef SETUP_MESSAGES_H__
#define SETUP_MESSAGES_H__

#include "hal_platform.h" 
#include "aci.h"
#define PIPE_HELLO_BLUETOOTH_HELLO_BLUETOOTH_NAME_SET 1

#define NUMBER_OF_PIPES 1
typedef struct
{
  aci_pipe_store_t  location;
  aci_pipe_type_t   pipe_type;
} services_pipe_type_mapping_t;

static services_pipe_type_mapping_t services_pipe_type_mapping[NUMBER_OF_PIPES] =
{
  {ACI_STORE_LOCAL, ACI_SET},
};

#define GAP_PPCP_MAX_CONN_INT 0x50 /**< Maximum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_MIN_CONN_INT  0x30 /**< Minimum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific maximum*/
#define GAP_PPCP_SLAVE_LATENCY 0
#define GAP_PPCP_CONN_TIMEOUT 0xc8 /** Connection Supervision timeout multiplier as a multiple of 10msec, 0xFFFF means no specific value requested */

/** @brief do a set_local_data a new value for PIPE_HELLO_BLUETOOTH_HELLO_BLUETOOTH_NAME_SET
 *  @param src source buffer to send data from
 *  @param size the number of bytes to send. Maximum size is 20
 *  @details use this function to do a set_local_data for PIPE_HELLO_BLUETOOTH_HELLO_BLUETOOTH_NAME_SET. If no transaction are currently
 *  running, the set will be immediate, otherwise, it will be done at the end of the current transaction
 *  when services_update_pipes will be called.
 */
void services_set_hello_bluetooth_hello_bluetooth_name(void *src, int size);

/** @brief function to trig pending transaction on pipes
 *  @details This function check for each pipe if it has a pending transaction (send/rx_request/ack)
 *   and if so executes this transaction.
 *   This function should be called in the APP_RUN state of the process function of the application.
 */
void services_update_pipes(void);

#define NB_SETUP_MESSAGES 18
#define SETUP_MESSAGES_CONTENT {\
    {0x00,\
        {\
            0x07,0x06,0x00,0x00,0x03,0x02,0x40,0x6c,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x06,0x00,0x00,0x06,0x00,0x01,\
            0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x1c,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x40,0x20,0x00,0x00,0x00,0x10,0x03,0x90,0x01,0x14,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x38,0x02,0xff,0x02,0x58,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x05,0x06,0x10,0x54,0x00,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x00,0x04,0x04,0x02,0x02,0x00,0x01,0x28,0x00,0x01,0x00,0x18,0x04,0x04,0x05,0x05,0x00,\
            0x02,0x28,0x03,0x01,0x02,0x03,0x00,0x00,0x2a,0x04,0x04,0x14,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x1c,0x0b,0x00,0x03,0x2a,0x00,0x01,0x4b,0x65,0x79,0x66,0x6f,0x62,0x64,0x65,0x6d,0x6f,\
            0x76,0x63,0x6f,0x6d,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x38,0x05,0x05,0x00,0x04,0x28,0x03,0x01,0x02,0x05,0x00,0x01,0x2a,0x06,0x04,0x03,0x02,\
            0x00,0x05,0x2a,0x01,0x01,0x00,0x00,0x04,0x04,0x05,0x05,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x54,0x06,0x28,0x03,0x01,0x02,0x07,0x00,0x04,0x2a,0x06,0x04,0x09,0x08,0x00,0x07,0x2a,\
            0x04,0x01,0x30,0x00,0x50,0x00,0x00,0x00,0xc8,0x00,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x70,0x02,0x02,0x00,0x08,0x28,0x00,0x01,0x01,0x18,0x04,0x04,0x10,0x10,0x00,0x09,0x28,\
            0x00,0x01,0xb8,0xd0,0x2d,0x81,0x63,0x29,0xef,0x96,0x8a,0x4d,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x8c,0x55,0xb3,0x01,0x00,0xb2,0x5a,0x04,0x04,0x13,0x13,0x00,0x0a,0x28,0x03,0x01,0x02,\
            0x0b,0x00,0xb8,0xd0,0x2d,0x81,0x63,0x29,0xef,0x96,0x8a,0x4d,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xa8,0x55,0xb3,0x02,0x00,0xb2,0x5a,0x04,0x04,0x14,0x00,0x00,0x0b,0x00,0x02,0x02,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x0b,0x06,0x20,0xc4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x0d,0x06,0x40,0x00,0x00,0x02,0x02,0x00,0x80,0x04,0x00,0x0b,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x13,0x06,0x50,0x00,0xb8,0xd0,0x2d,0x81,0x63,0x29,0xef,0x96,0x8a,0x4d,0x55,0xb3,0x00,0x00,0xb2,0x5a,\
        },\
    },\
    {0x00,\
        {\
            0x06,0x06,0x60,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x19,0x06,0x70,0x00,0x19,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x06,0x06,0xf0,0x00,0x83,0x1f,0x74,\
        },\
    },\
}

#endif
