#ifndef _HAL_FLASH_H
#define _HAL_FLASH_H

#include "hardware.h"

#define SPI_FLASH_ADDR 0xE00000


void Hal_Spi_Init(void );
void Hal_Spi_Cs_Clr(void );
void Hal_Spi_Cs_Set(void );
void Hal_Spi_Start(void );
void Hal_Spi_Stop(void );
void Hal_Spi_Tx_Single_With_Blocking(uint8_t val );
void Hal_Spi_Rx_Multiple_With_Block(uint8_t *buf, uint16_t length );


#endif 

