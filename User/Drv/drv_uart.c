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

void Drv_Uart_Init(void )
{
    Hal_Uart_Init();

    Hal_Uart_Register_Rx_Callback(Drv_Uart_Rx_Handler);
}

static void Drv_Uart_Rx_Handler(uint8_t rxData )
{
    static uint32_t addr;
    
   	Drv_Spi_Write_Page(addr, (uint8_t *)&rxData, 1);

    addr++;
}

