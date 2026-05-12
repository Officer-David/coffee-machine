
/**
 * \os_lyh_core.h
 */

#ifndef __OS_LYH_CORE_H
#define __OS_LYH_CORE_H

#include "os_lyh_config.h"
#include "os_msg.h"

// task protocol define
typedef void (*pInitTask)( uint8_t id);
// task protocol define
typedef void (*pTask)( void);
// event protocol define
typedef uint32_t (*pTaskEventHandlerFn)(uint8_t task_id, uint32_t events);

typedef void (*toOverTimeFunction)(void);

#define INVALID_ID    0x0F
#define INVALID_EVENT 0xFFFFFFFF

typedef struct __idandf{
	uint8_t taskid:4;
	uint8_t timeover:1;
}_idandf;
typedef struct __overTimeStruct{	
	_idandf idandf;
	uint32_t event;
	uint32_t time;
	toOverTimeFunction func;
}_overTimeStruct;

extern uint16_t thisPt;
extern uint8_t thisTask;
extern uint16_t tasksPendMask;
extern uint32_t eventMap[MAX_TASK_NUMBER];
extern uint32_t taskDelayCounter[MAX_TASK_NUMBER];
extern _overTimeStruct overTimeStruct[MAX_OVER_TIME_STRUCT_NUMBER];

#define  osal_lyh_set_this_pointer(x) thisTask = x
#define  osal_lyh_get_this_pointer() thisTask

#define osal_lyh_set_event(x, y) eventMap[x] |= y
#define osal_lyh_clr_event(x, y) eventMap[x] &= ~(y)

uint8_t osal_create_task(pInitTask init, pTask ta, pTaskEventHandlerFn ev);
uint8_t osal_lyh_get_event(void);

void osal_lyh_suspend_task(uint8_t task);
void osal_lyh_restore_task(uint8_t task);
void osal_lyh_delay_task(uint32_t t);
void osal_lyh_over_timer(uint32_t event, uint32_t t, toOverTimeFunction func);
void osal_lyh_delay_over_timer(uint32_t event, uint32_t t, toOverTimeFunction func);
void osal_lyh_kill_over_timer(uint32_t event);
void osal_lyh_core_init(void);
int osal_start_system(void);
#endif






