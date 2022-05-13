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
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private variables ------------------------------------*/

int main (void)
{
    Clk_Init();
    
    Drv_Timer_Init();

    Usb_Init(); 

    while(1)
    {
    }
} 


