#ifndef _DRV_LCD_H
#define _DRV_LCD_H

#include "hal_lcd.h"

void Drv_Lcd_Init(void );
void Drv_Lcd_Clr(uint16_t color , void (*handler)(void ));
void Drv_Lcd_Gif(uint8_t picIndex, lcd_isr_callback_t callback );
void Drv_Lcd_Set_Position(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY );

void Drv_Lcd_Wr_Reg(uint8_t reg );
void Drv_Lcd_Wr_Data(uint8_t dat );

void Drv_Lcd_Delay_US(uint32_t us );

#endif 

