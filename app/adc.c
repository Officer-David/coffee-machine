/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#define _ADC_C_
/* Includes ------------------------------------------------------------------*/
#include "gd32f30x.h"
#include "configboard.h"
#include "systick.h"
#include "adc.h"

#define CHANNEL_NUM		3

uint32_t adc_value[CHANNEL_NUM];

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);  
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);
    /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable ADC1 clock */
    rcu_periph_clock_enable(RCU_ADC1);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* config the GPIO as analog mode */  
    ISEN_ADC_CONFIG();
    NTC_ADC_CONFIG();
    AC_ADC_CONFIG();
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    
    /* ADC_DMA_channel deinit */
    dma_deinit(DMA0, DMA_CH0);
    
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr = (uint32_t)(adc_value);
    dma_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_32BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_32BIT;
    dma_data_parameter.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number = CHANNEL_NUM;
    dma_data_parameter.priority = DMA_PRIORITY_HIGH;  
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
  
    dma_circulation_enable(DMA0, DMA_CH0);
  
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);  
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* configure the ADC sync mode */
    adc_mode_config(ADC_DAUL_REGULAL_PARALLEL);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
    /* ADC scan mode function enable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL,CHANNEL_NUM);
    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL,CHANNEL_NUM);
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_4, ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_8, ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_9, ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_8, ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(ADC1, 1, ADC_CHANNEL_9, ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(ADC1, 2, ADC_CHANNEL_4, ADC_SAMPLETIME_239POINT5);

    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);
   
    /* enable ADC interface */
    adc_enable(ADC0);
    delay_1ms(1);    
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    /* enable ADC interface */
    adc_enable(ADC1);    
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);
    
    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
}

/* USER CODE BEGIN 1 */
/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gd_adc_Init(void)
{
    /* system clocks configuration */
    rcu_config();
    /* GPIO configuration */
    gpio_config();
    /* DMA configuration */
    dma_config();
    /* ADC configuration */
    adc_config();
}
/* USER CODE END 1 */

uint16_t gd_adc_get(uint8_t channel)
{
	uint32_t adValue = 0u;

	switch (channel) {
	case AC_VOLTAGE:
		adValue = adc_value[0] & 0xFFFF;
		adValue += (adc_value[2] >> 16);
		adValue >>= 1;
		break;
	case MOTOR_ISEN:
		adValue = adc_value[1] & 0xFFFF;
		adValue += (adc_value[0] >> 16);
		adValue >>= 1;
		break;
	case TEMPERATURE:
		adValue = adc_value[2] & 0xFFFF;
		adValue += (adc_value[1] >> 16);
		adValue >>= 1;
		break;
	default:
		break;
	}
	return (uint16_t)(adValue & 0x0FFF);
}

/* USER CODE BEGIN 2 */
void gd_adc_trigger(void)
{
    uint16_t ac_cur_value = gd_adc_get(AC_VOLTAGE);
    ac_max_value = (ac_cur_value > ac_max_value)? ac_cur_value:ac_max_value;
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

/* USER CODE END 2 */
