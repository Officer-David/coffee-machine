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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USART_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

/* define const ------------------------------------------------------*/
#define COMn               2U

#define COM1               USART0
#define COM1_CLK           RCU_USART0
#define COM1_TX_PIN        GPIO_PIN_9
#define COM1_RX_PIN        GPIO_PIN_10
#define COM1_GPIO_PORT     GPIOA
#define COM1_GPIO_CLK      RCU_GPIOA
#define COM1_MAX_SIZE      16

#define COM2               USART1
#define COM2_CLK           RCU_USART1
#define COM2_TX_PIN        GPIO_PIN_2
#define COM2_RX_PIN        GPIO_PIN_3
#define COM2_GPIO_PORT     GPIOA
#define COM2_GPIO_CLK      RCU_GPIOA
#define COM2_MAX_SIZE      64

PUBLIC uint8_t com2rx_buffer[COM2_MAX_SIZE];
PUBLIC uint8_t com2rx_frame[COM2_MAX_SIZE];
PUBLIC uint16_t com2rx_w_counter;
PUBLIC uint16_t com2rx_r_counter;
PUBLIC uint16_t com2rx_counter;

/* USER CODE BEGIN Includes */
void gd_usart_Init(void);
void gd_combus_cbfunc(void);
void gd_usart1_upload(uint8_t *strframe, uint16_t size);

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#undef PUBLIC
#endif /*__ USART_H__ */

