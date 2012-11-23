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
 * @brief Interface for hal_vdd
 */
 
/** @defgroup hal_vdd hal_vdd
@{
@ingroup hal
 
@brief Uses the ADC and the 1.2 V internal reference to calculate VDD
 
*/

#ifndef HAL_VDD_H__
#define HAL_VDD_H__

#include "hal_platform.h"

/** @brief Initialises the VDD library

The function sets up the ADC to read the VDD level and compares it with the
internal 1.2V reference. The function must be called before using 
hal_vdd_get_level.
 */
void hal_vdd_init(void);

/** @brief Gets the the VDD level in mV

The function performs 4 ADC measurements of VDD and returns the average. The
function converts the result to mV.

@return VDD in mV
 */
uint16_t hal_vdd_get_level(void);

#endif // HAL_VDD_H__
/** @} */
