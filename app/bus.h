
#ifndef __BUS_H_
#define __BUS_H_

#ifdef _BUS_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

#define BUS_INIT_STATE		0
#define BUS_IDLE_STATE		1
#define BUS_RUN_STATE		2

/****************************************************************************************************
 * message data struct
****************************************************************************************************/
PUBLIC uint8_t bus_id;	
PUBLIC uint8_t bus_task_step;

void init_bus_task(uint8_t id);
void bus_task_handler(void);
uint32_t bus_event_handler( uint8_t task_id, uint32_t events);

#undef PUBLIC
#endif


