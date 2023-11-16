#include "Flash.h"
#include "LPC177x_8x.h"			/* LPC17xx Peripheral Registers */
#include "Speech.h"
#include "SPI.h"
#include <stdlib.h>



void flash_init(void){
    LPC_IOCON->P1_8  = 0x0;// | (0x1 << 3); // flash cs
    LPC_IOCON->P1_4  = 0x0; // flash wp
    LPC_IOCON->P4_31 = 0x0; // flash hold
    LPC_GPIO1->DIR |= (0x1 << 8);
    LPC_GPIO1->DIR |= (0x1 << 4);
    LPC_GPIO4->DIR |= (0x1 << 31);
    LPC_GPIO1->SET |= (0x1 << 8);
    LPC_GPIO1->SET |= (0x1 << 4);
}


uint32_t flash_get_address(uint16_t block, uint16_t sector, uint16_t page){
    uint32_t start_address = 0;
    uint32_t end_address = 0;
    start_address =  ((block*64*1024) + (sector*4*1024) + (page*256));
    end_address =  (start_address+255);
    printf("%06X %06X\n", start_address, end_address);
    return start_address;
}

void flash_read_page(uint8_t* ret_ptr, uint16_t block, uint16_t sector, uint16_t page){
    uint8_t src_addr[SSP_BUFSIZE]; 
    uint8_t dest_addr[SSP_BUFSIZE];
    uint8_t bytes = 32;
    uint32_t address = flash_get_address(block, sector, page);

    //src_addr[0] = FLASH_REG_WRITE_ENABLE;
    src_addr[0] = FLASH_REG_READ_DATA;
    src_addr[1] = (address >> 16) & 0xFF;
    src_addr[2] = (address >> 8)  & 0xFF;
    src_addr[3] = (address >> 0)  & 0xFF;
    //printf("src1 %02X\n", src_addr[1]);
    //printf("src2 %02X\n", src_addr[2]);
    //printf("src3 %02X\n", src_addr[3]);
    
    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr, 4);
    SSPReceive(0, (uint8_t *)dest_addr, bytes);
    LPC_GPIO1->SET |= (0x1<<8);
    for (uint8_t i=0; i<bytes; i++){
        ret_ptr[i] = dest_addr[i];
    }
    //printf("%x\n", dest_addr[0]);
}


void flash_read_reg(uint32_t reg, uint8_t bytes, uint8_t* ret_ptr){
    uint8_t src_addr[SSP_BUFSIZE]; 
    uint8_t dest_addr[SSP_BUFSIZE];

    src_addr[0] = reg;
    
    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr, 1);
    SSPReceive(0, (uint8_t *)dest_addr, bytes);
    LPC_GPIO1->SET |= (0x1<<8);
    for (uint8_t i=0; i<bytes; i++){
        ret_ptr[i] = dest_addr[i];
    }
    //printf("%x\n", dest_addr[0]);
}


void flash_write_reg(uint32_t reg, uint8_t val){
    uint8_t src_addr[SSP_BUFSIZE]; 
    uint8_t dest_addr[SSP_BUFSIZE];

    src_addr[0] = reg;
    src_addr[1] = val;
    
    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr, 2);
    LPC_GPIO1->SET |= (0x1<<8);
}

void flash_write_page(uint8_t *data, uint16_t block, uint16_t sector, uint16_t page){
    uint8_t src_addr[FLASH_BUFSIZE]; 
    uint8_t dest_addr[SSP_BUFSIZE];
    for (uint16_t i=0; i<32; i++){
        src_addr[i+4] = data[i];
    }
    uint32_t address = flash_get_address(block, sector, page);

    src_addr[0] = FLASH_REG_WRITE_ENABLE;
    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr, 1);
    LPC_GPIO1->SET |= (0x1<<8);

    delay(10);

    src_addr[0] = FLASH_REG_GLOBAL_BLOCK_UNLOCK;
    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr, 1);
    LPC_GPIO1->SET |= (0x1<<8);

    
    src_addr[0] = FLASH_REG_PAGE_PROGRAM;
    src_addr[1] = (address >> 16) & 0xFF;
    src_addr[2] = (address >> 8)  & 0xFF;
    src_addr[3] = (address >> 0)  & 0xFF;
    
    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr, 36);
    LPC_GPIO1->SET |= (0x1<<8);
    flash_read_reg(FLASH_REG_STATUS1_READ, 1, dest_addr);
    //delay(10000);
}


