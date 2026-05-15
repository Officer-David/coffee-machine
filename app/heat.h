
#ifndef __HEAT_H_
#define __HEAT_H_

#ifdef _HEAT_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

#ifndef TRUE
#define TRUE				1
#endif
#ifndef FAIL
#define FAIL				0
#endif

#define REFVAL			(3.3)

// heating detect.
#define TARGET_TEMP_107D	(uint16_t)(1.80 / (REFVAL / (float)4096))	// 107 degress
#define TARGET_TEMP_100D	(uint16_t)(1.70 / (REFVAL / (float)4096))	// ?
#define TARGET_TEMP_90D		(uint16_t)(1.38 / (REFVAL / (float)4096))  // 90 degress

#define TARGET_TEMP_STARTUP		(uint16_t)(2.4 / (REFVAL / (float)4096))  
#define TARGET_TEMP_COFFEE		(uint16_t)(2.24 / (REFVAL / (float)4096)) 
#define TARGET_TEMP_HOTWATER	(uint16_t)(2.08 / (REFVAL / (float)4096))  
#define TARGET_TEMP_STEAM		(uint16_t)(2.64 / (REFVAL / (float)4096))  


// motor detect.
#define MOTOR_DETECT_1P80	(uint16_t)(1.80 / (REFVAL / (float)4096))	
#define MOTOR_DETECT_1P70	(uint16_t)(1.70 / (REFVAL / (float)4096))	
#define MOTOR_DETECT_1P38	(uint16_t)(1.38 / (REFVAL / (float)4096))  

// acVolt detect.
#define ACVOLT_DETECT_1P80	(uint16_t)(1.80 / (REFVAL / (float)4096))	
#define ACVOLT_DETECT_1P70	(uint16_t)(1.70 / (REFVAL / (float)4096))	
#define ACVOLT_DETECT_1P38	(uint16_t)(1.38 / (REFVAL / (float)4096))  


//total event: 32
#define HEAT_START_EVENT		((uint32_t)(1 << 0))	// 
#define HEAT_STOP_EVENT			((uint32_t)(1 << 1))	// 
#define HEAT_EVENT_INFO_2		((uint32_t)(1 << 2))	// 
#define HEAT_EVENT_INFO_3		((uint32_t)(1 << 3))	// 
#define HEAT_EVENT_INFO_4		((uint32_t)(1 << 4))	// 
#define HEAT_EVENT_INFO_5		((uint32_t)(1 << 5))	// 
#define HEAT_EVENT_INFO_6		((uint32_t)(1 << 6))	// 	
#define HEAT_EVENT_INFO_7		((uint32_t)(1 << 7))	//
#define HEAT_EVENT_INFO_8		((uint32_t)(1 << 8))	//
#define HEAT_EVENT_INFO_9		((uint32_t)(1 << 9))	// 

#define HEAT_INIT_STATE		0
#define HEAT_IDLE_STATE		1
#define HEAT_RUN_STATE		2

/****************************************************************************************************
 * message data struct
****************************************************************************************************/
PUBLIC uint8_t heat_id;	
PUBLIC uint8_t heat_task_step;

PUBLIC uint16_t target_temp;	

void init_heat_task(uint8_t id);
void heat_task_handler(void);
uint32_t heat_event_handler(uint8_t task_id, uint32_t events);
uint8_t heat_get_temp(uint16_t target_val);

#undef PUBLIC
#endif


