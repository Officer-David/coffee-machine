
#define _DCMOTOR_C_
#include <stdlib.h>
#include "systick.h"
#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"
#include "dcmotor.h"

/* Pin definitions */
#define MOTOR_RIGHT_PIN   GPIO_PIN_14
#define MOTOR_RIGHT_PORT  GPIOB
#define MOTOR_LEFT_PIN    GPIO_PIN_15
#define MOTOR_LEFT_PORT   GPIOB
#define MOTOR_POS_PIN     GPIO_PIN_8
#define MOTOR_POS_PORT    GPIOA

/* Static variable: motor state */
static MotorState motor_state = MOTOR_STATE_STOPED;

/* Static function: set two control pins with interlock protection */
static void dc_motor_SetPins(uint8_t in1, uint8_t in2) {
    /* Interlock protection: do not allow both pins high */
    if (in1 && in2) {
        in1 = 0;
        in2 = 0;
    }
	
    if (in1){
        SetMotor_IN1_High();
    }
    else{
        ClrMotor_IN1_Low();	
    }
	
    if (in2){
        SetMotor_IN2_High();
    }
    else{
        ClrMotor_IN2_Low();	
    }
}

/* Stop motor and update state */
void dc_motor_Stop(void) {
    dc_motor_SetPins(0, 0);
    if (motor_state != MOTOR_STATE_FAULT) {
        motor_state = MOTOR_STATE_STOPED;
    }
}

/* Start motor with specified direction */
void dc_motor_Start(MotorDirection dir) {
    /* Block operation when in fault state */
    if (motor_state == MOTOR_STATE_FAULT) {
        return;
    }
#if 0	
    /* Check position signal first; if low, prevent start */
    if (MOTOR_DET_STATE() == RESET) {
        dc_motor_Stop();
        return;
    }
#endif	
    switch (dir) {
        case MOTOR_RIGHT:
            dc_motor_SetPins(1, 0);
            motor_state = MOTOR_STATE_RUNNING_RIGHT;
            break;
        case MOTOR_LEFT:
            dc_motor_SetPins(0, 1);
            motor_state = MOTOR_STATE_RUNNING_LEFT;
            break;
        default:
            dc_motor_Stop();
            break;
    }
}

/* Get current motor state */
MotorState dc_motor_GetState(void) {
    return motor_state;
}

/* Clear fault state (call externally after fault recovery) */
void dc_motor_ClearFault(void) {
    if (motor_state == MOTOR_STATE_FAULT) {
        motor_state = MOTOR_STATE_STOPED;
        dc_motor_Stop();   /* Ensure pins output stop level */
    }
}

/* irq interrupt handler (shared by EXTI lines 5~9) */
void dc_motor_reach_irqHandler(void) {
    /* Software debounce: delay then re-check level */
    if (MOTOR_DET_STATE() == RESET) {
        dc_motor_Stop();   /* Position reached, stop motor immediately */
    }
}

/* Initialize motor control pins and external interrupt */
void gd_dcmotor_Init(void) {
    /* Enable GPIO clocks */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    /* Enable AFIO clock */
    rcu_periph_clock_enable(RCU_AF);

    /* Configure PB14, PB15 as push-pull output, 50MHz */
    MOTOR_IN1_CONFIG();
    MOTOR_IN2_CONFIG();

    /* Initial state: motor stopped */
    dc_motor_SetPins(0, 0);

    /* Configure PA8 as floating input */
    MOTOR_DET_CONFIG();

    motor_state = MOTOR_STATE_STOPED;
}

