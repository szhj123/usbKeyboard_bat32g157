/********************************************************
* @file       app_lcd.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"
#include "drv_flash.h"
#include "pic.h"
#include "app_lcd.h"
/* Private typedef --------------------------------------*/

/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Lcd_Clr_Callback(void );

/* Private variables ------------------------------------*/
lcd_ctrl_blokc_t lcdInfo;
func_state_t     funcState;


void App_Lcd_Init(void )
{
    Drv_Lcd_Init();    

    App_Lcd_Gif_Set();
}

void App_Lcd_Handler(void )
{
    if(lcdInfo.handler != NULL)
    {
        lcdInfo.handler();
    }
}

void App_Lcd_Clr_Set(uint16_t color )
{
    lcdInfo.bgColor = color;
    lcdInfo.funcState = FUNC_ENTRY;
    lcdInfo.handler = App_Lcd_Clr_Handler;
    lcdInfo.lcdWrOverFlag = 0;
}

void App_Lcd_Clr_Handler(void )
{
    switch(lcdInfo.funcState)
    {
        case FUNC_ENTRY:
        {
            Drv_Lcd_Clr(lcdInfo.bgColor, App_Lcd_Clr_Callback);
            lcdInfo.funcState = FUNC_EXIT;
            break;
        }
        case FUNC_EXIT:
        {
            if(lcdInfo.lcdWrOverFlag)
            {
                lcdInfo.lcdWrOverFlag = 0;

                App_Lcd_Normal_Set();
            }
            break;
        }
        default: break;
    }
}

void App_Lcd_Gif_Set(void )
{
    lcdInfo.funcState = FUNC_ENTRY;
    lcdInfo.lcdPicIndex = 0;
    lcdInfo.lcdPicNum = 37;
    lcdInfo.handler = App_Lcd_Gif_Handler;
}

void App_Lcd_Gif_Handler(void )
{
    switch(lcdInfo.funcState)
    {
        case FUNC_ENTRY:
        {
            Drv_Lcd_Gif(lcdInfo.lcdPicIndex, App_Lcd_Clr_Callback);
            lcdInfo.funcState = FUNC_PROCESS;
            break;
        }
        case FUNC_PROCESS:
        {
            if(lcdInfo.lcdWrOverFlag)
            {
                lcdInfo.lcdWrOverFlag = 0;
                
                lcdInfo.lcdPicIndex++;

                if(lcdInfo.lcdPicIndex < lcdInfo.lcdPicNum)
                {
                    Drv_Lcd_Gif(lcdInfo.lcdPicIndex, App_Lcd_Clr_Callback);
                }
                else
                {
                    lcdInfo.lcdPicIndex = 0;
                    lcdInfo.funcState = FUNC_ENTRY;
                }
                
            }
            break;
        }
        case FUNC_EXIT:
        {
            break;
        }
        default: break;
    }
}

void App_Lcd_Normal_Set(void )
{
    lcdInfo.funcState = FUNC_ENTRY;
    lcdInfo.handler = App_Lcd_Normal_Handler;
    lcdInfo.lcdWrOverFlag = 0;
}

void App_Lcd_Normal_Handler(void )
{
    switch(lcdInfo.funcState)
    {
        case FUNC_ENTRY:
        {
            break;
        }
        case FUNC_PROCESS:
        {
            break;
        }
        case FUNC_EXIT:
        {
            break;
        }
        default: break;
    }
}

void App_Lcd_Save_Picture(uint16_t picIndex )
{
    uint32_t flashAddr = picIndex * PIC_MAX_SIZE;

    App_Lcd_Erase_Picture(picIndex);
        
    Drv_Spi_Write(flashAddr, (uint8_t *)gImage_pic1, sizeof(gImage_pic1));
}

void App_Lcd_Erase_Picture(uint16_t picIndex )
{
    uint8_t i;
    uint32_t flashAddr = picIndex * PIC_MAX_SIZE;
    
    for(i=0;i<40;i++)
    {
        Drv_Spi_Sector_Erase(flashAddr);

        flashAddr += ERASE_SECTOR_SIZE;
    }
}

static void App_Lcd_Clr_Callback(void )
{
    lcdInfo.lcdWrOverFlag = 1;
}

