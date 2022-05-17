/********************************************************
* @file       interrupt.c
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
#include "hal_flash.h"
#include "hal_lcd.h"
#include "hal_uart.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
void IRQ18_Handler(void) __attribute__((alias("tm40_channel0_interrupt")));
void IRQ15_Handler(void) __attribute__((alias("lcdb_interrupt")));
void IRQ12_Handler(void) __attribute__((alias("spi1_interrupt")));
void IRQ08_Handler(void) __attribute__((alias("uart0_interrupt_receive")));

/* Private variables ------------------------------------*/

void tm40_channel0_interrupt(void )
{
    INTC_ClearPendingIRQ(TM00_IRQn);    /* clear INTTM00 interrupt flag */

    Drv_Timer_Isr_Handler();
}

void lcdb_interrupt(void )
{
    INTC_ClearPendingIRQ(LCDB_IRQn);     /* clear LCDB interrupt flag */

    Hal_Lcd_Clr_Isr_Handler();
}

void spi1_interrupt(void )
{
   INTC_ClearPendingIRQ(SPI1_IRQn);

   Hal_Lcd_Gif_Isr_Handler();

   Hal_Spi_Tx_Isr_Handler();

   Hal_Spi_Rx_Isr_Handler();
}

void uart0_interrupt_receive(void )
{
    INTC_ClearPendingIRQ(SR0_IRQn);

    Hal_Uart_Rx_Isr_Handler();
}

