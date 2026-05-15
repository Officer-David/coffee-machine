#ifndef __RELAY_H_
#define __RELAY_H_

#ifdef _RELAY_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif


enum _RELAY_MODE{
    COFFEE_MODE = 0,
    WATER_MODE,
};


void Relay2_ON(void);            //继电器打开
void Relay2_OFF(void);           //继电器关闭

void gd_realy_init(void);


#undef PUBLIC
#endif