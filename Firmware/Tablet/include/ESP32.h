#include <stdio.h>
#include <LPC177x_8x.h>


void esp32_init(void);
void read_esp32(uint32_t reg, uint8_t bytes, uint16_t* ret_ptr);
void esp32_get_accel_data(void);
void esp32_rcv(void);
void esp32_start_ble(void);