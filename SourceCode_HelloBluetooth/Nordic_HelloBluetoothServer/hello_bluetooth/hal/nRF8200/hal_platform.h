/* Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
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

#ifndef PLATFORM_H__
#define PLATFORM_H__

/** @file
* @brief 
*/
#include <reg24le1.h>
#include <stdint.h>
#include <stdbool.h>
#include <intrins.h>

//Reentrant is Keil C51 compiler specific
#define KEYWORD_REENTRANT reentrant

#define KEYWORD_CODE code

// SPIMCON1
#define SPI_RXFIFOFULL    0x08
#define SPI_RXDATAREADY   0x04
#define SPI_TXFIFOEMPTY   0x02
#define SPI_TXFIFOREADY   0x01


#define HAL_PLTF_MEM_RETENTION(variable_type, variable_name, address) xdata variable_type variable_name _at_ address;

#define HAL_PLTF_INTERRUPT_HANDLER(function_name, interrupt_vector) void function_name(void) interrupt interrupt_vector

//register access macros for ACI
#define hal_pltf_disable_ready_line_it() do { EX0 = 0; } while(0) // Disable GPINT1 (P0.6)
#define hal_pltf_enable_ready_line_it()  do { EX0 = 1; } while(0) // Disable GPINT1 (P0.6)

#define HAL_PLTF_SPI_FREQ_4MHZ                                      (0x00<<4)        /*1/2 of MCU clk, with 8MHz clk is 4MHz*/
#define HAL_PLTF_SPI_FREQ_2MHZ                                      (0x01<<4)        /*1/4 of MCU clk, with 8MHz clk is 2MHz*/
#define HAL_PLTF_SPI_FREQ_1MHZ                                      (0x02<<4)        /*1/8 of MCU clk, with 8MHz clk is 1MHz*/
#define HAL_PLTF_SPI_FREQ_500KHZ                                    (0x03<<4)        /*1/16 of MCU clk, with 8MHz clk is 500KHz*/
#define HAL_PLTF_SPI_FREQ_250KHZ                                    (0x04<<4)        /*1/32 of MCU clk, with 8MHz clk is 250KHz*/
#define HAL_PLTF_SPI_FREQ_125KHZ                                    (0x05<<4)        /*1/64 of MCU clk, with 8MHz clk is 125KHz*/
                                                                    
#define HAL_PLTF_SPI_ORDER_LSBIT_FIRST                              (0x01<<3)
#define HAL_PLTF_SPI_ORDER_MSBIT_FIRST                              (0x00<<3)
#define HAL_PLTF_SPI_CLK_POLARITY_LOW                               (0x01<<2)
#define HAL_PLTF_SPI_CLK_POLARITY_HIGH                              (0x00<<2)
#define HAL_PLTF_SPI_CLK_PH_SAMPLE_TRAILING_EDGE_SHIFT_LEADING_EDGE (0x01<<1)
#define HAL_PLTF_SPI_CLK_PH_SAMPLE_LEADING_EDGE_SHIFT_TRAILING_EDGE (0x00<<1)
#define HAL_PLTF_SPI_ENABLE                                         (0x01)

#define hal_pltf_clear_spi_master_config() do { SPIMCON0 = 0; } while(0)
#define hal_pltf_spi_master_config(freq, order, clk_polarity, clk_phase) do { SPIMCON0 = freq|order|clk_polarity|clk_phase; } while(0)

#define hal_pltf_enable_spi()  do { SPIMCON0 |= HAL_PLTF_SPI_ENABLE; } while(0)
#define hal_pltf_disable_spi() do { SPIMCON0 &= ~HAL_PLTF_SPI_ENABLE; } while(0)
  
#define hal_pltf_set_spim_data(aci_byte) do { SPIMDAT = aci_byte; } while(0)
#define hal_pltf_is_spim_data_ready() (SPIMSTAT & 0x04)
#define hal_pltf_get_spim_data() SPIMDAT
  
  
#define hal_pltf_enable_wakeup_from_pin()  do { WUPIN = 1; } while(0)
#define hal_pltf_disable_wakeup_from_pin() do { WUPIN = 0; } while(0)

