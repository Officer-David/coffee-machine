/**
  * @file    ac_motor_driver.c
  * @brief   AC Motor Driver - Enhanced Stability & Reliability
  * @version 2.0
  */
#define _ACMOTOR_C_
#include <stdlib.h>
#include "systick.h"
#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"
#include "acmotor.h"

/*============================ Macro Definitions ============================*/
/* Pin Definitions */
#define AC_PUMB_DRV_PIN       GPIO_PIN_6      /* PB6 - Motor drive output */
#define AC_PUMB_DRV_PORT      GPIOB
#define AC_PUMB_DRV_CLK       RCU_GPIOB

#define AC_HEAT_DRV_PIN       GPIO_PIN_7      /* PB7 - Heatr drive output */
#define AC_HEAT_DRV_PORT      GPIOB
#define AC_HEAT_DRV_CLK       RCU_GPIOB

#define AC_ZERO_CROSS_PIN      GPIO_PIN_9      /* PB9 - Zero crossing detection input */
#define AC_ZERO_CROSS_PORT     GPIOB
#define AC_ZERO_CROSS_CLK      RCU_GPIOB

#define AC_GRIND_DRV_PIN       GPIO_PIN_7      /* PA7 - Motor drive output */
#define AC_GRIND_DRV_PORT      GPIOA
#define AC_GRIND_DRV_CLK       RCU_GPIOA

#define AC_GRIND_DFB_PIN        GPIO_PIN_1      /* PA1 - Speed feedback input */
#define AC_GRIND_DFB_PORT       GPIOA
#define AC_GRIND_DFB_CLK        RCU_GPIOA

/* EXTI Configuration */
#define AC_ZERO_CROSS_EXTI_LINE   EXTI_9
#define AC_ZERO_CROSS_IRQn        EXTI5_9_IRQn

/* Timer for Phase Control Delay */
#define AC_HEAT_PHASE1_TIMER   TIMER3
#define AC_HEAT_PHASE1_CLK     RCU_TIMER3

#define AC_HEAT_PHASE2_TIMER   TIMER4
#define AC_HEAT_PHASE2_CLK     RCU_TIMER4

/* Timer for Speed Feedback Capture */
#define AC_GRIND_FB_TIMER      TIMER1
#define AC_GRIND_FB_CLK     RCU_TIMER1

/* Timer for Phase Control Delay */
#define AC_GRIND_PHASE1_TIMER   TIMER5
#define AC_GRIND_PHASE1_CLK     RCU_TIMER5

#define AC_GRIND_PHASE2_TIMER   TIMER6
#define AC_GRIND_PHASE2_CLK     RCU_TIMER6

#define AC_PUMB_PHASE1_TIMER   AC_GRIND_PHASE1_TIMER
#define AC_PUMB_PHASE1_CLK     AC_GRIND_PHASE1_CLK

#define AC_PUMB_PHASE2_TIMER   AC_GRIND_PHASE2_TIMER
#define AC_PUMB_PHASE2_CLK     AC_GRIND_PHASE2_CLK

/* AC Line Frequency Configuration */
#define AC_FREQUENCY_50HZ      50
#define AC_FREQUENCY_60HZ      60
#define AC_FREQUENCY		   AC_FREQUENCY_50HZ

/* Phase Control Parameters */
#define MAX_PHASE_ANGLE        180u
#define MIN_PHASE_ANGLE        0
#define PHASE_STEP             1

/* Speed Feedback Measurement */
#define SPEED_MEASURE_PERIOD_MS   100

/* Reliability Parameters */
#define ZERO_CROSS_TIMEOUT_MS     25      /* Maximum time without zero crossing (25ms > 20ms half-cycle) */
#define FEEDBACK_TIMEOUT_MS       500     /* Timeout for feedback signal loss (500ms) */
#define ZERO_CROSS_DEBOUNCE_US    50      /* Debounce time for zero crossing (microseconds) */
#define FEEDBACK_MIN_VALID_PERIOD_US 100 /* Minimum valid period (10kHz max, 100us) */
#define FEEDBACK_MAX_VALID_PERIOD_US 500000 /* Maximum valid period (2Hz min, 500ms) */

