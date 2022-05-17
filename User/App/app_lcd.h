#ifndef _APP_LCD_H
#define _APP_LCD_H

#include "drv_lcd.h"


typedef enum
{
    FUNC_ENTRY = 0,
    FUNC_PROCESS,
    FUNC_EXIT
}func_state_t;

typedef struct _lcd_ctrl_blokc_t
{
    void          (*handler)(void );
    func_state_t  funcState;
    uint16_t      bgColor;
    uint8_t       lcdPicNum;
    uint8_t       lcdPicIndex;
    uint8_t       lcdWrOverFlag;
}lcd_ctrl_blokc_t;

void App_Lcd_Init(void );
void App_Lcd_Handler(void );
void App_Lcd_Clr_Set(uint16_t color );
void App_Lcd_Clr_Handler(void );
void App_Lcd_Gif_Set(void );
void App_Lcd_Gif_Handler(void );
void App_Lcd_Normal_Set(void );
void App_Lcd_Normal_Handler(void );
void App_Lcd_Erase_Picture(uint16_t picIndex );
void App_Lcd_Save_Picture(uint16_t picIndex );

#endif 

