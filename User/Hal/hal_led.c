/********************************************************
* @file       hal_led.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_led.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Led_Init(void )
{
    Gpio_Led_Init();   
}

void Hal_Led_Set_Off(uint8_t port, uint8_t pin )
{
    PORT_SetBit((PORT_TypeDef)port, (PIN_TypeDef)pin);
}

void Hal_Led_Set_On(uint8_t port, uint8_t pin )
{
    PORT_ClrBit((PORT_TypeDef)port, (PIN_TypeDef)pin);
}

uint8_t Hal_Led_Get_State(uint8_t port, uint8_t pin )
{
    if(PORT_GetBit((PORT_TypeDef )port, (PIN_TypeDef )pin ))
    {
        return LED_OFF;
    }
    else
    {
        return LED_ON;
    }
}


