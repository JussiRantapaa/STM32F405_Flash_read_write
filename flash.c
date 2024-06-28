#include <stdlib.h>
#include "stm32f4xx.h"
#include "flash.h"
#include "main.h"


void flash_unlock(void){
	// Unlock flash control register
	FLASH->KEYR = (uint32_t)FLASH_KEY_1;
	FLASH->KEYR = (uint32_t)FLASH_KEY_2;
}
void flash_lock(void){
	FLASH->CR |= FLASH_CR_LOCK;
}
void flash_set_width(uint8_t width){
	flash_unlock();
	// Clear program size bits
	FLASH->CR &=~ (3U<<FLASH_CR_PSIZE_Pos);

	if(width == 16)
		FLASH->CR |= (1U<<FLASH_CR_PSIZE_Pos);
	else if(width == 32)
		FLASH->CR |= (2U<<FLASH_CR_PSIZE_Pos);
	else if(width == 64)
		FLASH->CR |= (3U<<FLASH_CR_PSIZE_Pos);
	flash_lock();
}
void flash_clear_sector(uint8_t sector){
	flash_unlock();
	// Check that sector is correct
	if(sector < 2 || sector > 11){
		return;
	}
	while(FLASH->SR & FLASH_SR_BSY){}
	// Clear the sector
	FLASH->CR |= FLASH_CR_SER;

	FLASH->CR &=~ (15U<<FLASH_CR_SNB_Pos);
	FLASH->CR |= (sector<<FLASH_CR_SNB_Pos);
	// Start clearing and wait till its done
	FLASH->CR |= FLASH_CR_STRT;

	while(FLASH->SR & FLASH_SR_BSY){}
	// Clear sector erase bit
	FLASH->CR &=~ FLASH_CR_SER;
	flash_lock();
}
void flash_write_8bit(uint8_t* data, uint32_t size,__IO uint8_t* address){
	flash_set_width(8);
	flash_unlock();
	// Check that no main Flash memory operation is ongoing by checking the BSY bit in the
	while(FLASH->SR & FLASH_SR_BSY){}

	if(((address + size) >= (__IO uint8_t*)FLASH_MAX_ADDRESS) || (address < (__IO uint8_t*)FLASH_MIN_ADDRESS)){
		error();
		return;
	}
	// Set the Program bit in the FLASH_CR register
	FLASH->CR |= FLASH_CR_PG;

	for(int i = 0;i < size;i++){
		*address = *data++;
		address++;
	}
	// Wait for the BSY bit to be cleared.
	while(FLASH->SR & FLASH_SR_BSY){}
	flash_lock();
}
void flash_write_16bit(uint16_t* data, uint32_t size,__IO uint16_t* address){
	flash_set_width(16);
	flash_unlock();

	// Check that no main Flash memory operation is ongoing by checking the BSY bit in the
	while(FLASH->SR & FLASH_SR_BSY){}

	// Check we are staying in bounds
	if(((address + size) >= (__IO uint16_t*)FLASH_MAX_ADDRESS) || (address < (__IO uint16_t*)FLASH_MIN_ADDRESS)){
		error();
		return;
	}
	// Set the Program bit in the FLASH_CR register
	FLASH->CR |= FLASH_CR_PG;

	for(int i = 0;i < size;i++){
		*address = *data++;
		address++;
	}
	// Wait for the BSY bit to be cleared.
	while(FLASH->SR & FLASH_SR_BSY){}

	flash_lock();
}
void flash_write_32bit(uint32_t* data, uint32_t size,__IO uint32_t* address){
	flash_set_width(32);
	flash_unlock();
	// Check that no main Flash memory operation is ongoing by checking the BSY bit in the
	while(FLASH->SR & FLASH_SR_BSY){}

	// Check we are within bounds
	if(((address + size) >= (__IO uint32_t*)FLASH_MAX_ADDRESS) || (address < (__IO uint32_t*)FLASH_MIN_ADDRESS)){
		error();
		return;
	}
	// Set the Program bit in the FLASH_CR register
	FLASH->CR |= FLASH_CR_PG;

	for(int i = 0;i < size;i++){
		*address = *data++;
		address++;
	}
	// Wait for the BSY bit to be cleared.
	while(FLASH->SR & FLASH_SR_BSY){}

	flash_lock();
}
uint8_t* flash_read_8bit(__IO uint8_t* address,uint32_t size){
	// Check that address is in bounds
	if(((address + size) >= (__IO uint8_t*)FLASH_MAX_ADDRESS) || (address < (__IO uint8_t*)FLASH_MIN_ADDRESS))
		return NULL;

	// Wait till flash is not busy
	while(FLASH->SR & FLASH_SR_BSY){}

	uint8_t* ptr = (uint8_t*)malloc(size);
	// Copy the contents
	for(int i = 0; i < size;i++){
		ptr[i] = *address++;
	}
	// Wait till flash is not busy
	while(FLASH->SR & FLASH_SR_BSY){}

	return ptr;
}
uint16_t* flash_read_16bit(__IO uint16_t* address,uint32_t size){
	// Check that address is in bounds
	if(((address + size) >= (__IO uint16_t*)FLASH_MAX_ADDRESS) || (address < (__IO uint16_t*)FLASH_MIN_ADDRESS))
		return NULL;

	// Wait till flash is not busy
	while(FLASH->SR & FLASH_SR_BSY){}

	uint16_t* ptr = (uint16_t*)malloc(size);
	// Copy the contents
	for(int i = 0; i < size;i++){
		ptr[i] = *address++;
	}
	// Wait till flash is not busy
	while(FLASH->SR & FLASH_SR_BSY){}

	return ptr;
}
uint32_t* flash_read_32bit(__IO uint32_t* address,uint32_t size){
	// Check that address is in bounds
	if(((address + size) >= (__IO uint32_t*)FLASH_MAX_ADDRESS) || (address < (__IO uint32_t*)FLASH_MIN_ADDRESS))
		return NULL;

	// Wait till flash is not busy
	while(FLASH->SR & FLASH_SR_BSY){}

	uint32_t* ptr = (uint32_t*)malloc(sizeof(uint32_t) * size);
	// Copy the contents
	for(int i = 0; i < size;i++){
		ptr[i] = *address++;
	}
	// Wait till flash is not busy
	while(FLASH->SR & FLASH_SR_BSY){}

	return ptr;
}

