#ifndef __DCMOTOR_H
#define __DCMOTOR_H

#include "gd32f30x.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DCMOTOR_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

/* Motor direction enumeration */
typedef enum {
    MOTOR_STOP = 0,
    MOTOR_RIGHT,
    MOTOR_LEFT
} MotorDirection;

/* Motor state enumeration */
typedef enum {
    MOTOR_STATE_STOPED = 0,
    MOTOR_STATE_RUNNING_RIGHT,
    MOTOR_STATE_RUNNING_LEFT,
    MOTOR_STATE_FAULT
} MotorState;

/* Public APIs */
void gd_dcmotor_Init(void);                 /* Initialize motor control pins and interrupt */
void dc_motor_Start(MotorDirection dir);  /* Start motor with specified direction */
void dc_motor_Stop(void);                 /* Stop motor */
MotorState dc_motor_GetState(void);       /* Get current motor state */
void dc_motor_ClearFault(void);           /* Clear fault flag (to re-enable motor) */
void dc_motor_reach_irqHandler(void);

#ifdef __cplusplus
}
#endif

#undef PUBLIC#endif

