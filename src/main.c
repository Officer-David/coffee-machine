/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

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

#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "user_apiopt.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
#ifdef __FIRMWARE_VERSION_DEFINE
    uint32_t fw_ver = 0;
#endif
    /* configure systick */
    systick_config();
    /* Initialize all peripherals */
    gd_remap_Init();
    gd_gpio_Init();
    /* motor init */
    gd_acmotor_Init();
    gd_dcmotor_Init();
    /* relay init */
    gd_realy_init();
    /* configure com */
    gd_usart_Init();
    /* init ext. interrupt */
    gd_exti_Init();
    /* init timer  */
    gd_timer_Init();
    /* init adc  */
    gd_adc_Init();
    /* init watchdog */
    gd_watchdog_init();
    /* print out the clock frequency of system, AHB, APB1 and APB2 */
    printf("\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
    printf("\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
    printf("\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
    printf("\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));
#ifdef __FIRMWARE_VERSION_DEFINE
    fw_ver = gd32f30x_firmware_version_get();
    /* print firmware version */
    printf("\r\nGD32F30x series firmware version: V%d.%d.%d\r\n", (uint8_t)(fw_ver >> 24), (uint8_t)(fw_ver >> 16), (uint8_t)(fw_ver >> 8));
#endif /* __FIRMWARE_VERSION_DEFINE */
  //while (1)
  {
    /* USER CODE END WHILE */
    osal_lyh_core_init();
    osal_create_task(init_key_task, key_task_handler, key_event_handler);
    osal_create_task(init_bus_task, bus_task_handler, bus_event_handler);
    osal_create_task(init_app_task, app_task_handler, app_event_handler);
    osal_create_task(init_heat_task, heat_task_handler, heat_event_handler);
    //osal_create_task(init_test_task, test_task_handler, test_event_handler);
    osal_start_system();
    /* USER CODE BEGIN 3 */
  }
}

#ifdef GD_ECLIPSE_GCC
/* retarget the C library printf function to the USART, in Eclipse GCC environment */
int __io_putchar(int ch)
{
    uint8_t txdata = (uint8_t)ch;
    gd_usart1_upload(&txdata, 1);
    return ch;
}
#else
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    uint8_t txdata = (uint8_t)ch;
    gd_usart1_upload(&txdata, 1);
    return ch;
}
#endif /* GD_ECLIPSE_GCC */

/**
  * @brief  Initialize independent watchdog (FWDGT) for system monitoring
  * @param  None
  * @retval None
  */
void gd_watchdog_init(void)
{
    /* Enable write access to FWDGT registers */
    fwdgt_write_enable();
    
    /* Configure watchdog: 40kHz / 256 = 156.25Hz, reload = 0x0FFF -> ~26 seconds */
    fwdgt_config(0xFFF, FWDGT_PSC_DIV256);
    
    /* Start watchdog */
    fwdgt_enable();
    
    /* Lock registers */
    fwdgt_write_disable();
}


