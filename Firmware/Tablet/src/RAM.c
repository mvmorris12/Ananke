#include "RAM.h"
#include "LPC177x_8x.h"			/* LPC17xx Peripheral Registers */
#include "SPI.h"
#include <stdlib.h>


void ram_init(void){
    LPC_SC->PCONP     |= (0x1 << 11);
    //LPC_EMC->Config &= ~(0x1);
    LPC_EMC->Config |= (0x1);
    LPC_SC->EMCCLKSEL  = (0x0);               // 0: SYSCLOCK, 1: SYSCLOCK/2
    LPC_IOCON->P2_19   = (0x0);
    LPC_IOCON->P2_16  |= (0x1 | (0x1 << 9));  // CAS
    LPC_IOCON->P2_20  |= (0x1 | (0x1 << 9));  // DYSC0
    LPC_IOCON->P2_17  |= (0x1 | (0x1 << 9));  // RAS
    LPC_IOCON->P4_25  |= (0x1 | (0x1 << 9));  // WE
    LPC_IOCON->P2_24  |= (0x1 | (0x1 << 9));  // CLKEN0
    LPC_IOCON->P2_18  |= (0x1);               // RAM CLK
    LPC_IOCON->P2_28  |= (0x1 | (0x1 << 9));  // LDQM
    LPC_IOCON->P2_29  |= (0x1 | (0x1 << 9));  // UDQM
    LPC_IOCON->P4_0   |= (0x1 | (0x1 << 9));  // A0
    //LPC_IOCON->P4_0    = (0x0);  // A0
    LPC_IOCON->P4_1   |= (0x1 | (0x1 << 9));  // A1
    LPC_IOCON->P4_2   |= (0x1 | (0x1 << 9));  // A2
    LPC_IOCON->P4_3   |= (0x1 | (0x1 << 9));  // A3
    LPC_IOCON->P4_4   |= (0x1 | (0x1 << 9));  // A4
    LPC_IOCON->P4_5   |= (0x1 | (0x1 << 9));  // A5
    LPC_IOCON->P4_6   |= (0x1 | (0x1 << 9));  // A6
    LPC_IOCON->P4_7   |= (0x1 | (0x1 << 9));  // A7
    LPC_IOCON->P4_8   |= (0x1 | (0x1 << 9));  // A8
    LPC_IOCON->P4_9   |= (0x1 | (0x1 << 9));  // A9
    LPC_IOCON->P4_10  |= (0x1 | (0x1 << 9));  // A10
    LPC_IOCON->P4_11  |= (0x1 | (0x1 << 9));  // A11
    //LPC_IOCON->P4_12  |= (0x1 | (0x1 << 9));  // A11
    LPC_IOCON->P4_12   = 0x0;  // A12 turned off
    LPC_IOCON->P4_13  |= (0x1 | (0x1 << 9));  // BA0
    LPC_IOCON->P4_14  |= (0x1 | (0x1 << 9));  // BA1
    LPC_IOCON->P3_0   |= (0x1 | (0x1 << 9));  // D0
    LPC_IOCON->P3_1   |= (0x1 | (0x1 << 9));  // D1
    LPC_IOCON->P3_2   |= (0x1 | (0x1 << 9));  // D2
    LPC_IOCON->P3_3   |= (0x1 | (0x1 << 9));  // D3
    LPC_IOCON->P3_4   |= (0x1 | (0x1 << 9));  // D4
    LPC_IOCON->P3_5   |= (0x1 | (0x1 << 9));  // D5
    LPC_IOCON->P3_6   |= (0x1 | (0x1 << 9));  // D6
    LPC_IOCON->P3_7   |= (0x1 | (0x1 << 9));  // D7
    LPC_IOCON->P3_8   |= (0x1 | (0x1 << 9));  // D8
    LPC_IOCON->P3_9   |= (0x1 | (0x1 << 9));  // D9
    LPC_IOCON->P3_10  |= (0x1 | (0x1 << 9));  // D10
    LPC_IOCON->P3_11  |= (0x1 | (0x1 << 9));  // D11
    LPC_IOCON->P3_12  |= (0x1 | (0x1 << 9));  // D12
    LPC_IOCON->P3_13  |= (0x1 | (0x1 << 9));  // D13
    LPC_IOCON->P3_14  |= (0x1 | (0x1 << 9));  // D14
    LPC_IOCON->P3_15  |= (0x1 | (0x1 << 9));  // D15
    LPC_IOCON->P3_16   = (0x0);
    LPC_IOCON->P3_30   = (0x0);               // SET TO GPIO
    LPC_SC->EMCDLYCTL  = 0x00081415;
    LPC_EMC->DynamicConfig0 = 0X00000480;     //((0x2 << 9) | (0x1 << 7)); 
    LPC_EMC->DynamicRasCas0 = 0x00000303;     // 3 RAS, 3 CAS latency */
    LPC_EMC->DynamicReadConfig = 0x00000001;     // Min 20ns. (n + 1) -> 3 clock cycles
    LPC_EMC->DynamicRP      = 0x00000002;     // Min 20ns. (n + 1) -> 3 clock cycles
    LPC_EMC->DynamicRAS     = 0x00000005;     // Min 44 ns.(n + 1) -> 6 clock cycles
    LPC_EMC->DynamicSREX    = 0x00000008;     // Min 75 ns. ( n + 1 ) -> 10 clock cycles */
    LPC_EMC->DynamicAPR     = 0x00000002;     // ??? ( n + 1 ) -> 2 clock cycles */
    LPC_EMC->DynamicDAL     = 0x00000006;     // tWR + tRP: min 35 ns (n + 1) -> 5 clock cycles */
    LPC_EMC->DynamicWR      = 0x00000002;     // Min 15 ns. ( n + 1 ) -> 2 clock cycles */
    LPC_EMC->DynamicRC      = 0x00000002;     // Min 66 ns. ( n + 1 ) -> 8 clock cycles */
    LPC_EMC->DynamicRFC     = 0x0000001F;     // Min 66 ns. ( n + 1 ) -> 8 clock cycles */
    LPC_EMC->DynamicXSR     = 0x00000009;     // Min 75 ns. ( n + 1 ) -> 10 clock cycles */
    LPC_EMC->DynamicRRD     = 0x00000002;     // Min 15 ns. ( n + 1 ) -> 2 clock cycles */
    LPC_EMC->DynamicMRD     = 0x00000002;     // 2 tCK. ( n + 1 ) -> 2 clock cycles */
    delay_ms(100);
    LPC_EMC->DynamicControl = 0x00000183; /* Issue NOP command */
    delay_ms(200);   /* wait 200ms */
    LPC_EMC->DynamicControl = 0x00000103; /* Issue PALL command */
    LPC_EMC->DynamicRefresh = 0x00000001; /* ( n * 16 ) -> 32 clock cycles */
    //LPC_EMC->Config &= 0xE;
    for(uint8_t i = 0; i < 0x80; i++);           /* wait 128 AHB clock cycles */

    //Timing for 120MHz Bus 
    LPC_EMC->DynamicRefresh = 0x7FF;
    LPC_EMC->DynamicControl = 0x00000083; /* Issue MODE command */
    //LPC_EMC->Config &= ~(0x1);
    LPC_EMC->Config |= (0x1);

    //Timing for 48/60/72MHZ Bus
    uint32_t dwtemp = *((volatile uint32_t *)(0xA0000000 | (0x33<<(12)))); /* 8 burst, 3 CAS latency */
    LPC_EMC->DynamicControl  = 0x00000000; /* Issue NORMAL command */

    //[re]enable buffers
    LPC_EMC->DynamicConfig0 |= 0x00080000; // Buffer enable
    //LPC_EMC->Config &= ~(0x1);
    LPC_EMC->Config |= (0x1);
    delay_ms(100);
    ram_write_init(NULL,NULL,NULL);
    //ram_read(NULL,NULL,NULL);
    printf("ram setup done\n");
    ram_test();
    
}


