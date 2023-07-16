#include "Power.h"
#include "I2C.h"


extern volatile unsigned char I2C_MasterBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned char I2C_SlaveBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned int I2C_Count[I2C_PORT_NUM];
extern volatile unsigned int I2C_ReadLength[I2C_PORT_NUM];
extern volatile unsigned int I2C_WriteLength[I2C_PORT_NUM];

void enable_10v_boost(void){
    LPC_IOCON->P3_18 = 0x0;
    LPC_GPIO3->DIR |= (0x1<<18);
    LPC_GPIO3->SET |= (0x1<<18);
}


void disable_10v_boost(void){
    LPC_IOCON->P3_18 = 0x0;
    LPC_GPIO3->DIR |= (0x1<<18);
    LPC_GPIO3->SET |= (0x0<<18);
}



void liion_read_config(void){
    I2C_ReadLength[1] = 2;
    I2C_WriteLength[1] = 2;
    I2C_MasterBuffer[1][0] = (0x36<<1) | 0x00;
    I2C_MasterBuffer[1][1] = 0x0C;
    I2C_MasterBuffer[1][2] = (0x36<<1) | 0x01;
    I2C_Engine(1);
    printf("%X\t%02X %02X\n", *I2C_MasterBuffer[1], I2C_SlaveBuffer[1][0], I2C_SlaveBuffer[1][1]);

};