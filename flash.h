#ifndef FLASH_H_
#define FLASH_H_


#define FLASH_KEY_1			0x45670123
#define FLASH_KEY_2			0xCDEF89AB

#define FLASH_MIN_ADDRESS	0x08004000
#define FLASH_MAX_ADDRESS	0x080FFFFF

#define FLASH_SECTOR_2		0x08008000
#define FLASH_SECTOR_3		0x0800C000
#define FLASH_SECTOR_4		0x08010000
#define FLASH_SECTOR_5		0x08020000
#define FLASH_SECTOR_6		0x08040000
#define FLASH_SECTOR_7		0x08060000
#define FLASH_SECTOR_8		0x08080000
#define FLASH_SECTOR_9		0x080A0000
#define FLASH_SECTOR_10		0x080C0000
#define FLASH_SECTOR_11		0x080E0000

void flash_unlock(void);
void flash_lock(void);
void flash_clear_sector(uint8_t sector);
void flash_write_8bit(uint8_t* data, uint32_t size, __IO uint8_t* address);
void flash_write_16bit(uint16_t* data, uint32_t size, __IO uint16_t* address);
void flash_write_32bit(uint32_t* data, uint32_t size, __IO uint32_t* address);
uint8_t* flash_read_8bit(__IO uint8_t* address,uint32_t size);
uint16_t* flash_read_16bit(__IO uint16_t* address,uint32_t size);
uint32_t* flash_read_32bit(__IO uint32_t* address,uint32_t size);
#endif /* FLASH_H_ */
