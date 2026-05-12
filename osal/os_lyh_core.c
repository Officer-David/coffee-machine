

/**
 * \os_lyh_core.c
 */

#include <stdlib.h>
#include "gd32f30x.h"
#include "os_lyh_core.h"
#include "configboard.h"
#include "Systick.h" 

uint8_t thisTask;
uint16_t thisPt;
uint16_t tasksPendMask;

uint32_t PreTicks;
/******************************************************************************************************/
pTask tasksArr[MAX_TASK_NUMBER];
/******************************************************************************************************/
/* you must construct the array which it's member point to each event.*/

uint32_t eventMap[MAX_TASK_NUMBER];
uint32_t taskDelayCounter[MAX_TASK_NUMBER];
pTaskEventHandlerFn taskseventArr[MAX_TASK_NUMBER];

_overTimeStruct overTimeStruct[MAX_OVER_TIME_STRUCT_NUMBER];
/*
* time interrupt: interval time is 1ms
* function description: this timer is used for overtime function and delay task
* 
*/
void osal_SysTicks(void)
{
	uint8_t i = 0;
	uint32_t u32_ticks = systick_get();

	while ( PreTicks != u32_ticks ){

		for(i = 0; i < sizeof(taskDelayCounter)/sizeof(taskDelayCounter[0]); i++){
			
			if(taskDelayCounter[i] != 0){
				
				taskDelayCounter[i]--;
				if(taskDelayCounter[i] == 0){
					
					tasksPendMask |= 1 << i;
				}
			}
		}
		
		for(i = 0; i < sizeof(overTimeStruct)/sizeof(overTimeStruct[0]); i++){
			
			if(overTimeStruct[i].time != 0){
				
				overTimeStruct[i].time--;
				if(overTimeStruct[i].time == 0){
					
					overTimeStruct[i].idandf.timeover = 1;
				}
			}
		}
		PreTicks++;
	}
}

/*
* function name:main
* description: running task.
* 1):handle task.
* 2):running overtime function.
*/
int osal_start_system(void){
	uint8_t i;

	while(1)
	{
		osal_SysTicks();
		/* Refresh watchdog to prevent reset */
		fwdgt_counter_reload();
		if(eventMap[thisTask] != 0)
		{
			taskseventArr[thisTask](thisTask, eventMap[thisTask]);
			eventMap[thisTask] = 0;
		}
		if(tasksPendMask &(1 << thisTask))
		{
		    tasksArr[thisTask]();
		}
		for(i = 0; i < sizeof(overTimeStruct)/sizeof(overTimeStruct[0]); i++)
		{
			if(overTimeStruct[i].idandf.taskid != thisTask)
			    continue;
			if(overTimeStruct[i].idandf.timeover)
			{
				overTimeStruct[i].idandf.taskid = INVALID_ID;
				overTimeStruct[i].idandf.timeover = 0;
				if(overTimeStruct[i].func != NULL)
				    overTimeStruct[i].func();
			}
		}
		if ( ++thisTask == MAX_TASK_NUMBER ){
			thisTask= 0;
		}
	}
}

void osal_lyh_core_init(void){
	uint8_t i;

	thisTask = 0;
	tasksPendMask = 0;
	PreTicks = systick_get();
	for(i = 0; i < sizeof(taskDelayCounter)/sizeof(taskDelayCounter[0]); i++)
	{
		eventMap[i] = 0;
		taskDelayCounter[i] = 0;
	}

	for(i = 0; i < sizeof(overTimeStruct)/sizeof(overTimeStruct[0]); i++)
	{
		overTimeStruct[i].idandf.taskid = INVALID_ID;
		overTimeStruct[i].idandf.timeover = 0;
		overTimeStruct[i].event = INVALID_EVENT;
		overTimeStruct[i].time  = 0;
		overTimeStruct[i].func = NULL;
	}
	
	for(i = 0; i < MAX_TASK_NUMBER; i++)
	{	
		tasksArr[i] = ((pTask)0);
		taskseventArr[i] = ((pTaskEventHandlerFn)0);
	}
}

