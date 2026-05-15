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
#ifndef __ACMOTOR_H__
#define __ACMOTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _ACMOTOR_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

/* define const ------------------------------------------------------*/
/* Motor States */
#define MOTOR_STATE_STOPPED        0
#define MOTOR_STATE_RUNNING        1
#define MOTOR_STATE_ERROR             2
#define MOTOR_STATE_RECOVERING   3

/* Error Flags */
#define ERROR_NONE                              0
#define ERROR_ZERO_CROSS_LOST        1
#define ERROR_FEEDBACK_LOST             2
#define ERROR_PHASE_TIMER_FAULT     3
#define ERROR_DRIVER_OVERCURRENT  4  /* Placeholder - can be extended with ADC */

#define CONTINUE_MODE			0
#define DUTY_MODE				1
#define STEAM_MODE				2

#define DUTY_ON					2
#define DUTY_PERIOD				10
#define STEAM_CYCLE_PERIOD		34

PUBLIC uint8_t heat_motor_state;
PUBLIC uint8_t grind_motor_state;
PUBLIC uint8_t pumb_motor_state;
PUBLIC uint8_t pumb_motor_duty;
PUBLIC uint8_t pumb_motor_mode;
PUBLIC uint8_t motor_error;
PUBLIC uint16_t steam_cycle_counter;


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void gd_acmotor_Init(void);
void ac_grind_stop(void);
void ac_grind_start(uint16_t initial_speed_rpm);
void ac_pumb_stop(void);
void ac_pumb_start(uint8_t mode);
void ac_heat_stop(void);
void ac_heat_start(void);
void ac_motor_zeroCross_irqHandler(void);
void ac_grind_fb_irqHandler(uint32_t capture);
void ac_grind_nofb_irqHandler(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#undef PUBLIC
#endif /*__ __ACMOTOR_H__ */

