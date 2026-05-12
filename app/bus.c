
/*
  ******************************************************************************
  * @file           : bus.c
  * @brief         : keyboard program body
  ******************************************************************************
  * @attention
  *
  * Date					 		
  * Description
  *____________________________________________________________________________*/

/*_____ I N C L U D E S ____________________________________________________*/
#define _BUS_C_
#include <stdio.h>
#include <string.h>
#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"
#include "display.h"
#include "usart.h"
#include "adc.h"
#include "bus.h"

#define DBG_PRINT			1
#define DBG_PERIOD(x)		(1000/x)

uint8_t bus_dbg_period;

/*_____ D E F I N I T I O N S ________________________________________________*/


/*_____ M A C R O S ______________________________________________________*/


/*_____ F U N C T I O N S __________________________________________________*/
void gd_addbg_out(void)
{
#ifdef DBG_PRINT
	if (++bus_dbg_period > DBG_PERIOD(Ms_10)) {
		uint16_t temp_val = gd_adc_get(TEMPERATURE);
		uint16_t motor_val = gd_adc_get(MOTOR_ISEN);
		uint16_t voltage_val = gd_adc_get(AC_VOLTAGE);
		printf("T:%d, M:%d, V:%d\r\n", temp_val, motor_val, voltage_val);
		bus_dbg_period = 0;
	}
#endif	
}

/************************************************************************
* function name: init_bus
* input: non
* output: non
* description: object thread
*************************************************************************/
void init_bus_task(uint8_t id)
{
	bus_id = id;
	bus_task_step = BUS_INIT_STATE;
}

/************************************************************************
* function name: bus_task_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void bus_task_handler(void)
{
	switch(bus_task_step)
	{
		case BUS_INIT_STATE:
			display_init();
			bus_dbg_period = 0;
			bus_task_step = BUS_RUN_STATE;
			break;
		case BUS_RUN_STATE:
			gd_addbg_out();
			gd_combus_cbfunc();
			osal_lyh_delay_task(Ms_10);
			break;
		case BUS_IDLE_STATE:
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: bus_processEvent
* input: non
* output: non
* description: this object's message loop
*************************************************************************/
uint32_t bus_event_handler( uint8_t task_id, uint32_t events)
{
	if (bus_id != task_id){
		return events;
	}
	return events;
}


