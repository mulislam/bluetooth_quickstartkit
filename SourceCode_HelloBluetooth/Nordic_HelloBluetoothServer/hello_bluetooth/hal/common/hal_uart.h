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
 * @brief interface for hal_uart
 */
 
/** @defgroup hal_uart hal_uart
@{
@ingroup hal
 
@brief Functions for sending data over the UART

*/
#ifndef HAL_UART_H
#define HAL_UART_H
#include "hal_platform.h"

#define HAL_UART_PACKET_SIZE 3


/**@brief UART initialization function
 *  @details
 *  This initializes the UART
*/
void hal_uart_init();

/**@brief Sends a message over the UART
 *  @details
 *  This function sends a given string over the UART. 
 *  @param p_msg NULL terminated string.
*/
void hal_uart_send_msg(char *p_msg);

/**@brief Initialize UART and send a message over the UART
 *  @details
 *  This function first initializes the UART and then 
 *  sends a given string over the UART. 
 *  @param p_msg NULL terminated string.
*/
void hal_uart_init_send(char *p_msg);

/**@brief Sends the given packet over UART 
 *  @details
 *  This function sends the given packet over UART, byte 0 first.
 *  @param p_msg_to_send Packet to send.
 *  @param size Number of bytes to send.
*/
void hal_uart_send_data(uint8_t *p_msg_to_send, uint8_t size);

/**@brief Initializes UART and sends the given packet over UART 
 *  @details
 *  This function first initializes the UART and then 
 *  sends the given packet over the UART. 
 *  @param p_msg_to_send Packet to send.
 *  @param size Number of bytes to send.
*/
void hal_uart_init_send_data(uint8_t *p_msg_to_send, uint8_t size)reentrant;

/**@brief Wait to receive one byte on the UART.
 *  @details
 *  This function waits until one byte is received from the UART
 *  and returns the received byte.
 *  @return Received byte.
*/
uint8_t hal_uart_read_byte();

#ifdef ENABLE_UART_INTERUPT_RCV
#define HAL_UART_MAX_LEN 5

/** Data type for received UART messages */
typedef struct hal_uart_data_t{
  uint8_t buffer_id;
  uint8_t msg_length;
  uint8_t buffer[HAL_UART_MAX_LEN];
} hal_uart_data_t;

/**@brief Enables reception by interrupt.
 * After calling this function any data received on the UART will trigger an interrupt which will be handled by @ref hal_uart_on_uart_isr
 */
 void hal_uart_enable_rcv_interrupt();

/**@brief set the UART receive buffer.
 * Use this function to set the buffer into which data received on the UART will be written. This function also enable data reception on the UART.
 * @param p_rcvd_msg pointer to a buffer to write received data to.
 */
void hal_uart_set_buffer(hal_uart_data_t *p_in_buffer);

/**@brief UART ISR function.
 * @details This function handles the UART interrupts. It reads a first byte containing the length to receive and
 * then, if this length is valid, the following interrupts will read the byte into the receive buffer.
 * When all bytes are received, call hal_uart_reveive_hook()
 */
 void hal_uart_on_uart_isr();

/** @brief UART Message received hook function
 *  @details A hook function that must be implemented by the client of this module. 
 * The function will be called by hal_uart when a new message has been received on the UART.
 *  @param p_rcvd_msg pointer to a structure containing the number of received bytes and a pointer to the received data
 */
extern void hal_uart_msg_rcv_hook(hal_uart_data_t *p_rcvd_msg);
#endif  //ENABLE_UART_INTERUPT_RCV
 
#endif //HAL_UART_H
/** @} */