#define hal_pltf_enable_wakeup_it()                do { WUIRQ = 1; } while(0)
#define hal_pltf_disable_wakeup_it()               do { WUIRQ = 0; } while(0)
#define hal_pltf_set_it_from_pin_on_falling_edge() do { IT0 = 1; } while(0)

#define HAL_PLTF_GPINT2 0x20
#define HAL_PLTF_GPINT1 0x10
#define HAL_PLTF_GPINT0 0x08
#define hal_pltf_select_gpint(gpint) do { INTEXP |= gpint; } while(0)

#define hal_pltf_configure_spi_for_aci() do{\
  hal_pltf_spi_master_config(HAL_PLTF_SPI_FREQ_2MHZ, HAL_PLTF_SPI_ORDER_LSBIT_FIRST, HAL_PLTF_SPI_CLK_POLARITY_HIGH, HAL_PLTF_SPI_CLK_PH_SAMPLE_LEADING_EDGE_SHIFT_TRAILING_EDGE);\
}while(0)

#define hal_pltf_config_ready_line() do{\
  hal_pltf_enable_wakeup_from_pin();\
  hal_pltf_enable_wakeup_it();\
  hal_pltf_set_it_from_pin_on_falling_edge();\
  hal_pltf_select_gpint(HAL_PLTF_GPINT1);\
}while(0)
  

//register access macros for HAL_POWER
// RTC2CON
#define RTC2_SFRCPT   0x10
#define RTC2_CMPMOD1  0x04
#define RTC2_CMPMOD0  0x02
#define RTC2_ENABLE   0x01

// PWRDWN
#define PWRD_PIN_WU     0x80
#define PWRD_TICK_WU    0x40
#define PWRD_COMP_WU    0x20

#define PWRD_DEEPSLEEP  0x01
#define PWRD_MEMRET_NT  0x02
#define PWRD_MEMRET_T   0x03
#define PWRD_REGRET     0x04
#define PWRD_STANDBY    0x07

#define hal_pltf_get_timer_it_enable_state()            WUIRQ
#define hal_pltf_set_timer_it_enable_state(is_enabled)  do { WUIRQ = is_enabled; } while(0)
#define hal_pltf_enable_wakup_tick_pin()                do { WUCON = 0x2B; } while(0)
#define hal_pltf_enable_wakup_pin()                     do { WUCON = 0x3B; } while(0)
#define hal_pltf_sleep_memory_retention() do{\
  OPMCON |= 0x02;\
  PWRDWN = PWRD_MEMRET_T;\
}while(0)

#define hal_pltf_disable_timer()                                do { RTC2CON &= ~RTC2_ENABLE; } while(0)
#define hal_pltf_enable_timer()                                 do { RTC2CON |= RTC2_ENABLE; } while(0)
#define hal_pltf_load_timer_compare_val(sleep_time)             do { RTC2CMP = sleep_time; } while(0)
#define hal_pltf_set_timer_mode_it_enabled_reset_timer_on_it()  do { RTC2CON |= (RTC2_CMPMOD1 | RTC2_CMPMOD0); } while(0)

#define hal_pltf_sleep_register_retention() do{\
  PWRDWN = PWRD_REGRET;\
  PWRDWN = PWRD_STANDBY;\
}while(0)

#define hal_pltf_disable_wakeup_from_ready_line()  do { WUOPC0 = 0x00; } while(0)

#define hal_pltf_wake_up_delay() do{\
  uint8_t j;\
  for( j = 0; j < 100; j++)\
  {\
    _nop_();\
    _nop_();\
    _nop_();\
    _nop_();\
    _nop_();\
    _nop_();\
    _nop_();\
    _nop_();\
    _nop_();\
    _nop_();\
  }\
}while(0)


//macro for clk control_register
#define hal_pltf_config_clk_ctrl_rc_oscillator() do { CLKLFCTRL = 0x01; } while(0)

//macros to configure wake-up
#define hal_pltf_config_wkup_pin_low_polarity() do { OPMCON = 0x04; } while(0)
#define hal_pltf_enable_wkup_rdy_line()         do { WUOPC0 = 0x40; } while(0)
#define hal_pltf_enable_wkup_p12()              do { WUOPC1 = 0x04; } while(0)


#endif /* PLATFORM_H__ */
