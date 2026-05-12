
/*
  ******************************************************************************
  * @file           : keyboard.c
  * @brief         : keyboard program body
  ******************************************************************************
  * @attention
  *
  * Date					 		
  * Description
  *____________________________________________________________________________*/

/*_____ I N C L U D E S ____________________________________________________*/
#define _KEYBOARD_C_
#include <string.h>
#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"
#include "keyboard.h"
#include "app.h"

/*_____ D E F I N I T I O N S ________________________________________________*/


/*_____ M A C R O S ______________________________________________________*/


/*_____ F U N C T I O N S __________________________________________________*/
void ini_keys_value(void)
{
	memset((uint8_t*)&keyinfo, 0, sizeof(_keystruct));
	keyinfo.ulUIOnboardSwitches = 0xFFFFFFFF;
#ifdef KEY_HOLD_FUNCTION_ENABLE
	keyinfo.ulHoldTime[KEY_POS_1] = Ms_600/Ms_10;	  // interval time: 1s	
	keyinfo.ulHoldTime[KEY_POS_2] = Ms_1000/Ms_10;	  // interval time: 1s	
	keyinfo.ulHoldTime[KEY_POS_3] = Ms_1000/Ms_10;	  // interval time: 1s	
	keyinfo.ulHoldTime[KEY_POS_4] = Ms_1000/Ms_10;	  // interval time: 1s	
	keyinfo.ulHoldTime[KEY_POS_5] = Ms_1000/Ms_10;	  // interval time: 1s	
	keyinfo.ulHoldTime[KEY_POS_6] = Ms_1000/Ms_10;	  // interval time: 1s	
	keyinfo.ulHoldTime[KEY_POS_7] = Ms_1000/Ms_10;	  // interval time: 1s	
	keyinfo.ulHoldTime[KEY_POS_8] = Ms_1000/Ms_10;	  // interval time: 1s	
	keyinfo.ulHoldTime[KEY_POS_9] = Ms_3000/Ms_10;	  // interval time: 3s	
	keyinfo.ulHoldTime[KEY_POS_10] = Ms_3000/Ms_10;	  // interval time: 3s	
	keyinfo.ulHoldTime[KEY_POS_11] = Ms_1000/Ms_10;	  // interval time: 1s	
	keyinfo.pulUIHoldCount[KEY_POS_1] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_2] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_3] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_4] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_5] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_6] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_7] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_8] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_9] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_10] = 0xFFFF;
	keyinfo.pulUIHoldCount[KEY_POS_11] = 0xFFFF;
#endif
	keyTouch_in = 0u;
}

/****************************************************************************************************
* function name: UIOnboardSwitchDebouncer
* input: non
* output: non
* description: scan all key
****************************************************************************************************/
void key_switch_debouncer(uint32_t ulSwitches)
{
	uint32_t ulDelta, ulIdx;
	
	keyinfo.ulPressF = 0u;
	keyinfo.ulLPressF = 0u;
	keyinfo.ulReleaseF = 0u;
	/* Determine the switches that are at a different state than the debounced state*/
	ulDelta = ulSwitches ^ keyinfo.ulUIOnboardSwitches;

	/* Increment the clocks by one.*/
	keyinfo.ulUIOnboardClockA ^= keyinfo.ulUIOnboardClockB;
	keyinfo.ulUIOnboardClockB = ~keyinfo.ulUIOnboardClockB;

	/* Reset the clocks corresponding to switches that have not changed state.*/
	keyinfo.ulUIOnboardClockA &= ulDelta;
	keyinfo.ulUIOnboardClockB &= ulDelta;

	/* Get the new debounced switch state.*/
	keyinfo.ulUIOnboardSwitches &= (keyinfo.ulUIOnboardClockA |	keyinfo.ulUIOnboardClockB);
	keyinfo.ulUIOnboardSwitches |= ((~(keyinfo.ulUIOnboardClockA |keyinfo.ulUIOnboardClockB)) & ulSwitches);

	/* Determine the switches that just changed debounced state.*/
	ulDelta ^= (keyinfo.ulUIOnboardClockA | keyinfo.ulUIOnboardClockB);

	/* Loop through all of the switches.*/
	for(ulIdx = 0; ulIdx < MAX_KEY_NUMBER; ulIdx++)
	{
		/* See if this switch just changed state.*/
		if(ulDelta & (1 << ulIdx))
		{
			/* See if the switch was just pressed or released. */
			if(!(keyinfo.ulUIOnboardSwitches & (1u << ulIdx)))
			{
			#ifdef KEY_HOLD_FUNCTION_ENABLE
				keyinfo.pulUIHoldCount[ulIdx] = 0;	//first_time[ulIdx]; 
				if ( !keyinfo.ulHoldTime[ulIdx] ) {
					keyinfo.ulPressF |= (1 << ulIdx);
				}
			#else
				keyinfo.ulPressF |= (1 << ulIdx);
			#endif
			}
			else
			{
				keyinfo.ulReleaseF |= (1 << ulIdx);
			}
		}
		/* See if there is a hold defined for this switch and the switch is pressed.*/
	#ifdef KEY_HOLD_FUNCTION_ENABLE
		if (keyinfo.ulHoldTime[ulIdx] != 0)
		{
			if (keyinfo.ulUIOnboardSwitches & (1 << ulIdx)){
				if (keyinfo.pulUIHoldCount[ulIdx] < keyinfo.ulHoldTime[ulIdx])
				{
					keyinfo.pulUIHoldCount[ulIdx] = 0xFFFF;
					keyinfo.ulPressF |= 1 << ulIdx;
				}
			}
			else {
				/* Increment the hold count if it is not maxed out.*/
				if (keyinfo.pulUIHoldCount[ulIdx] < 0xFFFF)
				{
					keyinfo.pulUIHoldCount[ulIdx]++;
				}
				if (keyinfo.pulUIHoldCount[ulIdx] == keyinfo.ulHoldTime[ulIdx])
				{
					keyinfo.pulUIHoldCount[ulIdx] = 0xFFFF;
					keyinfo.ulLPressMaskF |= 1 << ulIdx;
					keyinfo.ulLPressF |= 1 << ulIdx;
				}
			}
		}
	#endif
	}

	key_prohibit_mask();
	if (keyinfo.ulPressF || keyinfo.ulReleaseF || keyinfo.ulLPressF)
	{
		osal_lyh_set_event(app_id, APP_KEY_CHANGE_EVENT);
	}
}