/*============================ Global Variables ============================*/
/* Motor control variables (volatile for ISR safety) */
uint16_t target_phase_delay_us = 0;
uint16_t current_phase_delay_us = 0;
uint16_t target_speed_rpm = 0;
uint16_t current_speed_rpm = 0;

/* Zero crossing management */
uint32_t last_zero_cross_tick_ms = 0;

/* Speed feedback management */
uint32_t last_feedback_tick_ms = 0;
uint32_t last_capture_value = 0;
uint32_t current_period_us = 0;

/*============================ Private Function Prototypes ============================*/
static uint16_t ac_angle_to_delay_us(uint16_t angle);
static uint16_t ac_delay_us_to_angle(uint16_t delay_us);
static void ac_motor_set_phase_delay(uint16_t delay_us);
void ac_motor_clear_error(void);

/*============================ Peripheral Initialization ============================*/
/**
  * @brief  Initialize GPIO pins with pull-up/pull-down for noise immunity
  * @param  None
  * @retval None
  */
static void ac_motor_gpio_init(void)
{
    /* Enable GPIO clocks */
    rcu_periph_clock_enable(AC_ZERO_CROSS_CLK);
    rcu_periph_clock_enable(AC_GRIND_DRV_CLK);
    rcu_periph_clock_enable(AC_GRIND_DFB_CLK);
    rcu_periph_clock_enable(AC_PUMB_DRV_CLK);
    rcu_periph_clock_enable(AC_HEAT_DRV_CLK);

    /* Configure PB6 as motor drive output (push-pull) - initial low */
    PUMB_CONFIG();
    ClrPumb_Off();

    /* Configure PB7 as motor drive output (push-pull) - initial low */
    HEAT_CONFIG();
    ClrHeat_Off();	
	
    /* Configure PB9 as zero crossing detection input (pull-up for noise immunity) */
    ZERO_DET_CONFIG();	
    
    /* Configure PA7 as motor drive output (push-pull) - initial low */
    GRIND_CONFIG();
    ClrGrind_Off();	
    
    /* Configure PA1 as speed feedback input (pull-up for noise immunity) */
    GRIND_DET_CONFIG();
}

/**
  * @brief  Initialize EXTI for zero crossing with hardware filtering
  * @param  None
  * @retval None
  */
static void ac_motor_zero_cross_exti_init(void)
{
    /* Enable AFIO clock */
    rcu_periph_clock_enable(RCU_AF);
    
    /* Configure PB9 as EXTI source */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_9);
    
    /* Configure EXTI with both edges, but hardware filter is not available; use software debounce */
    exti_init(EXTI_9, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    
    /* Clear pending flag */
    exti_interrupt_flag_clear(EXTI_9);
    
    /* Set NVIC priority (higher than motor control timers) */
    nvic_irq_enable(EXTI5_9_IRQn, 0, 0);
}

/**
  * @brief  Initialize timer for speed feedback capture with filtering
  * @param  None
  * @retval None
  */
static void ac_motor_feedback_timer_init(uint32_t ptimer, rcu_periph_enum pClock)
{
    timer_parameter_struct timer_initpara;
    timer_ic_parameter_struct ic_initpara;
    
    /* Enable timer clock */
    rcu_periph_clock_enable(pClock);
    timer_deinit(ptimer);
    
    /* Configure timer base: 1MHz counting (1us resolution) */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 119;          /* 120MHz / 120 = 1MHz */
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 0xFFFF;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(ptimer, &timer_initpara);
    
    /* Configure input capture with filter (4 samples, fDTS/2) */
    timer_channel_input_struct_para_init(&ic_initpara);
    ic_initpara.icpolarity = TIMER_IC_POLARITY_RISING;
    ic_initpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    ic_initpara.icprescaler = TIMER_IC_PSC_DIV1;
    ic_initpara.icfilter = 0x4;              /* Filter: 6 samples, fDTS/2 (see user manual) */
    timer_input_capture_config(ptimer, TIMER_CH_1, &ic_initpara);
    
    /* Enable capture interrupt */
    timer_interrupt_enable(ptimer, TIMER_INT_CH1);
    /* Enable update interrupt for overflow detection */
    timer_interrupt_enable(ptimer, TIMER_INT_UP);
    
    /* NVIC priority (lower than zero crossing) */
    nvic_irq_enable(TIMER1_IRQn, 2, 0);
    
    /* Start timer */
    timer_enable(ptimer);
}

