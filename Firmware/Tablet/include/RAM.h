#include "LPC177x_8x.h"			/* LPC17xx Peripheral Registers */

#define EMC_ADDRESS_DYCS0   (0xA0000800)
#define EMC_ADDRESS_CS0     (0x80000000)
#define DRAM_BASE_ADDRESS   (uint32_t *) EMC_ADDRESS_DYCS0
#define DRAM_SIZE           (8 * 1024 * 8)

extern void delay(uint32_t i);
extern void delay_ms(uint32_t ms);

void ram_test(void);
void ram_init(void);
void ram_write(uint32_t row, uint32_t col, uint32_t val);
void ram_read(uint32_t reg, uint8_t bytes, uint16_t* ret_ptr);
void ram_chip_erase(void);