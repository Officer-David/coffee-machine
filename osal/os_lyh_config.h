/*
 * os_lyh_config.h
 *
 * Created: 2014-3-31 14:06:24
 *  Author: liuyh
 */ 


#ifndef OS_LYH_CONFIG_H_
#define OS_LYH_CONFIG_H_


#define MAX_TASK_NUMBER               4
#define MAX_OVER_TIME_STRUCT_NUMBER   16

#define disable_system_irq()	__disable_irq() 
#define enable_system_irq()	__enable_irq() 

#endif /* OS_LYH_CONFIG_H_ */

