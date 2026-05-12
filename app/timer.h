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
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _TIMER_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

/* define const ------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void gd_timer_Init(void);
void gd_timer_buz(uint32_t  status);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#undef PUBLIC
#endif /*__ DISPLAY_H__ */

