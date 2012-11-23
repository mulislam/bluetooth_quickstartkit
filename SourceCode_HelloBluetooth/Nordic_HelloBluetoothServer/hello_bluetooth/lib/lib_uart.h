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
 * $LastChangedRevision$
 */
/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/
#ifndef LIB_UART_H__
#define LIB_UART_H__

/** @file
* @brief UART library
*/

/** @defgroup lib_uart lib_uart
@{
@ingroup lib

@brief Library for the 

@details This library implements functions 

*/


#include "hal_platform.h"
#include "hal_uart.h"

#define LIB_UART_NB_BUFFERS 3
#define LIB_UART_BUF_LEN (HAL_UART_MAX_LEN-1)
typedef struct lib_uart_rcv_msg_t{
    uint8_t length;
    uint8_t cmd;
    uint8_t data_buffer[LIB_UART_BUF_LEN];
}lib_uart_rcv_msg_t;

/** @brief Hook function for receiving messages from the UART library.
 *  @details A hook function that must be implemented by the client of this module.
 *  The function will be called each time a message is received on the UART.
 */
extern void lib_uart_post_msg_hook();

/** @name Functions for library management */
//@{

/** @brief Initialization function.
 */
void lib_uart_init();

/** @brief Get the data from the last received message
 *  @details This function returns a command byte and copies the associated data into the given structure.
 *  @param[out] p_out_data pointer to an lib_uart_rcv_msg_t structure.
 *  @return command byte
 */
uint8_t lib_uart_get_msg(lib_uart_rcv_msg_t *p_out_data);

//@}

/** @} */

#endif /* LIB_UART_H__ */