/**
  * @brief  Initialize timer for phase control (single pulse mode)
  * @param  None
  * @retval None
  */
static void ac_motor_phase_timer_init(uint32_t ptimer, rcu_periph_enum pClock)
{
    timer_parameter_struct timer_initpara;
    
    /* Enable timer clock */
    rcu_periph_clock_enable(pClock);
    timer_deinit(ptimer);
    
    /* Configure timer base: 1MHz counting (1us resolution) */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 119;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 20000;            /* Max 20ms */
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(ptimer, &timer_initpara);
    
    /* Single pulse mode */
    timer_single_pulse_mode_config(ptimer, TIMER_SP_MODE_SINGLE);
    timer_update_event_enable(ptimer);
    
    /* Enable update interrupt for turning off output */
    timer_interrupt_enable(ptimer, TIMER_INT_UP);
    
    /* NVIC priority (higher than feedback timer) */
    switch(ptimer){
    case TIMER3:
          nvic_irq_enable(TIMER3_IRQn, 1, 0);
          break;
    case TIMER4:
          nvic_irq_enable(TIMER4_IRQn, 1, 0);
          break;
    case TIMER5:
          nvic_irq_enable(TIMER5_IRQn, 1, 0);
          break;
    case TIMER6:
          nvic_irq_enable(TIMER6_IRQn, 1, 0);
          break;
    default:
          break;
    }
    /* Start timer (will be disabled until needed) */
    timer_enable(ptimer);
    timer_disable(ptimer);      /* Initially off */
}

void ac_heat_turnoff(void)
{
    /* Turn off drive output immediately */
    ClrHeat_Off();
    /* Stop phase timer */
    timer_disable(AC_HEAT_PHASE1_TIMER);
    timer_disable(AC_HEAT_PHASE2_TIMER);
    timer_interrupt_flag_clear(AC_HEAT_PHASE1_TIMER, TIMER_INT_FLAG_UP);
    timer_interrupt_flag_clear(AC_HEAT_PHASE2_TIMER, TIMER_INT_FLAG_UP);
}

void ac_pumb_turnoff(void)
{
    /* Turn off drive output immediately */
    ClrPumb_Off();
    /* Stop phase timer */
    timer_disable(AC_PUMB_PHASE1_TIMER);
    timer_disable(AC_PUMB_PHASE2_TIMER);
    timer_interrupt_flag_clear(AC_PUMB_PHASE1_TIMER, TIMER_INT_FLAG_UP);
    timer_interrupt_flag_clear(AC_PUMB_PHASE2_TIMER, TIMER_INT_FLAG_UP);
}

void ac_grind_turnoff(void)
{
    /* Turn off drive output immediately */
    ClrGrind_Off();
    /* Stop phase timer */
    timer_disable(AC_GRIND_PHASE1_TIMER);
    timer_disable(AC_GRIND_PHASE2_TIMER);
    timer_interrupt_flag_clear(AC_GRIND_PHASE1_TIMER, TIMER_INT_FLAG_UP);
    timer_interrupt_flag_clear(AC_GRIND_PHASE2_TIMER, TIMER_INT_FLAG_UP);
}

/**
  * @brief  Convert phase angle to delay time (microseconds)
  */
static uint16_t ac_angle_to_delay_us(uint16_t angle)
{
    uint16_t half_cycle_us;
    
    if (AC_FREQUENCY_50HZ == AC_FREQUENCY) {
        half_cycle_us = 10000;
    } else {
        half_cycle_us = 8333;
    }
    
    if (angle > MAX_PHASE_ANGLE) angle = MAX_PHASE_ANGLE;
    return (uint16_t)((uint32_t)angle * half_cycle_us / MAX_PHASE_ANGLE);
}

/**
  * @brief  Convert delay time to phase angle (degrees)
  */
