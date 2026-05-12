/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#define _USART_C_
#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"
#include "keyboard.h"
#include "display.h"
#include "usart.h"

#define ARRAYNUM(arr_name)     (uint32_t)(sizeof(arr_name)/sizeof(*(arr_name)))
#define USART0_DATA_ADDRESS(x)    ((uint32_t)(x + 0x00000004U)/*0x40013804*/)

#define HEAD_ID			0xFFAA

/* USER CODE BEGIN 0 */
uint8_t com1tx_buffer[COM1_MAX_SIZE];
uint8_t com1rx_buffer[COM1_MAX_SIZE];
uint8_t com1rx_frame[COM1_MAX_SIZE];
uint16_t com1rx_counter;

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure display                                                            */
/*----------------------------------------------------------------------------*/
void gd_uart_counter_init(void)
{
    com1rx_counter = ARRAYNUM(com1rx_buffer);
    com2rx_counter = 0x0000;
    com2rx_w_counter = 0x0000;
    com2rx_r_counter = 0x0000;	
}

/* USER CODE BEGIN 1 */
/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        COM1: COM1 on the board
      \arg        COM2: COM2 on the board
    \param[out] none
    \retval     none
*/
void gd_uart_com_init(uint32_t com)
{
    rcu_periph_enum COM_CLK[COMn] = {COM1_CLK, COM2_CLK};
    rcu_periph_enum COM_GPIO_CLK[COMn] = {COM1_GPIO_CLK, COM2_GPIO_CLK};
    uint32_t COM_GPIO_PORT[COMn] = {COM1_GPIO_PORT, COM2_GPIO_PORT};
    uint32_t COM_TX_PIN[COMn] = {COM1_TX_PIN, COM2_TX_PIN};
    uint32_t COM_RX_PIN[COMn] = {COM1_RX_PIN, COM2_RX_PIN};
    uint32_t com_id = 0U;
    if(COM1 == com){
        com_id = 0U;
    }else if(COM2 == com){
        com_id = 1U;
    }
    
    /* enable GPIO clock */
    rcu_periph_clock_enable(COM_GPIO_CLK[com_id]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[com_id]);

    /* connect port to USARTx_Tx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, COM_TX_PIN[com_id]);

    /* connect port to USARTx_Rx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, COM_RX_PIN[com_id]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
}

/*!
    \brief      configure USART DMA
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gd_usart0_dma_config(void)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA0 */
    rcu_periph_clock_enable(RCU_DMA0);
    /* deinitialize DMA channel3(USART0 tx) */
    dma_deinit(DMA0, DMA_CH3);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)com1tx_buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number =  0x00U;
    dma_init_struct.periph_addr = USART0_DATA_ADDRESS(USART0);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH3, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH3);
    dma_memory_to_memory_disable(DMA0, DMA_CH3);

    dma_deinit(DMA0, DMA_CH4);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.number =  ARRAYNUM(com1rx_buffer);
    dma_init_struct.memory_addr = (uint32_t)com1rx_buffer;
    dma_init(DMA0, DMA_CH4, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_enable(DMA0, DMA_CH4);
    dma_memory_to_memory_disable(DMA0, DMA_CH4);

    /* enable USART0 DMA channel transmission and reception */
    dma_channel_disable(DMA0, DMA_CH3);
    dma_channel_enable(DMA0, DMA_CH4);

    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
    usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
}

void gd_usart_Init(void)
{
    gd_uart_com_init(COM1);
    gd_uart_com_init(COM2);
    /* USART0 DMA configure */
    gd_usart0_dma_config();

    /* USART1 interrupt configuration */
    nvic_irq_enable(USART1_IRQn, 0, 0);
    /* enable USART1 receive interrupt */
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    /* enable USART1 transmit interrupt */
    //usart_interrupt_enable(USART1, USART_INT_TBE);
    gd_uart_counter_init();	
}

void gd_usart1_upload(uint8_t *strframe, uint16_t size)
{
	uint16_t i;
	
	if (!strframe || !size) {
		return;
	}
 	for (i=0; i<size; i++) {
        /* write one byte to the transmit data register */
        usart_data_transmit(USART1, strframe[i]);
		/* wait until USART1 transmit end */
        while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
	}
}

