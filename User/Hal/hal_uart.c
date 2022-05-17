/********************************************************
* @file       hal_uart.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_uart.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
uart_isr_callback_t uart_rx_callback = NULL;

void Hal_Uart_Init(void )
{
    SystemCoreClock = 64000000;

    UART0_Init(SystemCoreClock, 115200);

    //INTC_EnableIRQ(ST1_IRQn);  
    INTC_DisableIRQ(ST0_IRQn);
    INTC_EnableIRQ(SR0_IRQn);       
}

void Hal_Uart_Register_Rx_Callback(uart_isr_callback_t callback )
{
    uart_rx_callback = callback;
}

void Hal_Uart_Rx_Isr_Handler(void )
{
    volatile uint8_t rx_data;

    rx_data = SCI0->RXD0;

    if(uart_rx_callback != NULL)
    {
        uart_rx_callback(rx_data);
    }
}

