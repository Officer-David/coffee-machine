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
#include "gd32f30x.h"
#include "configboard.h"
#include "extint.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void gd_exti_Init(void)
{
    /* enable the key clock */
    rcu_periph_clock_enable(RCU_AF);

    /* connect zero EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);

    /* configure zero EXTI line */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_flag_clear(EXTI_0);

    /* enable and set zero EXTI interrupt to the lowest priority */
    nvic_irq_enable(EXTI0_IRQn, 2U, 0U);

    /* connect 8 EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_8);

    /* configure 8 EXTI line */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_8);

    /* enable and set zero EXTI interrupt to the lowest priority */
    nvic_irq_enable(EXTI5_9_IRQn, 0U, 0U);
}

/* USER CODE END 1 */

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
