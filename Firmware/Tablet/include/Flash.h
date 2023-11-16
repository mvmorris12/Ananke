#include "LPC177x_8x.h"			/* LPC17xx Peripheral Registers */
#include "SPI.h"
#include <stdlib.h>

#define FLASH_REG_WRITE_ENABLE        (0x06)
#define FLASH_REG_VOL_WRITE_ENABLE    (0x50)
#define FLASH_REG_WRITE_DISABLE       (0x04)
#define FLASH_REG_DEVICE_ID           (0x90)
#define FLASH_REG_UNIQUE_ID           (0x4B)
#define FLASH_REG_READ_DATA           (0x03)
#define FLASH_REG_FAST_READ           (0x0B)
#define FLASH_REG_PAGE_PROGRAM        (0x02)
#define FLASH_REG_ERASE_4KB_SECTOR    (0x20)
#define FLASH_REG_ERASE_32KB_BLOCK    (0x52)
#define FLASH_REG_ERASE_64KB_BLOCK    (0xD8)
#define FLASH_REG_ERASE_CHIP          (0xC7) // 0x60
#define FLASH_REG_STATUS1_READ        (0x05)
#define FLASH_REG_STATUS1_WRITE       (0x01)
#define FLASH_REG_STATUS2_READ        (0x35)
#define FLASH_REG_STATUS2_WRITE       (0x31)
#define FLASH_REG_STATUS3_READ        (0x15)
#define FLASH_REG_STATUS3_WRITE       (0x11)
#define FLASH_REG_GLOBAL_BLOCK_LOCK   (0x7E)
#define FLASH_REG_GLOBAL_BLOCK_UNLOCK (0x98)
#define FLASH_REG_ENABLE_RESET        (0x66)
#define FLASH_REG_RESET_DEVICE        (0x99)


extern void delay(uint32_t i);
void flash_init(void);
void flash_read_page(uint8_t* ret_ptr, uint16_t block, uint16_t sector, uint16_t page);
void flash_read_reg(uint32_t reg, uint8_t bytes, uint8_t* ret_ptr);
void flash_write_reg(uint32_t reg, uint8_t val);
void flash_write_page(uint8_t *data, uint16_t block, uint16_t sector, uint16_t page);
void flash_get_deviceid(void);
void flash_get_uniqueid(void);
void flash_test(void);
void flash_block_erase(uint8_t block);
uint32_t flash_get_address(uint16_t block, uint16_t sector, uint16_t page);
void flash_write_audio(uint32_t *signal, uint8_t rgb);
void flash_write_audio_to_ram(uint8_t rgb);