void ram_write_init(uint32_t row, uint32_t col, uint32_t val){
    uint32_t* ram_addr = (uint32_t *)0xA0000000;
    //*ram_addr = 0xABCDEF21;
    //ram_addr++;
    for (uint32_t j=0; j<480; j++){
        for (uint32_t i=0; i<800; i++){
            *ram_addr = 0x00000000;
            ram_addr++;
            //*ram_addr = 0x0000;
            //ram_addr++;
        }
        //}
    }
    ram_addr = (uint32_t *)0xA0500000;
    //*ram_addr = 0xABCDEF21;
    //ram_addr++;
    for (uint32_t j=0; j<480; j++){
        for (uint32_t i=0; i<800; i++){
            *ram_addr = 0x00000000;
            ram_addr++;
            //*ram_addr = 0x0000;
            //ram_addr++;
        }
        //}
    }
    //ram_addr++;
    //for (uint32_t i=0; i<4000; i++){
    //    *ram_addr = 0xFF00FF00;
    //    ram_addr++;
    //}
    //for (uint32_t i=0; i<200000; i++){
    //    *ram_addr = 0x0000FF00;
    //    ram_addr++;
    //}
}

void ram_read(uint32_t reg, uint8_t bytes, uint16_t* ret_ptr){
    uint32_t  read_val = 0;
    uint32_t* ram_addr;
    ram_addr = (uint32_t *)0xA0000000;
    for (uint32_t i=1; i<800000; i++){
        read_val = *ram_addr;
        //delay(1000000);
        printf("%d %x %x\n", i, ram_addr, read_val);//, read_val_test);
        ram_addr++;
        //delay(1000000);
    }
}


