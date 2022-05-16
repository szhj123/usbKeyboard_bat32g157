#ifndef _DRV_LED_H
#define _DRV_LED_H

#include "hal_led.h"

#define LED_FLASH_SLOW_ON_TIME                   250 //ms
#define LED_FLASH_SLOW_OFF_TIME                  1000 //ms
#define LED_FLASH_REGULAR_ON_TIME                400 //ms
#define LED_FLASH_REGULAR_OFF_TIME               400 //ms
#define LED_FLASH_QUICK_ON_TIME                  250 //ms
#define LED_FLASH_QUICK_OFF_TIME                 250 //ms


typedef enum
{
    GPIO_PORTA = 0,
    GPIO_PORTB = 1,
    GPIO_PORTC = 2,
    GPIO_PORTD = 3,
    GPIO_PORTH = 7
}gpio_port_t;

typedef enum
{
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,
    GPIO_PIN8,
    GPIO_PIN9,
    GPIO_PIN10,
    GPIO_PIN11,
    GPIO_PIN12,
    GPIO_PIN13,
    GPIO_PIN14,
    GPIO_PIN15
}gpio_pin_t;

typedef enum _led_flash_t
{
    LED_FLASH_SOLID_OFF = 0,
    LED_FLASH_SOLID_ON,
    LED_FLASH_SLOW,
    LED_FLASH_REGULAR,
    LED_FLASH_QUICK
}led_flash_t;

typedef struct _led_ctrl_block_t
{
    gpio_port_t gpioPort;
    gpio_pin_t  gpioPin;

    led_flash_t ledFlash;

    uint16_t    ledOnTime;
    uint16_t    ledOffTime;
    uint16_t    ledDelayCnt;
    
}led_ctrl_block_t;


void Drv_Led_Init(void );
void Drv_Led_Flash(led_ctrl_block_t *led );
void Drv_Led_Set_Flash(led_ctrl_block_t *led, led_flash_t flashType );

#endif 

