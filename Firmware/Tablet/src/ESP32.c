#include "ESP32.h"
#include "SPI.h"
#include "Interrupts.h"


extern volatile uint16_t x_accel, y_accel, z_accel;
extern volatile uint8_t cube_data_updated_flag;

void esp32_init(void){
    LPC_IOCON->P1_6 = 0x0;
    LPC_GPIO1->DIR |= (0x1)<<6;
    //LPC_GPIO1->SET |= (0x1)<<6;
    LPC_IOCON->P1_5 = 0x0;
    LPC_GPIO1->DIR |= (0x1)<<5;
    LPC_GPIO1->SET |= (0x1)<<5;

}


void esp32_read(uint32_t reg, uint8_t bytes, uint16_t* ret_ptr){
    uint8_t src_addr[SSP_BUFSIZE]; 
    uint8_t dest_addr[SSP_BUFSIZE];

    src_addr[0] = reg | 0x80;
    
    LPC_GPIO1->CLR |= (0x1<<5);
    //LPC_GPIO0->CLR |= (0x1<<1);
    SSPSend(0, (uint8_t *)src_addr, 1);
    SSPReceive(0, (uint8_t *)dest_addr, bytes);
    LPC_GPIO1->SET |= (0x1<<5);
    //LPC_GPIO0->SET |= (0x1)<<1;
    for (uint8_t i=0; i<bytes; i++){
        ret_ptr[i] = dest_addr[i];
    }
    //printf("%x %x %x %x %x %x \n", dest_addr[0], dest_addr[1], dest_addr[2], dest_addr[3], dest_addr[4], dest_addr[5]);
}

void esp32_rcv(void){
    uint8_t src_addr[SSP_BUFSIZE]; 
    uint8_t dest_addr[SSP_BUFSIZE];
    dest_addr[0] = 0x0;
    dest_addr[1] = 0x0;
    dest_addr[2] = 0x0;

    SSPReceive(0, (uint8_t *)dest_addr, 1);
    //LPC_GPIO4->SET |= (0x1<<9);
    //for (uint8_t i=0; i<bytes; i++){
    //    ret_ptr[i] = dest_addr[i];
    //}
    printf("%x %x %x %x %x %x \n", dest_addr[0], dest_addr[1], dest_addr[2], dest_addr[3], dest_addr[4], dest_addr[5]);
}


void esp32_get_accel_data(void){
    uint16_t xyz_vals[6];
    //esp32_read(0x5A, 6, &xyz_vals);
    x_accel = (xyz_vals[0] << 8) | xyz_vals[1];
    y_accel = (xyz_vals[2] << 8) | xyz_vals[3];
    z_accel = (xyz_vals[4] << 8) | xyz_vals[5];
    cube_data_updated_flag = 1;
    //printf("%X  %X  %X\n", x_accel, y_accel, z_accel);
    
    //set flag to signal data needs to be drawn

    //for (uint32_t i=0; i<10000; i++){} // delay to ensure no data missed. can use 1000
}

void esp32_start_ble(void){
    LPC_GPIO1->SET |= (0x1)<<6;
    LPC_GPIO1->CLR |= (0x1)<<6;
}