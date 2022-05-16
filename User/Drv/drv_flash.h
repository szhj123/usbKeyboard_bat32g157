#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

#include "hal_flash.h"

#define WRITE_ENABLE                        0x06
#define WRITE_DISABLE                       0x04
#define READ_STATUS_REGISTER                0x05
#define WRITE_STATUS_REGISTER               0x01
#define READ_DATA                           0x03
#define FAST_READ                           0x0b
#define PAGE_PROGRAM                        0x02
#define SECTOR_ERASE                        0x20
#define BLOCK_ERASE_32K                     0x52
#define BLOCK_ERASE_64K                     0xd8
#define CHIP_ERASE                          0x60
#define READ_JEDEC_ID                       0x9f

#define PAGE_SIZE                           256

typedef struct
{
    uint8_t manufacture_id;
    uint8_t memory_type_id;
    uint8_t capacity_id;
}flash_id_t;

void Drv_Flash_Init(void );
void Drv_Spi_Read_Jedec_Id(void );
void Drv_Spi_Sector_Erase(uint32_t addr );
void Drv_Spi_Write_Enable(void );
uint8_t Drv_Spi_Read_Status(void );
void Drv_Spi_Wait_Bus_Idle(void );

#endif 

