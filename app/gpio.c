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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
void gd_remap_Init(void)
{
	/* GPIO Ports Clock Enable */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	gpio_pin_remap_config(GPIO_PD01_REMAP, ENABLE);
}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void gd_gpio_Init(void)
{
  /* GPIO Ports Clock Enable */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);

    /* configure key pin as input */
    WKUP_CONFIG();
    P3V_DET_CONFIG();
    WATER_DET_CONFIG();
    METER_DET_CONFIG();
    PALLET_DET_CONFIG();
    MOTOR_DET_CONFIG();

    //BUZZER_CONFIG();
    //ClrBUZZER_Low();
    RELAY1_CONFIG();
    ClrRelay1_Off();
    RELAY2_CONFIG();
    ClrRelay2_Off();

    GRIND_CONFIG();
    ClrGrind_Off();
   
    POWER_5V_CONFIG();
    SetPower_5V_On();

    POWER_3V_CONFIG();
    SetPower_3V_On();

    IIC_SCL_CONFIG();
    Setiic_scl_High();

    IIC_SDA_CONFIG();
    Setiic_sda_High();

    PUMB_CONFIG();
    ClrPumb_Off();

    HEAT_CONFIG();
    ClrHeat_Off();	
	
    ZERO_CONFIG();	
    ClrZero_Off();		// alway low, no used

    MOTOR_IN1_CONFIG();
    ClrMotor_IN1_Low();

    MOTOR_IN2_CONFIG();
    ClrMotor_IN2_Low();

    DBG_TEST3_CONFIG();
    DBG_TEST4_CONFIG();
    NC2_CONFIG()
    NC3_CONFIG()
    NC4_CONFIG()
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