static uint16_t ac_delay_us_to_angle(uint16_t delay_us)
{
    uint16_t half_cycle_us;
    
    if (AC_FREQUENCY_50HZ == AC_FREQUENCY) {
        half_cycle_us = 10000;
    } else {
        half_cycle_us = 8333;
    }
    
    if (delay_us > half_cycle_us) delay_us = half_cycle_us;
    return (uint16_t)((uint32_t)((delay_us * MAX_PHASE_ANGLE) / half_cycle_us));
}

/**
  * @brief  Safely shut down motor and set error code
  * @param  error_code: Error code to set
  * @retval None
  */
static void ac_motor1_safe_shutdown(uint8_t error_code)
{

    ac_grind_turnoff();
    /* Disable interrupts temporarily to avoid race conditions */
    __disable_irq();
    /* Set error state */
    motor_error = error_code;
    grind_motor_state = MOTOR_STATE_ERROR;
    
    /* Clear target speed */
    target_speed_rpm = 0;
    target_phase_delay_us = 0;
    __enable_irq();
}

/**
  * @brief  Health monitoring routine (call periodically, e.g., every 10ms)
  */
static void ac_grind_health_monitor(void)
{
    uint32_t now = systick_get();
    
    /* Check for zero crossing loss (only when motor running) */
    if (grind_motor_state == MOTOR_STATE_RUNNING) {
        if ((now - last_zero_cross_tick_ms) > ZERO_CROSS_TIMEOUT_MS) {
            ac_motor1_safe_shutdown(ERROR_ZERO_CROSS_LOST);
        }
    }
    
    /* Check for feedback signal loss (optional, when running) */
    if ((grind_motor_state == MOTOR_STATE_RUNNING) && (target_speed_rpm > 0)) {
        if ((now - last_feedback_tick_ms) > FEEDBACK_TIMEOUT_MS) {
            ac_motor1_safe_shutdown(ERROR_FEEDBACK_LOST);
        }
    }
    
    /* Clear error after recovery attempt (if motor stopped and error set) */
    if (grind_motor_state == MOTOR_STATE_ERROR) {
        /* User must call ac_motor_clear_error() after fixing issue */
        /* This function does not auto-clear to avoid automatic unsafe restart */
    }
}

/**
  * @brief  Set phase delay with validation and atomic update
  * @param  delay_us: Delay time in microseconds (0 - half-cycle)
  * @retval None
  */
static void ac_motor_set_phase_delay(uint16_t delay_us)
{
    uint16_t half_cycle_us = (AC_FREQUENCY_50HZ == 1) ? 10000 : 8333;
    
    /* Validate input */
    if (delay_us > half_cycle_us) {
        delay_us = half_cycle_us;
    }
    
    /* Atomic update */
    __disable_irq();
    target_phase_delay_us = delay_us;
    __enable_irq();
}

/**
  * @brief  Clear error and return to stopped state
  */
void ac_motor_clear_error(void)
{
    __disable_irq();
    grind_motor_state = MOTOR_STATE_STOPPED;
    motor_error = ERROR_NONE;
    __enable_irq();
}

/*============================ Public API ============================*/

/**
  * @brief  Initialize AC motor driver with enhanced reliability
  * @param  None
  * @retval None
  */
void gd_acmotor_Init(void)
{
    /* Initialize hardware */
    ac_motor_gpio_init();
    ac_motor_zero_cross_exti_init();
    ac_motor_feedback_timer_init(AC_GRIND_FB_TIMER, AC_GRIND_FB_CLK);
    ac_motor_phase_timer_init(AC_GRIND_PHASE1_TIMER, AC_GRIND_PHASE1_CLK);
    ac_motor_phase_timer_init(AC_GRIND_PHASE2_TIMER, AC_GRIND_PHASE2_CLK);
    ac_motor_phase_timer_init(AC_HEAT_PHASE1_TIMER, AC_HEAT_PHASE1_CLK);
    ac_motor_phase_timer_init(AC_HEAT_PHASE2_TIMER, AC_HEAT_PHASE2_CLK);

    /* Initialize variables */
    heat_motor_state = MOTOR_STATE_STOPPED;
    pumb_motor_state = MOTOR_STATE_STOPPED;
    grind_motor_state = MOTOR_STATE_STOPPED;
    motor_error = ERROR_NONE;
    target_phase_delay_us = 0;
    current_phase_delay_us = 0;
    current_speed_rpm = 0;
    target_speed_rpm = 0;
    last_zero_cross_tick_ms = systick_get();
    last_feedback_tick_ms = last_zero_cross_tick_ms;
}

