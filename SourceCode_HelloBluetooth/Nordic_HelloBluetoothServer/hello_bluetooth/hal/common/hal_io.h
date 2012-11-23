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

/** @file
*/

/** @defgroup hal_io hal_io
 * @{
 * @ingroup hal
 * @brief Macros input/output management
*/

#include "hal_platform.h"

#ifndef HAL_IO_H__
#define HAL_IO_H__

#define HAL_IO_P00 P0, 0
#define HAL_IO_P01 P0, 1
#define HAL_IO_P02 P0, 2
#define HAL_IO_P03 P0, 3
#define HAL_IO_P04 P0, 4
#define HAL_IO_P05 P0, 5
#define HAL_IO_P06 P0, 6
#define HAL_IO_P07 P0, 7

#define HAL_IO_P10 P1, 0
#define HAL_IO_P11 P1, 1
#define HAL_IO_P12 P1, 2
#define HAL_IO_P13 P1, 3
#define HAL_IO_P14 P1, 4
#define HAL_IO_P15 P1, 5
#define HAL_IO_P16 P1, 6
#define HAL_IO_P17 P1, 7

#define HAL_IO_INPUT_DIR |= 
#define HAL_IO_OUTPUT_DIR &= ~

#define HAL_IO_OUTPUT 0
#define HAL_IO_INPUT 1

#define HAL_IO_OUTPUT_NORMAL_STRENGTH 0x00
#define HAL_IO_OUTPUT_HIGH_STRENGTH 0x60
#define HAL_IO_INPUT_BUF_ON_NO_PULL 0x00
#define HAL_IO_INPUT_BUF_ON_PULL_DOWN 0x20
#define HAL_IO_INPUT_BUF_ON_PULL_UP 0x40
#define HAL_IO_INPUT_BUF_OFF 0x60

#define HAL_IO_CONFIG_INTERNAL(port_name, io_num, io_dir, is_input, io_mode)\
  do{\
    port_name##DIR io_dir(1<<io_num);\
    port_name##CON = (io_mode)|(is_input<<4)|(io_num);\
  }while(0)
  


#define HAL_IO_SET_STATE_INTERNAL(port_name, io_num, io_state)\
  do{\
    port_name##io_num = io_state;\
  }while(0)

#define HAL_IO_READ_INTERNAL(port_name, io_num) port_name##io_num
  


/**@brief Macro to configure an I/O.
 *  @details
 *  This macro configures a given I/O to input or output with pullup/buffer configuration.
 *  @param io_name I/O to configure.
 *  @param is_input Indicate if the I/O is to be set as an input.
 *  @param io_mode Pull resistor and buffer configuration (must be HAL_IO_OUTPUT_NORMAL_STRENGTH, HAL_IO_OUTPUT_HIGH_STRENGTH, HAL_IO_INPUT_BUF_ON_NO_PULL,
 *  HAL_IO_INPUT_BUF_ON_PULL_DOWN, HAL_IO_INPUT_BUF_ON_PULL_UP, or HAL_IO_INPUT_BUF_OFF).
*/
#define HAL_IO_CONFIG(io_name, is_input, io_mode) HAL_IO_CONFIG_INTERNAL(io_name, is_input##_DIR, is_input, io_mode)

/**@brief Macro to set an output.
 *  @details
 *  This macro sets the given output to the given level (1 -> high, 0 -> low).
 *  @param io_name Output to change.
 *  @param io_state Level to set.
*/
#define HAL_IO_SET_STATE(io_name, io_state)\
  HAL_IO_SET_STATE_INTERNAL(io_name, io_state) 

/**@brief Macro that reads the current state of an input.
 *  @details
 *  This macro reads the current state of a logical input.
 *  @param io_name Input to read.
 *  @return Input state level (1 if high, 0 if low).
*/
#define HAL_IO_READ(io_name) HAL_IO_READ_INTERNAL(io_name)


#endif //HAL_IO_H__

/** @} */