ErrStatus gd_usart1_rx_decode(uint8_t ch) 
{
	if (com2rx_counter < COM2_MAX_SIZE) {
		com2rx_frame[com2rx_counter++] = ch;
		if (ch == '\n') {
			com2rx_counter = 0;
			return SUCCESS;
		}
	}
	return ERROR;
}

void gd_usart1_download(void)
{
	uint16_t com2_wcnt = com2rx_w_counter;
	while (com2rx_r_counter != com2_wcnt) {
		uint8_t ch = com2rx_buffer[com2rx_r_counter];
		if (++com2rx_r_counter >= COM2_MAX_SIZE) {
			com2rx_r_counter = 0x0000;
		}
		if (SUCCESS ==gd_usart1_rx_decode(ch)){
		}
	}
}

/*!
    \brief      memory decode function
    \param[in]  ch: source data
    \param[out] frame: package data
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus gd_usart0_rx_decode(uint8_t ch) 
{
	static uint8_t chksum, len, size, stateFlow = 0;

	chksum += ch;
	switch (stateFlow){
	case 0:
		if (ch == HEAD_ID / 0X100){
			stateFlow++;
		}
		break;
	case 1:
		stateFlow++;
		if (ch != HEAD_ID % 0X100){
			stateFlow = 0x00;
		}
		break;
	case 2:
		len = ch;
		chksum =  ch;
		size = 0x00;
		stateFlow++;
		break;
	case 3:
		if (size < COM1_MAX_SIZE) {
			com1rx_frame[size]  = ch;
		}
		if (++size == len) {
			stateFlow = 0;
			if (!chksum){
			    return SUCCESS;
			}
		}
		break;
	default:
		break;
	}
    return ERROR;
}

void gd_usart0_upload(void)
{
	uint8_t j, idx = 0, chksum = 0;

	/* check USART0 TX DMA0 channel transfer complete */
	if (dma_transfer_number_get(DMA0, DMA_CH3)){
		return;
	}
	com1tx_buffer[idx++] = HEAD_ID / 0x100;
	com1tx_buffer[idx++] = HEAD_ID % 0x100;
	com1tx_buffer[idx++] = 0x00;
	com1tx_buffer[idx++] = (virtul_led >> 24)  &  0xFF;
	com1tx_buffer[idx++] = (virtul_led >> 16)  &  0xFF;
	com1tx_buffer[idx++] = (virtul_led >> 8)  &  0xFF;
	com1tx_buffer[idx++] = (virtul_led >> 0)  &  0xFF;
	com1tx_buffer[2] = idx - 2;		// header(2)
	for (j=2; j<idx; j++) {
		chksum += com1tx_buffer[j]; 
	}
	com1tx_buffer[idx++] = 0 - chksum;
	dma_channel_disable(DMA0, DMA_CH3);	
	dma_flag_clear(DMA0, DMA_CH3, DMA_INTC_FTFIFC | DMA_INTC_HTFIFC | DMA_INTC_ERRIFC);
	dma_transfer_number_config(DMA0, DMA_CH3, idx);
	dma_channel_enable(DMA0, DMA_CH3);
}

void gd_usart0_download(void)
{
	uint8_t ch;
	uint16_t rx_index = dma_transfer_number_get(DMA0, DMA_CH4);
	while (rx_index != com1rx_counter){
		ch = com1rx_buffer[COM1_MAX_SIZE - com1rx_counter];
		if (--com1rx_counter == 0x0000){
			com1rx_counter = COM1_MAX_SIZE;
		}
		if (SUCCESS == gd_usart0_rx_decode(ch)){
			keyTouch_in = (com1rx_frame[0]<<8) | com1rx_frame[1];
		}
	}
	if (dma_flag_get(DMA0, DMA_CH4,DMA_INTF_FTFIF)) {
		dma_flag_clear(DMA0, DMA_CH4, DMA_INTC_FTFIFC | DMA_INTC_HTFIFC | DMA_INTC_ERRIFC);
	}
}

/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
void gd_combus_cbfunc(void)
{
	gd_usart0_upload();
	gd_usart0_download();
	gd_usart1_download();
}

/* USER CODE END 2 */