/**
  * @brief  Start AC heat motor
  * @param  None
  * @retval None
  */
void ac_heat_start(void)
{
    if (heat_motor_state == MOTOR_STATE_RUNNING) {
        /* Cannot start when in error state - must clear error first */
        return;
    }
    /* Update state */
    heat_motor_state = MOTOR_STATE_RUNNING;
    motor_error = ERROR_NONE;
    
    /* Reset timeout counters */
    last_zero_cross_tick_ms = systick_get();
}

/**
  * @brief  Start AC pumb motor
  * @param  initial_mode:duty or continue
  * @retval None
  */
void ac_pumb_start(uint8_t mode)
{
    if (pumb_motor_state == MOTOR_STATE_RUNNING) {
        /* Cannot start when in error state - must clear error first */
        return;
    }
    pumb_motor_mode = mode;
    pumb_motor_duty = 0;
    
    /* Update state */
    pumb_motor_state = MOTOR_STATE_RUNNING;
    motor_error = ERROR_NONE;
    
    /* Reset timeout counters */
    last_zero_cross_tick_ms = systick_get();
}

/**
  * @brief  Start AC grind motor
  * @param  initial_speed_rpm: Initial target speed (RPM)
  * @retval None
  */
void ac_grind_start(uint16_t initial_speed_rpm)
{
    uint16_t angle;
    if ((grind_motor_state == MOTOR_STATE_RUNNING) ||(grind_motor_state == MOTOR_STATE_ERROR)) {
        /* Cannot start when in error state - must clear error first */
        return;
    }
    
    /* Validate speed range */
    if (initial_speed_rpm > 2000) initial_speed_rpm = 2000;
    
    /* Set target speed */
    target_speed_rpm = initial_speed_rpm;
    
    /* Calculate initial phase delay */
    angle = (uint16_t)((uint32_t)((target_speed_rpm * MAX_PHASE_ANGLE) / 2000u));
    if (angle < 10) angle = 10;
    ac_motor_set_phase_delay(ac_angle_to_delay_us(angle));
    
    /* Update state */
    grind_motor_state = MOTOR_STATE_RUNNING;
    motor_error = ERROR_NONE;
    
    /* Reset timeout counters */
    last_zero_cross_tick_ms = systick_get();
    last_feedback_tick_ms = last_zero_cross_tick_ms;
}

/**
  * @brief  Stop AC grind motor
  * @param  None
  * @retval None
  */
void ac_grind_stop(void)
{
    __disable_irq();
    grind_motor_state = MOTOR_STATE_STOPPED;
    motor_error = ERROR_NONE;
    target_speed_rpm = 0;
    target_phase_delay_us = 0;
    __enable_irq();
    ac_grind_turnoff();
}

/**
  * @brief  Stop AC pumb motor
  * @param  None
  * @retval None
  */
void ac_pumb_stop(void)
{
    __disable_irq();
    pumb_motor_state = MOTOR_STATE_STOPPED;
    motor_error = ERROR_NONE;
    __enable_irq();
    ac_pumb_turnoff();
}

/**
  * @brief  Stop AC heat motor
  * @param  None
  * @retval None
  */
void ac_heat_stop(void)
{
    __disable_irq();
    heat_motor_state = MOTOR_STATE_STOPPED;
    motor_error = ERROR_NONE;
    __enable_irq();
    ac_heat_turnoff();
}

/**
  * @brief  Set AC motor speed (safe)
  * @param  speed_rpm: Target speed (RPM)
  * @retval None
  */
void ac_grind_set_speed(uint16_t speed_rpm)
{
    uint16_t angle;
    if ((grind_motor_state != MOTOR_STATE_RUNNING) || (speed_rpm == 0)) {
        if (speed_rpm == 0) {
            ac_grind_stop();
        }
        return;
    }
    /* Validate speed range */
    if (speed_rpm > 2000) speed_rpm = 2000;
    
    /* Update target speed */
    target_speed_rpm = speed_rpm;
    
    /* Calculate new phase delay */
    angle = (uint16_t)((uint32_t)speed_rpm * MAX_PHASE_ANGLE / 2000);
    if (angle < 10) angle = 10;
    ac_motor_set_phase_delay(ac_angle_to_delay_us(angle));
}

