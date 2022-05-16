#ifndef _HAL_LED_H
#define _HAL_LED_H

#include "hardware.h"

#define LED_OFF                0
#define LED_ON                 (!LED_OFF)

void Hal_Led_Init(void );
void Hal_Led_Set_Off(uint8_t port, uint8_t pin );
void Hal_Led_Set_On(uint8_t port, uint8_t pin );
uint8_t Hal_Led_Get_State(uint8_t port, uint8_t pin );

#endif 

