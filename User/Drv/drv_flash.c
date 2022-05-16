/********************************************************
* @file       drv_flash.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_flash.h"
/* Private typedef --------------------------------------*/

/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/

/* Private variables ------------------------------------*/
static flash_id_t flash_id;

void Drv_Flash_Init(void )
{
    Hal_Spi_Init();

    Drv_Spi_Read_Jedec_Id();

    //Drv_Spi_Sector_Erase(0);
}

void Drv_Spi_Read_Jedec_Id(void )
{    
    uint8_t dummy[3] = {0x00};
    
    Hal_Spi_Start();

    Hal_Spi_Tx_Single_With_Blocking(READ_JEDEC_ID);

    Hal_Spi_Rx_Multiple_With_Block((uint8_t *)&flash_id, sizeof(flash_id_t));

    Hal_Spi_Stop();
}

void Drv_Spi_Sector_Erase(uint32_t addr )
{
    Drv_Spi_Write_Enable();

    Hal_Spi_Start();
    
    Hal_Spi_Tx_Single_With_Blocking(SECTOR_ERASE);

    Hal_Spi_Tx_Single_With_Blocking((uint8_t )(addr >> 16));
    Hal_Spi_Tx_Single_With_Blocking((uint8_t )(addr >> 8));
    Hal_Spi_Tx_Single_With_Blocking((uint8_t )(addr));

    Hal_Spi_Stop();

    Drv_Spi_Wait_Bus_Idle();
}

void Drv_Spi_Write_Enable(void )
{
    Hal_Spi_Start();

    Hal_Spi_Tx_Single_With_Blocking(WRITE_ENABLE);

    Hal_Spi_Stop();

    Drv_Spi_Wait_Bus_Idle();
}

uint8_t Drv_Spi_Read_Status(void )
{
    uint8_t status[2] = {0};
    
    Hal_Spi_Tx_Single_With_Blocking(READ_STATUS_REGISTER);

    Hal_Spi_Rx_Multiple_With_Block((uint8_t *)status, 2);

    return status[0];
}

void Drv_Spi_Wait_Bus_Idle(void )
{
    uint8_t status;
    
    Hal_Spi_Start();

    do{
        status = Drv_Spi_Read_Status();
    }while(status & 0x01);

    Hal_Spi_Stop();
}








