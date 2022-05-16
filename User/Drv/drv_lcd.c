/********************************************************
* @file       drv_lcd.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_lcd.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Drv_Lcd_Init(void )
{
    Hal_Lcd_Init();

    LCD_CS_LOW();	
	LCD_RST_LOW();
	Drv_Lcd_Delay_US(120000*48);
	LCD_RST_HIGH();
	Drv_Lcd_Delay_US(100000*48);

    Drv_Lcd_Wr_Reg(0xCF);  
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Data(0xC1); 
	Drv_Lcd_Wr_Data(0X30); 
	Drv_Lcd_Wr_Reg(0xED);  
	Drv_Lcd_Wr_Data(0x64); 
	Drv_Lcd_Wr_Data(0x03); 
	Drv_Lcd_Wr_Data(0X12); 
	Drv_Lcd_Wr_Data(0X81); 
	Drv_Lcd_Wr_Reg(0xE8);  
	Drv_Lcd_Wr_Data(0x85); 
	Drv_Lcd_Wr_Data(0x10); 
	Drv_Lcd_Wr_Data(0x7A); 
	Drv_Lcd_Wr_Reg(0xCB);  
	Drv_Lcd_Wr_Data(0x39); 
	Drv_Lcd_Wr_Data(0x2C); 
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Data(0x34); 
	Drv_Lcd_Wr_Data(0x02); 
	Drv_Lcd_Wr_Reg(0xF7);  
	Drv_Lcd_Wr_Data(0x20); 
	Drv_Lcd_Wr_Reg(0xEA);  
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Reg(0xC0);    //Power control 
	Drv_Lcd_Wr_Data(0x1B);   //VRH[5:0] 
	Drv_Lcd_Wr_Reg(0xC1);    //Power control 
	Drv_Lcd_Wr_Data(0x01);   //SAP[2:0];BT[3:0] 
	Drv_Lcd_Wr_Reg(0xC5);    //VCM control 
	Drv_Lcd_Wr_Data(0x30); 	 //3F
	Drv_Lcd_Wr_Data(0x30); 	 //3C
	Drv_Lcd_Wr_Reg(0xC7);    //VCM control2 
	Drv_Lcd_Wr_Data(0XB7); 
	Drv_Lcd_Wr_Reg(0x36);    // Memory Access Control 
	Drv_Lcd_Wr_Data(0x6C);   //(0xC9); 
	Drv_Lcd_Wr_Reg(0x3A);   
	Drv_Lcd_Wr_Data(0x55); 
	Drv_Lcd_Wr_Reg(0xB1);   
	Drv_Lcd_Wr_Data(0x00);   
	Drv_Lcd_Wr_Data(0x1A); 
	Drv_Lcd_Wr_Reg(0xB6);    // Display Function Control 
	Drv_Lcd_Wr_Data(0x0A); 
	Drv_Lcd_Wr_Data(0xA2);

	Drv_Lcd_Wr_Reg(0xF2);    // 3Gamma Function Disable 
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Reg(0x26);    //Gamma curve selected 
	Drv_Lcd_Wr_Data(0x01); 
	Drv_Lcd_Wr_Reg(0xE0);    //Set Gamma 
	Drv_Lcd_Wr_Data(0x0F); 
	Drv_Lcd_Wr_Data(0x2A); 
	Drv_Lcd_Wr_Data(0x28); 
	Drv_Lcd_Wr_Data(0x08); 
	Drv_Lcd_Wr_Data(0x0E); 
	Drv_Lcd_Wr_Data(0x08); 
	Drv_Lcd_Wr_Data(0x54); 
	Drv_Lcd_Wr_Data(0XA9); 
	Drv_Lcd_Wr_Data(0x43); 
	Drv_Lcd_Wr_Data(0x0A); 
	Drv_Lcd_Wr_Data(0x0F); 
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Data(0x00); 		 
	Drv_Lcd_Wr_Reg(0XE1);    //Set Gamma 
	Drv_Lcd_Wr_Data(0x00); 
	Drv_Lcd_Wr_Data(0x15); 
	Drv_Lcd_Wr_Data(0x17); 
	Drv_Lcd_Wr_Data(0x07); 
	Drv_Lcd_Wr_Data(0x11); 
	Drv_Lcd_Wr_Data(0x06); 
	Drv_Lcd_Wr_Data(0x2B); 
	Drv_Lcd_Wr_Data(0x56); 
	Drv_Lcd_Wr_Data(0x3C); 
	Drv_Lcd_Wr_Data(0x05); 
	Drv_Lcd_Wr_Data(0x10); 
	Drv_Lcd_Wr_Data(0x0F); 
	Drv_Lcd_Wr_Data(0x3F); 
	Drv_Lcd_Wr_Data(0x3F); 
	Drv_Lcd_Wr_Data(0x0F); 
	Drv_Lcd_Wr_Reg(0x2B); 
	Drv_Lcd_Wr_Data(0x00);
	Drv_Lcd_Wr_Data(0x00);
	Drv_Lcd_Wr_Data(0x01);
	Drv_Lcd_Wr_Data(0x3f);
	Drv_Lcd_Wr_Reg(0x2A); 
	Drv_Lcd_Wr_Data(0x00);
	Drv_Lcd_Wr_Data(0x00);
	Drv_Lcd_Wr_Data(0x00);
	Drv_Lcd_Wr_Data(0xef);	 
	Drv_Lcd_Wr_Reg(0x11); //Exit Sleep
	Drv_Lcd_Delay_US(120000*48);//delay_ms(120);
	Drv_Lcd_Wr_Reg(0x29); //display on

    BACKLIGHT_ON();
}

void Drv_Lcd_Clr(uint16_t color, lcd_isr_callback_t callback )
{
    Drv_Lcd_Set_Position(0, 0, LCD_W-1,LCD_H-1);

    Hal_Lcd_Set_BgColor(color, callback);
}

void Drv_Lcd_Gif(uint8_t picIndex, lcd_isr_callback_t callback )
{
    Drv_Lcd_Set_Position(0, 0, LCD_W-1,LCD_H-1);
    
    Hal_Lcd_Show_Picture(picIndex, callback );
}

void Drv_Lcd_Set_Position(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY )
{
	Drv_Lcd_Wr_Reg(0x2A);
	Drv_Lcd_Wr_Data(startX>>8);
	Drv_Lcd_Wr_Data(startX&0xff);
	Drv_Lcd_Wr_Data(endX>>8);
	Drv_Lcd_Wr_Data(endX&0xff);
    
	Drv_Lcd_Wr_Reg(0x2B);
	Drv_Lcd_Wr_Data(startY>>8);
	Drv_Lcd_Wr_Data(startY&0xff);
	Drv_Lcd_Wr_Data(endY>>8);
	Drv_Lcd_Wr_Data(endY&0xff);
}

void Drv_Lcd_Wr_Reg(uint8_t reg )
{
    LCD_CS_LOW();
	LCD_DC_LOW();
	
	Write_LBDATAL(reg);

	LCD_CS_HIGH();
}

void Drv_Lcd_Wr_Data(uint8_t dat )
{
    LCD_CS_LOW();
	LCD_DC_HIGH();
	
	Write_LBDATAL(dat);

	LCD_CS_HIGH();
}

void Drv_Lcd_Delay_US(uint32_t us )
{
    while(--us);
}
