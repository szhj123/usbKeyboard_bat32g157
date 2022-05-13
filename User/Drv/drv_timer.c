/********************************************************
* @file       drv_timer.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private variables ------------------------------------*/
timer_ctrl_block_t timerList[TIMER_MAX_NUM];

void Drv_Timer_Init(void )
{
    Tim4_Channel0_Init();
}

static uint8_t Drv_Timer_Register(void (*handler)(void *), uint16_t ticks, uint16_t period, void *arg )
{
    uint8_t id;

    for(id=0;id<TIMER_MAX_NUM;id++)
    {
        if(!timerList[id].isRegistFlag)
        {
            timerList[id].handler = handler;
            timerList[id].arg = arg;
            timerList[id].ticks = 0;
            timerList[id].period = period;

            timerList[id].isRegistFlag = 1;

            break;
        }
    }

    return id;
}

uint8_t Drv_Timer_Register_Oneshort(void (*handler)(void *), uint16_t ticks, void *arg )
{
    return Drv_Timer_Register(handler, ticks, 0, arg);
}

uint8_t Drv_Timer_Register_Period(void (*handler)(void *), uint16_t ticks, uint16_t period, void *arg )
{
    return Drv_Timer_Register(handler, ticks, period, arg);
}

void Drv_Timer_Delete(uint8_t id )
{
    if(id >= TIMER_MAX_NUM || timerList[id].isRegistFlag == 0)
    {
        return ;
    }

    timerList[id].handler = NULL;
    timerList[id].arg = NULL;
    timerList[id].ticks = 0;
    timerList[id].period = 0;
    timerList[id].isRegistFlag = 0;
}

void Drv_Timer_Isr_Handler(void )
{
    uint8_t id;

    for(id=0;id<TIMER_MAX_NUM;id++)
    {
        if(timerList[id].isRegistFlag)
        {
            if(timerList[id].ticks > 0)
            {
                timerList[id].ticks--;
            }

            if(timerList[id].ticks == 0)
            {
                if(timerList[id].handler != NULL)
                {
                    timerList[id].handler(timerList[id].arg);
                }

                if(timerList[id].period)
                {
                    timerList[id].ticks = timerList[id].period;
                }
                else
                {
                    Drv_Timer_Delete(id);
                }
            }
        }
    }
}

