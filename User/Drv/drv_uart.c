/********************************************************
* @file       drv_uart.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hardware.h"
#include "drv_timer.h"
#include "drv_flash.h"
#include "drv_uart.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Uart_Rx_Handler(uint8_t rxData );
/* Private variables ------------------------------------*/
void uart_test(void *arg )
{
    printf("hello world\n");
}

void Drv_Uart_Init(void )
{
    Hal_Uart_Init();

    Hal_Uart_Register_Rx_Callback(Drv_Uart_Rx_Handler);

    Drv_Timer_Register_Period(uart_test, 0, 1000, NULL);
}

static void Drv_Uart_Rx_Handler(uint8_t rxData )
{
    static uint32_t addr;
    
    //Drv_Spi_Write_Page(addr, (uint8_t *)&rxData, 1);

    addr++;
}

