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
#define _DISPLAY_C_
#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"
#include "display.h"
#include "app.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure display                                                            */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
void display_init(void)
{
	virtul_led = 0;
	//app_virtual_ledtest_cbfunc();
}

void display_led(uint8_t led, uint8_t bitval)
{
	if (bitval == LED_OFF) {
		CLR_LED(led);
	}
	else if (bitval == LED_ON){
		SET_LED(led);
	}
	else {
		TOG_LED(led);
	}
}

uint8_t display_get_led(uint8_t led)
{
	return GET_LED(led)? LED_ON:LED_OFF;
}

/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
void app_virtual_ledtest_cbfunc(void)
{
	virtul_led <<= 1;
	if (!virtul_led) {
		virtul_led = 1u;
	}
	osal_lyh_over_timer(OS_VIRUTAL_LED, Ms_1000, app_virtual_ledtest_cbfunc);
}

/* USER CODE END 2 */