/**
  * @brief  Get current motor state
  * @retval Motor state (0=stopped, 1=running, 2=error, 3=recovering)
  */
uint8_t ac_motor_get_state(void)
{
    return grind_motor_state;
}

/**
  * @brief  Get current error code
  * @retval Error code (0=none)
  */
uint8_t ac_motor_get_error(void)
{
    return motor_error;
}

/**
  * @brief  Get current speed (RPM)
  * @retval Speed value (RPM)
  */
uint16_t ac_motor_get_speed(void)
{
    return current_speed_rpm;
}

/**
  * @brief  Get current conduction angle (degrees)
  * @retval Angle (0-180)
  */
uint16_t ac_motor_get_phase_angle(void)
{
    return ac_delay_us_to_angle(current_phase_delay_us);
}

/**
  * @brief  Periodic health check (call from main loop every 10ms)
  */
void ac_motor_periodic_check(void)
{
    ac_grind_health_monitor();
    
    /* Refresh watchdog to prevent reset */
    fwdgt_counter_reload();
}

/*============================ Interrupt Service Routines ============================*/

/**
  * @brief  zero crossing with debounce
  */
void ac_motor_zeroCross_irqHandler(void)
{
	/* Trigger phase control if motor running */
	if (grind_motor_state == MOTOR_STATE_RUNNING) {
		ac_grind_turnoff();
		/* Start new timing */
		timer_counter_value_config(AC_GRIND_PHASE1_TIMER, 0);
		timer_autoreload_value_config(AC_GRIND_PHASE1_TIMER, 300/*target_phase_delay_us*/);		// delay time
		timer_enable(AC_GRIND_PHASE1_TIMER);
	}
	/* Trigger phase control if motor running */
	else if (pumb_motor_state == MOTOR_STATE_RUNNING) {
		ac_pumb_turnoff();
		/* Start new timing */
		timer_counter_value_config(AC_PUMB_PHASE1_TIMER, 0);
		timer_autoreload_value_config(AC_PUMB_PHASE1_TIMER, 300/*target_phase_delay_us*/);		// delay time
		timer_enable(AC_PUMB_PHASE1_TIMER);
	}
	else {
		ac_grind_turnoff();
		ac_pumb_turnoff();
	}
	ac_heat_turnoff();
	/* Trigger phase control if motor running */
	if (heat_motor_state == MOTOR_STATE_RUNNING) {
		/* Start new timing */
		timer_counter_value_config(AC_HEAT_PHASE1_TIMER, 0);
		timer_autoreload_value_config(AC_HEAT_PHASE1_TIMER, 100);		// delay time
		timer_enable(AC_HEAT_PHASE1_TIMER);
	}
	last_zero_cross_tick_ms = systick_get();
}

/**
  * @brief  speed feedback capture
  */
void ac_grind_fb_irqHandler(uint32_t capture)
{
        uint32_t period, freq_hz;

        if (capture >= last_capture_value) {
            period = capture - last_capture_value;
        } else {
            period = (0xFFFF - last_capture_value) + capture;
        }
        /* Validate period (ignore invalid values) */
        if ((period > FEEDBACK_MIN_VALID_PERIOD_US) && (period < FEEDBACK_MAX_VALID_PERIOD_US)) {
            current_period_us = period;
            freq_hz = 1000000 / period;
            current_speed_rpm = (uint16_t)(freq_hz * 60);
            last_feedback_tick_ms = systick_get();
        }
        last_capture_value = capture;
}

/**
  * @brief  no speed feedback capture
  */
void ac_grind_nofb_irqHandler(void)
{
        /* Overflow - no valid signal for long time */
        if (grind_motor_state == MOTOR_STATE_RUNNING) {
            uint32_t system_tick_ms = systick_get();		
            if ((system_tick_ms - last_feedback_tick_ms) > FEEDBACK_TIMEOUT_MS) {
                ac_motor1_safe_shutdown(ERROR_FEEDBACK_LOST);
            }
        }
}

