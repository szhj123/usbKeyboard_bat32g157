#ifndef _DRV_TIMER_H
#define _DRV_TIMER_H

#include "hardware.h"

#define TIMER_MAX_NUM                       16

#define TIMER_NULL                          TIMER_MAX_NUM

typedef struct _timer_ctrl_block_t
{
    void (*handler)(void *);
    void *arg;
    uint16_t ticks;
    uint16_t period;
    uint8_t isRegistFlag;
}timer_ctrl_block_t;

void Drv_Timer_Init(void );
uint8_t Drv_Timer_Register_Oneshort(void (*handler)(void *), uint16_t ticks, void *arg );
uint8_t Drv_Timer_Register_Period(void (*handler)(void *), uint16_t ticks, uint16_t period, void *arg );
void Drv_Timer_Delete(uint8_t id );
void Drv_Timer_Isr_Handler(void );



#endif 

