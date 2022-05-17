#ifndef _HAL_UART_H
#define _HAL_UART_H

#include "hardware.h"

typedef void (*uart_isr_callback_t)(uint8_t );

void Hal_Uart_Init(void );
void Hal_Uart_Register_Rx_Callback(uart_isr_callback_t callback );
void Hal_Uart_Rx_Isr_Handler(void );

#endif 