uint8_t osal_create_task(pInitTask init, pTask ta, pTaskEventHandlerFn ev){
	uint8_t i = 0;

	while(i < sizeof(tasksArr)/sizeof(tasksArr[0]))
	{
		if(tasksArr[i] == NULL)
			break;
		i++;
	}
	if(i == sizeof(tasksArr)/sizeof(tasksArr[0]))
	    return 0;
	// load task
	tasksArr[i] = ta;
	// load messge loop.
	taskseventArr[i] = ev;
	// initializing this task.
	init(i);
	// enable this task.
	tasksPendMask |= 1 << i;
	return 1;
}

void osal_lyh_suspend_task(uint8_t task){
	
	tasksPendMask &= ~(1 << task);  
}

void osal_lyh_restore_task(uint8_t task){
	
	tasksPendMask |= (1 << task);  
}

void osal_lyh_delay_task(uint32_t t){
	
	if(t == 0)
	    return;
	
	tasksPendMask &= ~(1 << thisTask);
	taskDelayCounter[thisTask] = t;
}

void osal_lyh_delay_over_timer(uint32_t event, uint32_t t, toOverTimeFunction func){
	
	uint8_t i;
	for(i = 0; i < sizeof(overTimeStruct)/sizeof(overTimeStruct[0]); i++){
		
		if(overTimeStruct[i].event == event){
			
			tasksPendMask &= ~(1 << thisTask);
			overTimeStruct[i].idandf.taskid = thisTask;
			overTimeStruct[i].func = func;
			overTimeStruct[i].event = event;
			overTimeStruct[i].time = t;
			overTimeStruct[i].idandf.timeover = 0;
			return;
		}
	}
	
	for(i = 0; i < sizeof(overTimeStruct)/sizeof(overTimeStruct[0]); i++){
		
		if(overTimeStruct[i].event == INVALID_EVENT){
			
			tasksPendMask &= ~(1 << thisTask);
			overTimeStruct[i].idandf.taskid = thisTask;
			overTimeStruct[i].func = func;
			overTimeStruct[i].event = event;
			overTimeStruct[i].time = t;
			overTimeStruct[i].idandf.timeover = 0;
			return;
		}
	}
}

void osal_lyh_over_timer(uint32_t event, uint32_t t, toOverTimeFunction func){
	
	uint8_t i;
	for(i = 0; i < sizeof(overTimeStruct)/sizeof(overTimeStruct[0]); i++){
		
		if(overTimeStruct[i].event == event){
			
			overTimeStruct[i].idandf.taskid = thisTask;
			overTimeStruct[i].func = func;
			overTimeStruct[i].event = event;
			overTimeStruct[i].time = t;
			overTimeStruct[i].idandf.timeover = 0;
			return;
		}
	}
	
	for(i = 0; i < sizeof(overTimeStruct)/sizeof(overTimeStruct[0]); i++){
		
		if(overTimeStruct[i].event == INVALID_EVENT){
			
			overTimeStruct[i].idandf.taskid = thisTask;
			overTimeStruct[i].func = func;
			overTimeStruct[i].event = event;
			overTimeStruct[i].time = t;
			overTimeStruct[i].idandf.timeover = 0;
			return;
		}
	}
}

void osal_lyh_kill_over_timer(uint32_t event){
	
	uint8_t i;
	for(i = 0; i < sizeof(overTimeStruct)/sizeof(overTimeStruct[0]); i++){
		
		if(overTimeStruct[i].event == event){
			
			overTimeStruct[i].idandf.taskid = INVALID_ID;
			overTimeStruct[i].func = 0;
			overTimeStruct[i].event = INVALID_EVENT;			
			overTimeStruct[i].time = 0;
			overTimeStruct[i].idandf.timeover = 0;
			return;
		}
	}
}

uint8_t  osal_lyh_get_event(void){
	
	return eventMap[thisTask];
}


