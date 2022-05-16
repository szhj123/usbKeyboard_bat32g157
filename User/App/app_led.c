/********************************************************
* @file       app_led.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"

#include "app_led.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Led_Handler(void *arg );

/* Private variables ------------------------------------*/
led_ctrl_block_t led1 =
{
    .gpioPort   = GPIO_PORTD,
    .gpioPin    = GPIO_PIN6,
    .ledFlash   = LED_FLASH_SOLID_OFF,
    .ledOnTime  = 0,
    .ledOffTime = 0,
    .ledDelayCnt = 0
};

led_ctrl_block_t led2 =
{
    .gpioPort   = GPIO_PORTD,
    .gpioPin    = GPIO_PIN7,
    .ledFlash   = LED_FLASH_SOLID_ON,
    .ledOnTime  = 0,
    .ledOffTime = 0,
    .ledDelayCnt = 0
};

led_ctrl_block_t led3 =
{
    .gpioPort   = GPIO_PORTD,
    .gpioPin    = GPIO_PIN8,
    .ledFlash   = LED_FLASH_SLOW,
    .ledOnTime  = LED_FLASH_SLOW_ON_TIME,
    .ledOffTime = LED_FLASH_SLOW_OFF_TIME,
    .ledDelayCnt = 0
};

led_ctrl_block_t led4 =
{
    .gpioPort   = GPIO_PORTD,
    .gpioPin    = GPIO_PIN9,
    .ledFlash   = LED_FLASH_REGULAR,
    .ledOnTime  = LED_FLASH_REGULAR_ON_TIME,
    .ledOffTime = LED_FLASH_REGULAR_OFF_TIME,
    .ledDelayCnt = 0
};

led_ctrl_block_t led5 =
{
    .gpioPort   = GPIO_PORTD,
    .gpioPin    = GPIO_PIN10,
    .ledFlash   = LED_FLASH_QUICK,
    .ledOnTime  = LED_FLASH_QUICK_ON_TIME,
    .ledOffTime = LED_FLASH_QUICK_OFF_TIME,
    .ledDelayCnt = 0
};

led_ctrl_block_t led6 =
{
    .gpioPort   = GPIO_PORTD,
    .gpioPin    = GPIO_PIN11,
    .ledFlash   = LED_FLASH_REGULAR,
    .ledOnTime  = LED_FLASH_REGULAR_ON_TIME,
    .ledOffTime = LED_FLASH_REGULAR_OFF_TIME,
    .ledDelayCnt = 0
};



void App_Led_Init(void )
{
    Drv_Led_Init();

    Drv_Timer_Register_Period(App_Led_Handler, 0, 1, NULL);
}

static void App_Led_Handler(void *arg )
{
    if(led1.ledDelayCnt < 0xffff)
    {
        led1.ledDelayCnt++;
    }

    if(led2.ledDelayCnt < 0xffff)
    {
        led2.ledDelayCnt++;
    }

    if(led3.ledDelayCnt < 0xffff)
    {
        led3.ledDelayCnt++;
    }

    if(led4.ledDelayCnt < 0xffff)
    {
        led4.ledDelayCnt++;
    }

    if(led5.ledDelayCnt < 0xffff)
    {
        led5.ledDelayCnt++;
    }

    if(led6.ledDelayCnt < 0xffff)
    {
        led6.ledDelayCnt++;
    }
    
    Drv_Led_Flash(&led1);
    Drv_Led_Flash(&led2);
    Drv_Led_Flash(&led3);
    Drv_Led_Flash(&led4);
    Drv_Led_Flash(&led5);
    Drv_Led_Flash(&led6);
}

