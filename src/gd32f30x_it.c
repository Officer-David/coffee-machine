/*!
    \file    gd32f30x_it.c
    \brief   interrupt service routines

   \version 2024-12-20, V3.0.1, firmware for GD32F30x
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x_it.h"
#include "main.h"
#include "systick.h"
#include "user_apiopt.h"

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    uwTick++;
    gd_adc_trigger();
}

/*!
    \brief      this function handles USART0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)){
        /* read one byte from the receive data register */
        com2rx_buffer[com2rx_w_counter++] = (uint8_t)usart_data_receive(USART1);
        if (com2rx_w_counter >= COM2_MAX_SIZE) {
            com2rx_w_counter = 0x0000;
        }
    }       
//    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TBE)){
//        /* write one byte to the transmit data register */
//        usart_data_transmit(USART1, tx1_buffer[tx1_counter++]);
//    }
}


/*!
    \brief      this function handles external lines 0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI0_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(EXTI_0))
    {
        //ToggleTest3Pin();
        exti_interrupt_flag_clear(EXTI_0);
    }
}

/**
    \brief      this function handles external lines 5_9 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI5_9_IRQHandler(void)
{
    if (exti_interrupt_flag_get(EXTI_9) == SET) {

        ac_motor_zeroCross_irqHandler();
        /* Clear interrupt flag */
        exti_interrupt_flag_clear(EXTI_9);
    }
    if (exti_interrupt_flag_get(EXTI_8) == SET) {

	   	//ToggleTest3Pin();
        dc_motor_reach_irqHandler();
        /* Clear interrupt flag */
        exti_interrupt_flag_clear(EXTI_8);
    }
}

/**
  * @brief  TIMER1 interrupt - speed feedback capture
  */
void TIMER1_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_CH1) == SET) {

        ac_grind_fb_irqHandler(timer_channel_capture_value_register_read(TIMER1, TIMER_CH_1));
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_CH1);
    }
    
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP) == SET) {

        // ac_grind_nofb_irqHandler();	
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    }
}

/**
  * @brief  TIMER2 interrupt - phase control update
  */
void TIMER2_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP) == SET) {
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
    }
}

/**
  * @brief  TIMER3 interrupt - phase control update (output off)
  */
void TIMER3_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP) == SET) {
        /* Turn on drive output */
        SetHeat_On();
        timer_disable(TIMER3);
        timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);

        timer_counter_value_config(TIMER4, 0);
        timer_autoreload_value_config(TIMER4, 600);	// triger pluse
        timer_enable(TIMER4);
    }
}

/**
  * @brief  TIMER6 interrupt - phase control update (output off)
  */
void TIMER4_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER4, TIMER_INT_FLAG_UP) == SET) {
        /* Turn off drive output */
        ClrHeat_Off();
        timer_disable(TIMER4);
        timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_UP);
    }
}

/**
  * @brief  TIMER5 interrupt - phase control update (output off)
  */
void TIMER5_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP) == SET) {
        /* Turn on drive output */
        if (grind_motor_state == MOTOR_STATE_RUNNING){
            SetGrind_On();
        }
        else if (pumb_motor_state == MOTOR_STATE_RUNNING){
            if (pumb_motor_mode == DUTY_MODE) {
                if (pumb_motor_duty < DUTY_ON){
                    SetPumb_On();
                }
                if (++pumb_motor_duty == DUTY_PERIOD){
                    pumb_motor_duty = 0;
                }
            }
            else if (pumb_motor_mode == STEAM_MODE) {
                /* STEAM_MODE: Turn on at position 0 and 1 in cycle, wait 3-36 */
                if (steam_cycle_counter == 0 || steam_cycle_counter == 1) {
                    SetPumb_On();
                }
                steam_cycle_counter++;
                if (steam_cycle_counter >= STEAM_CYCLE_PERIOD) {
                    steam_cycle_counter = 0;
                }
            }
            else {
                /* CONTINUE_MODE or other modes: turn on every time */
                SetPumb_On();
            }
        }
        timer_disable(TIMER5);
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);

        timer_counter_value_config(TIMER6, 0);
        timer_autoreload_value_config(TIMER6, 6300);	// triger pluse
        timer_enable(TIMER6);
    }
}

/**
  * @brief  TIMER6 interrupt - phase control update (output off)
  */
void TIMER6_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP) == SET) {
        /* Turn off drive output */
        ClrGrind_Off();
        ClrPumb_Off();
        timer_disable(TIMER6);
        timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP);
    }
}

