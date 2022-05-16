#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

#include "hardware.h"

#define SPI_DMA_CHANNEL 1

#define SPI_FLASH_ADDR 0xE00000

#define SPI_CS_HIGH() PORT->PSETB = (1<<6)//PB06
#define SPI_CS_LOW() PORT->PCLRB = (1<<6)//PB06

void Drv_Flash_Init(void );

/* Function prototype declaration */
uint8_t SPIHS1_Send(uint8_t ch);

uint32_t SpiFlash_ReadMidDid(void);
void SpiFlash_ChipErase(void);
uint8_t SpiFlash_ReadStatusReg(void);
void SpiFlash_WriteStatusReg(uint32_t u32Value);
void SpiFlash_WaitReady(void);
void SpiFlash_PageProgram(uint32_t u32StartAddress, uint8_t *au8DataBuffer, uint32_t u32ByteCount);
void SpiFlash_ReadData(uint32_t u32StartAddress, uint8_t *au8DataBuffer, uint32_t u32ByteCount);
void SpiFlash_FastReadData(uint32_t u32StartAddress, uint8_t *au8DataBuffer, uint32_t u32ByteCount);


#endif 

