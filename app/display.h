/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    display.h
  * @brief   This file contains all the function prototypes for
  *          the display.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DISPLAY_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

/* define const ------------------------------------------------------*/
#define TOG_LED(x)		virtul_led ^= BIT(x)
#define SET_LED(x)		virtul_led |= BIT(x)
#define CLR_LED(x)		virtul_led &= ~BIT(x)
#define GET_LED(x)		(virtul_led & BIT(x))

#define LED_OFF			0
#define LED_ON			1
#define LED_TOGGLE		2

#define LED1			0		// nc
#define COFFEE1_LED		1
#define CALC_CLEAN_LED	2
#define LED4			3		// nc
#define WATER1_LED		4
#define START_LED		5
#define AMERICANO2_LED	6
#define LED8			7		// nc
#define PALLET_ERR_LED	8
#define ESPRESSO2_LED	9
#define COFFEE2_LED		10
#define LED12			11		// nc
#define ESPRESSO1_LED	12
#define COFFEE3_LED		13
#define WATER_ERR_LED	14
#define AMERICANO1_LED	15
#define WATER3_LED		16
#define AQUA_CLEAN_LED	17
#define STEAM_LED		18
#define COFFEE4_LED		19
#define LED21			20		// nc
#define HOT_WATER		21
#define LED23			22		// nc
#define COFFEE_FULL_LED	23
#define LED25			24		// nc
#define WATER2_LED		25
#define AQUA_GREEN_LED	26		
#define LED28			27		// nc
#define LED29			28		// nc

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
PUBLIC uint32_t virtul_led;	

/* USER CODE END Private defines */

void display_init(void);
uint8_t display_get_led(uint8_t led);
void display_led(uint8_t led, uint8_t bitval);
void app_virtual_ledtest_cbfunc(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#undef PUBLIC
#endif /*__ DISPLAY_H__ */

