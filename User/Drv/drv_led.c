/********************************************************
* @file       drv_led.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"
#include "drv_led.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Drv_Led_Init(void )
{
    Hal_Led_Init();
}

void Drv_Led_Flash(led_ctrl_block_t *led )
{
    switch(led->ledFlash)
    {
        case LED_FLASH_SOLID_OFF:
        {
            Hal_Led_Set_Off((uint8_t )led->gpioPort, (uint16_t )led->gpioPin);
            break;
        }
        case LED_FLASH_SOLID_ON:
        {
            Hal_Led_Set_On((uint8_t )led->gpioPort, (uint16_t )led->gpioPin);
            break;
        }
        case LED_FLASH_SLOW:
        case LED_FLASH_REGULAR:
        case LED_FLASH_QUICK:
        {
            if(Hal_Led_Get_State((uint8_t )led->gpioPort, (uint8_t )led->gpioPin) == LED_ON )
            {
                if(led->ledDelayCnt > led->ledOnTime)
                {
                    Hal_Led_Set_Off((uint8_t )led->gpioPort, (uint16_t )led->gpioPin);
    
                    led->ledDelayCnt = 0;
                }
            }
            else
            {
                if(led->ledDelayCnt > led->ledOffTime)
                {
                    Hal_Led_Set_On((uint8_t )led->gpioPort, (uint16_t )led->gpioPin);
    
                    led->ledDelayCnt = 0;
                }
            }
            break;
        }
        default: break;
    }
}

void Drv_Led_Set_Flash(led_ctrl_block_t *led, led_flash_t flashType )
{
    led->ledFlash = flashType;
    
    switch(flashType )
    {
        case LED_FLASH_SOLID_OFF:
        {
            led->ledOffTime = 0;
            led->ledOnTime = 0;
            break;
        }
        case LED_FLASH_SOLID_ON:
        {
            led->ledOffTime = 0;
            led->ledOnTime = 0;
            break;
        }
        case LED_FLASH_SLOW:
        {
            led->ledOffTime = LED_FLASH_SLOW_OFF_TIME;
            led->ledOnTime = LED_FLASH_SLOW_ON_TIME;
            break;
        }
        case LED_FLASH_REGULAR:
        {
            led->ledOffTime = LED_FLASH_REGULAR_OFF_TIME;
            led->ledOnTime = LED_FLASH_REGULAR_ON_TIME;
            break;
        }
        case LED_FLASH_QUICK:
        {
            led->ledOffTime = LED_FLASH_QUICK_OFF_TIME;
            led->ledOnTime = LED_FLASH_QUICK_ON_TIME;
            break;
        }
        default: break;
    }
    
    led->ledDelayCnt = 0;
}


