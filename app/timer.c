/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
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

/* Includes ------------------------------------------------------------------*/
#define _TIMER_C_
#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"
#include "timer.h"

#define BUZZ_CLK				500
#define BUZZ_PERIOD(x)		(1000000/x)

#define T2_CLK				1000
#define T2_PERIOD(x)			(1000000/x)

/* USER CODE BEGIN 0 */
void gpio_buz_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    
    /*Configure PA11(TIMER0_CH3) as alternate function*/
    BUZZER_CONFIG();	
}

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure display                                                            */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer0_pwm_config(void)
{
    /* TIMER0 configuration: generate PWM signals with different duty cycles:
       TIMER0CLK = SystemCoreClock / 120 = 1MHz */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER0);
    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = BUZZ_PERIOD(BUZZ_CLK);
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);

     /* CH0 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER0,TIMER_CH_3,&timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3, 0/*BUZZ_PERIOD(BUZZ_CLK)/2*/);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

    timer_primary_output_config(TIMER0, ENABLE);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);
}

void timer0_init(void)
{
    /* configure the GPIO ports */
    gpio_buz_config();
	
    /* configure the timer peripheral */
    timer0_pwm_config();
}

void timer2_init(void)
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);
    timer_deinit(TIMER2);

    /* TIMER2 configuration */
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = T2_PERIOD(T2_CLK);
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* Enable update interrupt  */
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    /* NVIC priority (higher than feedback timer) */
    nvic_irq_enable(TIMER2_IRQn, 1, 0);
    /* Start timer  */
    timer_enable(TIMER2);
}

void gd_timer_Init(void)
{
	timer0_init();
	//timer2_init();
}

void gd_timer_buz(uint32_t  status)
{
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3, status? (BUZZ_PERIOD(BUZZ_CLK)/2):0);
}


/* USER CODE END 1 */

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
