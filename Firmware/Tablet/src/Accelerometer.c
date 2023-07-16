#include "LPC177x_8x.h"			/* LPC17xx Peripheral Registers */
#include "Accelerometer.h"
#include "SPI.h"
#include <stdlib.h>

extern struct Sensor_Data sensor_data;

 void test_acc(void){
    ////WORKS
    //uint8_t src_addr[SSP_BUFSIZE]; 
    //uint8_t dest_addr[SSP_BUFSIZE];
     
    //for (uint8_t i = 0; i < SSP_BUFSIZE; i++){
    //      src_addr[i] = 0;
    //      dest_addr[i] = 0;
    //}

    //src_addr[0] = 0x0F | 0x80;
    
    //LPC_GPIO0->CLR |= (0x1<<20);
    //SSPSend(1, (uint8_t *)src_addr, 1);
    //SSPReceive(1, (uint8_t *)dest_addr, 1);
    //LPC_GPIO0->SET |= (0x1<<20);
    //printf("%X %X %X %X %X %X %X %X %X\t\n", src_addr[0], dest_addr[0], dest_addr[1], dest_addr[2], dest_addr[3], dest_addr[4], dest_addr[5], dest_addr[6], dest_addr[7]);
    //delay_short();

    acc_write(0x06, 0x12);
}


void acc_pwr_on(void){
    LPC_IOCON->P0_19 = 0x0; // acc gnd enable
    LPC_GPIO0->DIR |= (0x1<<19); // acc gnd enable
    LPC_GPIO0->SET |= (0x1<<19); // turn on acc gnd nfet
    printf("\tAccelerometer power on\n");
}


void acc_pwr_off(void){
    LPC_IOCON->P0_19 = 0x0; // acc gnd enable
    LPC_GPIO0->DIR |= (0x1<<19); // acc gnd enable
    LPC_GPIO0->SET |= (0x0<<19); // turn off acc gnd nfet
    printf("\tAccelerometer power off\n");
}


void acc_write(uint32_t reg, uint32_t val){
    uint8_t src_addr[SSP_BUFSIZE]; 
    uint8_t dest_addr[SSP_BUFSIZE];

    src_addr[0] = reg;
    src_addr[1] = val;
    
    LPC_GPIO0->CLR |= (0x1<<20);
    SSPSend(0, (uint8_t *)src_addr, 2);
    LPC_GPIO0->SET |= (0x1<<20);

    src_addr[0] = reg | 0x80;
    
    LPC_GPIO0->CLR |= (0x1<<20);
    SSPSend(0, (uint8_t *)src_addr, 1);
    SSPReceive(0, (uint8_t *)dest_addr, 1);
    LPC_GPIO0->SET |= (0x1<<20);
}


void acc_read(uint32_t reg, uint8_t bytes, uint16_t* ret_ptr){
    uint8_t src_addr[SSP_BUFSIZE]; 
    uint8_t dest_addr[SSP_BUFSIZE];

    src_addr[0] = reg | 0x80;
    
    LPC_GPIO0->CLR |= (0x1<<20);
    SSPSend(0, (uint8_t *)src_addr, 1);
    SSPReceive(0, (uint8_t *)dest_addr, bytes);
    LPC_GPIO0->SET |= (0x1<<20);
    for (uint8_t i=0; i<bytes; i++){
        ret_ptr[i] = dest_addr[i];
    }
    //printf("%x\n", dest_addr[0]);
}


void acc_init(void){
    LPC_IOCON->P0_20 = 0x000; // acc cs
    LPC_GPIO0->DIR |= (0x1<<20); // acc cs
    acc_pwr_on();    
    acc_write(ACC_CTRL1_XL, 0x80);
    acc_write(ACC_CTRL2_G, 0x48);  
    acc_write(ACC_CTRL4_C, 0x04); 
    //acc_read(ACC_CTRL1_XL);
    //acc_read(ACC_CTRL2_G);  
    //acc_read(ACC_CTRL4_C);  
    printf("\tAccelerometer initialized\n");   

}

void acc_read_xyz(void){
    uint16_t xyz_vals[6];
    acc_read(ACC_OUTX_L_XL, 6, &xyz_vals);
    //for (uint8_t i=0; i<6; i++){
    //    printf("%X ", xyz_vals[i]);
    //}
    //printf("\n");

    sensor_data.acc.x = (xyz_vals[1]<<8) + xyz_vals[0];
    sensor_data.acc.y = (xyz_vals[3]<<8) + xyz_vals[2];
    sensor_data.acc.z = (xyz_vals[5]<<8) + xyz_vals[4];
    printf("%X\t%X\t%X\n", sensor_data.acc.x, sensor_data.acc.y, sensor_data.acc.z);
}