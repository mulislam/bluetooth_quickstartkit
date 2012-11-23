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


#include "hal_vdd.h"
#include "hal_platform.h"

#define ADC_PWRUP   0x80
#define ADC_BUSY    0x40
#define ADC_CHSEL3  0x20
#define ADC_CHSEL2  0x10
#define ADC_CHSEL1  0x08
#define ADC_CHSEL0  0x04
#define ADC_REFSEL1 0x02
#define ADC_REFSEL0 0x01

#define ADC_DIFFM1  0x80
#define ADC_DIFFM0  0x40
#define ADC_CONT    0x20
#define ADC_RATE2   0x10
#define ADC_RATE1   0x08
#define ADC_RATE0   0x04
#define ADC_TACQ1   0x02
#define ADC_TACQ0   0x01

#define ADC_RESOL1  0x80
#define ADC_RESOL0  0x40
#define ADC_RLJUST  0x20
#define ADC_UFLOW   0x10
#define ADC_OFLOW   0x08
#define ADC_RANGE   0x04

static void adc_conf_reset(void);
static uint16_t adc_sample(void);

static uint16_t gain_factor = 341;
static int16_t offset = 0;

void hal_vdd_init(void)
{
  adc_conf_reset();
  ADCCON1 |= (ADC_CHSEL3|ADC_CHSEL2|ADC_CHSEL1);  // Ch = 1/3 VDD, Ref = Internal 1.2V
  ADCCON2 |= (ADC_RATE0|ADC_TACQ1|ADC_TACQ0);     // Tacq = 36 us
  ADCCON3 |= (ADC_RESOL1|ADC_RLJUST);             // 10 bit resolution, right justified
}

uint16_t hal_vdd_get_level(void)
{
  uint8_t i;
  uint16_t adc_value = 0;

  for (i = 0; i < 4; i++)
  {
    adc_value += adc_sample();
  }

  // Converting the sum of 4 10-bit samples to the number of mV
  // Which is: (adc_value * 3600) / (1024 * 4)
  // Reduced to: (adc_value * 225) / 256

  // adc_value * 225
  MD0 = (uint8_t)(adc_value & 0x00FF);
  MD4 = 225;
  MD1 = (uint8_t)(adc_value >> 8);
  MD5 = 0;

  // Wait for the calculation to complete
  for (i = 0; i < 11; i++)
  {
    _nop_();
  }

  // Charging the result for division
  MD0 = MD0;
  MD1 = MD1;
  MD2 = MD2;
  MD3 = MD3;

  // Divide by 256 (0x0100)
  MD4 = 0x00;
  MD5 = 0x01;

  // Wait for the calculation to complete
  for (i = 0; i < 17; i++)
  {
    _nop_();
  }

  return (uint16_t)((MD1<<8) | MD0);
}

/** Sets the ADC config registers to their reset values
 */
static void adc_conf_reset(void)
{
  ADCCON1 = 0;
  ADCCON2 = 0;
  ADCCON3 = 0;
}

/** Starts the ADC and returns one sample
 * @return ADC data
 */
static uint16_t adc_sample(void)
{
  ADCCON1 |= ADC_PWRUP;
  _nop_();
  _nop_();
  _nop_();

  while (0 < (ADCCON1 & ADC_BUSY))
    ;

  return ((uint16_t)ADCDATH<<8)|(uint16_t)ADCDATL;
}
