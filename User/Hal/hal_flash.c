/********************************************************
* @file       hal_flash.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Spi_Init(void )
{
    SPI1_MasterInit(SPI_MODE_0);
}

void Hal_Spi_Cs_Clr(void )
{
    NSS1_PORT_CLR();
}

void Hal_Spi_Cs_Set(void )
{
    NSS1_PORT_SET();
}

void Hal_Spi_Start(void )
{
    SPI1_Start();
}

void Hal_Spi_Stop(void )
{
    SPI1_Stop();
}

void Hal_Spi_Tx_Single_With_Blocking(uint8_t val )
{
    SPIHS1->SDRO1 = val; 

    while(SPIHS1->SPIS1 & _0001_SPI_UNDER_EXECUTE) 
    {
        ;
    }
}

void Hal_Spi_Rx_Multiple_With_Block(uint8_t *buf, uint16_t length )
{
    uint16_t i;
    uint8_t dummy = 0xff;

    for(i=0;i<length;i++)
    {
        SPIHS1->SDRO1 = dummy; 
        while(SPIHS1->SPIS1 & _0001_SPI_UNDER_EXECUTE) 
        {
            ;
        }

        buf[i] = SPIHS1->SDRI1;
    }
}



