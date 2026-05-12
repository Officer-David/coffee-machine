
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
#define _HEAT_C_
#include <stdio.h>
#include <string.h>
#include "gd32f30x.h"
#include "user_apiopt.h"

/*_____ D E F I N I T I O N S ________________________________________________*/
#define DELT_VAL			10

/*_____ M A C R O S ______________________________________________________*/


/*_____ F U N C T I O N S __________________________________________________*/

void heat_task_nextstep(uint8_t Step)
{
	osal_lyh_restore_task(heat_id);
	heat_task_step = Step;
}

void heat_temp_deal(uint16_t target_val)
{
	uint16_t cur_temp_val = gd_adc_get(TEMPERATURE);
	int16_t temp_delt;

	temp_delt = cur_temp_val - target_val;
	if (temp_delt > DELT_VAL){
		ac_heat_stop();
	}
	else if (temp_delt < -DELT_VAL){
		ac_heat_start();
	}
}

uint8_t heat_get_temp(uint16_t target_val)
{
	uint16_t cur_temp_val = gd_adc_get(TEMPERATURE);
	int16_t temp_delt;

	temp_delt = cur_temp_val - target_val;
	if ((temp_delt < DELT_VAL) && (temp_delt > -DELT_VAL)){
		return TRUE;
	}
	return FAIL;
}

/************************************************************************
* function name: init_heat
* input: non
* output: non
* description: object thread
*************************************************************************/
void init_heat_task(uint8_t id)
{
	heat_id = id;
	heat_task_step = HEAT_INIT_STATE;
}

/************************************************************************
* function name: heat_task_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void heat_task_handler(void)
{
	switch(heat_task_step)
	{
		case HEAT_INIT_STATE:
			ac_heat_stop();
			heat_task_step = HEAT_IDLE_STATE;
			break;
		case HEAT_RUN_STATE:
			heat_temp_deal(target_temp);
			osal_lyh_delay_task(Ms_100);
			break;
		case HEAT_IDLE_STATE:
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: heat_processEvent
* input: non
* output: non
* description: this object's message loop
*************************************************************************/
uint32_t heat_event_handler( uint8_t task_id, uint32_t events)
{
	if (heat_id != task_id){
		return events;
	}
	if (HEAT_START_EVENT & events){
		if (heat_task_step != HEAT_RUN_STATE){
			heat_task_nextstep(HEAT_RUN_STATE);
		}
	}
	if (HEAT_STOP_EVENT & events){
		if (heat_task_step == HEAT_RUN_STATE){
			heat_task_nextstep(HEAT_INIT_STATE);
		}
	}
	return events;
}


