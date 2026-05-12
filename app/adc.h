/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef _ADC_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#define AC_VOLTAGE		0
#define MOTOR_ISEN		1
#define TEMPERATURE		2

PUBLIC uint16_t ac_max_value;

/* USER CODE BEGIN Includes */
void gd_adc_Init(void);
void gd_adc_trigger(void);
uint16_t gd_adc_get(uint8_t channel);

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#undef PUBLIC
#endif /*__ GPIO_H__ */