void flash_get_deviceid(void){
    uint8_t spi_rx[5] = {0};
    uint16_t deviceid = 0;
    flash_read_reg(FLASH_REG_DEVICE_ID, 5, spi_rx);
    deviceid = (spi_rx[3] << 8) | (spi_rx[4] << 0);
    printf("Flash Device ID: %04X\n", deviceid);
}


void flash_get_uniqueid(void){
    uint8_t spi_rx[12] = {0};
    uint64_t uniqueid_buf[12] = {0};
    uint64_t uniqueid = 0;
    flash_read_reg(FLASH_REG_UNIQUE_ID, 12, spi_rx);
    for (uint8_t i=0; i<8; i++){
        uniqueid_buf[i] = spi_rx[i+4];
    }
    uniqueid = (uniqueid_buf[0] << 56) |
               (uniqueid_buf[1] << 48) |
               (uniqueid_buf[2] << 40) |
               (uniqueid_buf[3] << 32) |
               (uniqueid_buf[4] << 24) |
               (uniqueid_buf[5] << 16) |
               (uniqueid_buf[6] << 8)  |
               (uniqueid_buf[7] << 0);
     printf("Flash Unique ID: %08X\n", uniqueid);
}


void flash_block_erase(uint8_t block){
    uint32_t address = block * 0x10000;
    uint8_t src_addr[SSP_BUFSIZE]; 

    src_addr[0] = FLASH_REG_WRITE_ENABLE;
    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr, 1);
    LPC_GPIO1->SET |= (0x1<<8);

    delay(10);

    src_addr[0] = FLASH_REG_GLOBAL_BLOCK_UNLOCK;
    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr, 1);
    LPC_GPIO1->SET |= (0x1<<8);

    
    src_addr[0] = FLASH_REG_ERASE_64KB_BLOCK;
    src_addr[1] = (address >> 16) & 0xFF;
    src_addr[2] = (address >> 8)  & 0xFF;
    src_addr[3] = (address >> 0)  & 0xFF;

    LPC_GPIO1->CLR |= (0x1<<8);
    SSPSend(0, (uint8_t *)src_addr,4);
    LPC_GPIO1->SET |= (0x1<<8);
    delay(10000000);

}

void flash_test(void){
    int test = 33;
    uint8_t spi_rx[33] = {0};
    //uint8_t* spi_rx = (uint8_t*)malloc(sizeof(uint8_t)*256);
    printf("flash test\n");
    //flash_init();
    //flash_block_erase(0);
    uint8_t flash_data_array[33];
    //uint8_t* flash_data_array = (uint8_t*)malloc(sizeof(uint8_t)*256);
    for (uint8_t i=0; i<test; i++){
        flash_data_array[i] = i;
    }
flash_block_erase(1);
        delay(100000);
    flash_write_page(&flash_data_array, 1, 0, 15);
        delay(100000);
    flash_read_page(spi_rx, 1, 0, 15);
    for (uint8_t i=0; i<32; i++){
        printf("%d\t%02X\n", i, spi_rx[i]);
        delay(10000);
    }
    int i=1;
    printf("end %d\n", i);

}

void flash_write_audio(uint32_t *signal, uint8_t rgb){
    uint8_t spi_rx[32] = {0};
    uint8_t flash_data_array[32];
    uint16_t block = 0;
    uint16_t sector = 0;
    uint16_t page = 0;
    //uint8_t* spi_rx = (uint8_t*)malloc(sizeof(uint8_t)*256);
    printf("flash test\n");
    //flash_init();
    flash_block_erase(0);
    for (uint8_t i=0; i<32; i++){
        flash_data_array[i] = i;
    }
    flash_write_page(flash_data_array, 0, 0, 0);
        delay(100000);
    flash_read_page(spi_rx, 0, 0, 0);
    for (uint8_t i=0; i<32; i++){
        printf("%d\t%02X\n", i, spi_rx[i]);
        delay(10000);
    }
    //printf("%X, %X\n", audio_signal_r[0], audio_signal_r[500]);
    //printf("%X, %X\n", audio_signal_g[0], audio_signal_g[500]);
    //printf("%X, %X\n", audio_signal_b[0], audio_signal_b[500]);
    int i=1;
    printf("end %d\n", i);
    
}

void flash_write_audio_to_ram(uint8_t rgb){

}