void key_scan_process(void)
{
	uint32_t ulSwitches = 0xFFFFFFFF;

	if (TOUCH_START_KEY()){
		ulSwitches &= ~KEY_START_MASK; 
	}
	if (TOUCH_ONOFF_KEY()){
		ulSwitches &= ~KEY_ONOFF_MASK; 
	}
	if (TOUCH_HOTWATER_KEY()){
		ulSwitches &= ~KEY_HOTWATER_MASK; 
	}
	if (TOUCH_ESPRESSO_KEY()){
		ulSwitches &= ~KEY_ESPRESSO_MASK; 
	}
	if (TOUCH_STEAM_KEY()){
		ulSwitches &= ~KEY_STEAM_MASK; 
	}
	if (TOUCH_AMERICANO_KEY()){
		ulSwitches &= ~KEY_AMERICANO_MASK; 
	}
	if (TOUCH_AQUA_KEY()){
		ulSwitches &= ~KEY_AQUA_MASK; 
	}
	if (TOUCH_COFFE_KEY()){
		ulSwitches &= ~KEY_COFFE_MASK; 
	}
	if (TOUCH_CALC_KEY()){
		ulSwitches &= ~KEY_CALC_MASK; 
	}
	if (TOUCH_WATER_KEY()){
		ulSwitches &= ~KEY_WATER_MASK; 
	}
	if (TOUCH_PAD_KEY()){
		ulSwitches &= ~KEY_PAD_MASK; 
	}
	// input signal detect
	if (!PALLET_DET_STATE()){
		ulSwitches &= ~PALLET_DET_MASK; 
	}
	if (P3V_DET_STATE()){
		ulSwitches &= ~PW3P3V_DET_MASK; 
	}
	if (!WKUP_STATE()){
		ulSwitches &= ~WKUP_DET_MASK; 
	}
	if (!METER_DET_STATE()){
		ulSwitches &= ~METER_DET_MASK; 
	}
	if (!MOTOR_DET_STATE()){
		ulSwitches &= ~MOTOR_DET_MASK; 
	}
	if (WATER_DET_STATE()){
		ulSwitches &= ~WATER_DET_MASK; 
	}

	key_switch_debouncer(ulSwitches);
}

/************************************************************************
* function name: init_key
* input: non
* output: non
* description: object thread
*************************************************************************/
void init_key_task(uint8_t id)
{
	keyboard_id = id;
	keyboard_task_step = KEY_INIT_STATE;
}

/************************************************************************
* function name: key_task_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void key_task_handler(void)
{
	switch(keyboard_task_step)
	{
		case KEY_INIT_STATE:
			ini_keys_value();
			osal_lyh_delay_task(Ms_50);
			keyboard_task_step = KEY_RUN_STATE;
			break;
		case KEY_RUN_STATE:
			key_scan_process();
			osal_lyh_delay_task(Ms_10);
			break;
		case KEY_IDLE_STATE:
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: key_processEvent
* input: non
* output: non
* description: this object's message loop
*************************************************************************/
uint32_t key_event_handler( uint8_t task_id, uint32_t events)
{
	if (keyboard_id != task_id){
		return events;
	}

	return events;
}

void key_prohibit_mask(void)
{
	uint32_t keyMask = ~keyinfo.ulProhibitkF;
	
	keyinfo.ulPressF &= keyMask;
	keyinfo.ulLPressF &= keyMask;
	keyinfo.ulReleaseF &= keyMask;
}

void key_prohibit_setup(uint32_t key, uint8_t status)
{
	keyinfo.ulProhibitkF &= ~key;
	keyinfo.ulProhibitkF |= (status==TRUE)? key:0u;
	key_prohibit_mask();
}