void ram_chip_erase(void){

}


void ram_write_test(void){
  
}

void ram_test(void){
    uint32_t* ram_addr;
    ram_addr = (uint32_t*)0xA0000000;
    //ram_addr++;

    printf("start RAM write test\n");
    for (uint32_t i=0; i<200; i++){
        switch(i&0x0F){
            case 0x0:
                *ram_addr = 0xF000F123;
                break;
            case 0x1:
                *ram_addr = 0xF111F11F;
                break;
            case 0x2:
                *ram_addr = 0xF222F22F;
                break;
            case 0x3:
                *ram_addr = 0xF333F33F;
                break;
            case 0x4:
                *ram_addr = 0xF444F44F;
                break;
            case 0x5:
                *ram_addr = 0xF555F55F;
                break;
            case 0x6:
                *ram_addr = 0xF666F66F;
                break;
            case 0x7:
                *ram_addr = 0xF777F77F;
                break;
            case 0x8:
                *ram_addr = 0xF888F88F;
                break;
            case 0x9:
                *ram_addr = 0xF999F99F;
                break;
            case 0xA:
                *ram_addr = 0xFAAAFAAF;
                break;
            case 0xB:
                *ram_addr = 0xFBBBFBBF;
                break;
            case 0xC:
                *ram_addr = 0xFCCCFCCF;
                break;
            case 0xD:
                *ram_addr = 0xFDDDFDDF;
                break;
            case 0xE:
                *ram_addr = 0xFEEEFEEF;
                break;
            case 0xF:
                *ram_addr = 0xFABCF987;
                break;
        }
        //*ram_addr = 0xABCDEF01;
        //printf("%X  %X\n", i, *ram_addr);
    //*ram_addr = i | 0xD000;
        //printf("%X  %X\n", i, (0xF000|i));
        //ram_addr+=4;
        ram_addr++;
        //delay(10000);
    }
    
    printf("finished RAM write test\n");
    //delay_ms(10000);
    printf("start RAM read test\n");
    ram_addr = (uint32_t*)0xA0000004;
    //ram_addr++;
    uint32_t error_cnt = 0;
    uint32_t read_val = 0;
    uint32_t read_val_test = 0;

    for (uint32_t i=1; i<200; i++){
        read_val = *ram_addr;
        switch(i&0x0F){
            case 0x0:
                read_val_test = 0xF000F123;
                break;
            case 0x1:
                read_val_test = 0xF111F11F;
                break;
            case 0x2:
                read_val_test = 0xF222F22F;
                break;
            case 0x3:
                read_val_test = 0xF333F33F;
                break;
            case 0x4:
                read_val_test = 0xF444F44F;
                break;
            case 0x5:
                read_val_test = 0xF555F55F;
                break;
            case 0x6:
                read_val_test = 0xF666F66F;
                break;
            case 0x7:
                read_val_test = 0xF777F77F;
                break;
            case 0x8:
                read_val_test = 0xF888F88F;
                break;
            case 0x9:
                read_val_test = 0xF999F99F;
                break;
            case 0xA:
                read_val_test = 0xFAAAFAAF;
                break;
            case 0xB:
                read_val_test = 0xFBBBFBBF;
                break;
            case 0xC:
                read_val_test = 0xFCCCFCCF;
                break;
            case 0xD:
                read_val_test = 0xFDDDFDDF;
                break;
            case 0xE:
                read_val_test = 0xFEEEFEEF;
                break;
            case 0xF:
                read_val_test = 0xFABCF987;
                break;
        }
        uint64_t read_val = {0};
        read_val = *ram_addr;
            //printf("%X  %X\n", i, read_val);
        //}
        if (read_val != read_val_test){
            error_cnt++;
            printf("%d %x %x %x %x %x\n", i, ram_addr, read_val, read_val_test);
            //break;
        }
        //ram_addr+=4;
        ram_addr++;
        //delay(100);
    }
    delay(100);
    //printf("errors: %d\n", error_cnt);
    printf("\n");

};