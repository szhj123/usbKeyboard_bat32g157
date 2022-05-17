/********************************************************
* @file       hardware.c
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

#include "drv_uart.h"
#include "drv_flash.h"
#include "app_lcd.h"
#include "app_led.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

int main (void)
{
    Clk_Init();
    
    Drv_Timer_Init();

    Drv_Uart_Init();

    Drv_Spi_Flash_Init();

    App_Lcd_Init();

    App_Led_Init();

    Usb_Init(); 

    while(1)
    {
        App_Lcd_Handler();
    }
} 


