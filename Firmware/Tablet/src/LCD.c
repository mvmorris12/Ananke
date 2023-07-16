#include "LCD.h"
#include "Power.h"
#include "I2C.h"
#include <stdlib.h>

#define PIXELS_ROW  (480)
#define PIXELS_COL  (800)
#define RGB(r,g,b) ((r<<16) + (g<<8) + (b))

extern volatile unsigned char I2C_MasterBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned char I2C_SlaveBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned int I2C_Count[I2C_PORT_NUM];
extern volatile unsigned int I2C_ReadLength[I2C_PORT_NUM];
extern volatile unsigned int I2C_WriteLength[I2C_PORT_NUM];
extern volatile uint8_t get_touch_coordinates_flag;
typedef uint32_t lcd_arr_t[PIXELS_COL];

uint32_t *pFB32, col;
uint16_t touch_x[5], touch_y[5];
lcd_arr_t *lcd_arr = (lcd_arr_t*) 0xA0000000; // TODO was 0xA0000800;


void lcd_test(void){
    lcd_draw_circle(200,200,50,0xFFFFFFFF);
    lcd_draw_circle(200,200,70,0xFFFFFFFF);
    lcd_draw_circle(200,200,90,0xFFFFFFFF);
    lcd_draw_circle(200,200,30,0xFFFFFFFF);
    lcd_draw_circle(200,200,10,0xFFFFFFFF);
}

void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t val){
    lcd_arr[y][x] = val;
}


void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color){
    int16_t dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int16_t err = dx + dy, e2; /* error value e_xy */ 
    printf("dx: %x\ndy: %d\nerr: %d\n", dx, dy, err);
    for (;;){  /* loop */
        lcd_draw_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1){
            break;
        }
        e2 = 2 * err;
        if (e2 >= dy){ 
            err += dy; 
            x0 += sx;  /* e_xy+e_x > 0 */
        } 
        if (e2 <= dx){ 
            err += dx; 
            y0 += sy;  /* e_xy+e_y < 0 */
        } 
    }
}


void lcd_draw_circle(int16_t xm, int16_t ym, int16_t r, uint32_t color){
    int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
    do {
        lcd_draw_pixel(xm-x, ym+y, color);  /*   I. Quadrant */
        lcd_draw_pixel(xm-y, ym-x, color);  /*  II. Quadrant */
        lcd_draw_pixel(xm+x, ym-y, color);  /* III. Quadrant */
        lcd_draw_pixel(xm+y, ym+x, color);  /*  IV. Quadrant */
        r = err;
        if (r >  x){
            err += ++x*2+1; /* e_xy+e_x > 0 */
        }
        if (r <= y){
            err += ++y*2+1; /* e_xy+e_y < 0 */
        }
    } while (x < 0);
}

void lcd_read_touch_coords(void){
    //printf("getting touch coordinates\n");
    I2C_ReadLength[0] = 28;
    I2C_WriteLength[0] = 2;
    I2C_MasterBuffer[0][0] = (0x38<<1) | 0x00;
    I2C_MasterBuffer[0][1] = 0x03;
    I2C_MasterBuffer[0][2] = (0x38<<1) | 0x01;

    I2C_Engine(0);
    touch_x[0] = ((I2C_SlaveBuffer[0][0] & 0x0F)<<8) + I2C_SlaveBuffer[0][1];
    touch_y[0] = ((I2C_SlaveBuffer[0][2])<<8) + I2C_SlaveBuffer[0][3];
    touch_x[1] = ((I2C_SlaveBuffer[0][6] & 0x0F)<<8) + I2C_SlaveBuffer[0][7];
    touch_y[1] = ((I2C_SlaveBuffer[0][8] & 0x0F)<<8) + I2C_SlaveBuffer[0][9];
    touch_x[2] = ((I2C_SlaveBuffer[0][12] & 0x0F)<<8) + I2C_SlaveBuffer[0][13];
    touch_y[2] = ((I2C_SlaveBuffer[0][14] & 0x0F)<<8) + I2C_SlaveBuffer[0][15];
    touch_x[3] = ((I2C_SlaveBuffer[0][18] & 0x0F)<<8) + I2C_SlaveBuffer[0][19];
    touch_y[3] = ((I2C_SlaveBuffer[0][20] & 0x0F)<<8) + I2C_SlaveBuffer[0][21];
    touch_x[4] = ((I2C_SlaveBuffer[0][24] & 0x0F)<<8) + I2C_SlaveBuffer[0][25];
    touch_y[4] = ((I2C_SlaveBuffer[0][26] & 0x0F)<<8) + I2C_SlaveBuffer[0][27];
    if (touch_x[0] < 480 && touch_y[0] < 800){
        printf("\n%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", touch_x[0], touch_y[0], touch_x[1], touch_y[1], touch_x[2], touch_y[2], touch_x[3], touch_y[3], touch_x[4], touch_y[4]);
    } 
    //if (touch_x[1] < 480 && touch_y[1] < 800){
        //printf("%d\t%d\t", touch_x[1], touch_y[1]);
    //} 
    //else {
    //    printf("x");
    //}
}

extern void touch_check(void){
    if (get_touch_coordinates_flag){
        get_touch_coordinates_flag = 0;
        //printf("getting coordinates\n");
        lcd_read_touch_coords();
    }
}

uint32_t rst_cnt = 0;

void lcd_init(void){
    InitLCDPorts();
    enable_10v_boost();
    lcd_config();
    lcd_interrupt_enable();
}


void lcd_config(void){
    lcd_param_t lcd_param;

    lcd_param.HBP = HBP_4DLCD;
    lcd_param.HFP = HFP_4DLCD;
    lcd_param.HSW = HSW_4DLCD;
    lcd_param.PPL = PPL_4DLCD;
    lcd_param.VBP = VBP_4DLCD;
    lcd_param.VFP = VFP_4DLCD;
    lcd_param.VSW = VSW_4DLCD;
    lcd_param.LPP = LPP_4DLCD;
    lcd_param.IOE = IOE_4DLCD;
    lcd_param.IPC = IPC_4DLCD;
    lcd_param.IHS = IHS_4DLCD;
    lcd_param.IVS = IVS_4DLCD;
    lcd_param.ACB = ACB_4DLCD;
    lcd_param.BPP = BPP_4DLCD;
    lcd_param.Clk = CLK_4DLCD;
    lcd_param.LCD = LCD_4DLCD;
    
    LPC_SC->PCONP |= (1UL << 0);  // Power the LCDC
    LPC_LCD->CTRL &= ~(1UL << 0); // Disable the LCDC
    LPC_LCD->TIMH = ((((lcd_param.PPL/16)-1))<<2) | (lcd_param.HSW-1)<<8 | (lcd_param.HFP-1)<<16 | (lcd_param.HBP-1)<<24;
    LPC_LCD->TIMV = (((lcd_param.LPP-1)&0x3FF)<<0) | (((lcd_param.VSW)&0x3F)<<10) | (lcd_param.VFP-1)<<16 | (lcd_param.VBP-1)<<24;
    LPC_LCD->POL  = ((lcd_param.Clk)<<0 | (lcd_param.IVS)<<11 | (lcd_param.IHS)<<12 | (lcd_param.IPC)<<13 | (lcd_param.PPL-1)<<16 | (lcd_param.BCD)<<26);
    LPC_LCD->CTRL = (lcd_param.BPP)<<1 | (lcd_param.LCD)<<5 | (0)<<8;
    LPC_SC->LCD_CFG = 0x0; // No panel clock prescaler
    LPC_LCD->UPBASE = (uint32_t)lcd_arr;//<<3;
    LPC_LCD->CTRL |= (0x1);
    LPC_LCD->CTRL |= (0x1 << 11);// Enable LCD signals
    //printf("LCD + SDRAM started\r\n");
}

void InitLCDPorts(void) {
    LPC_IOCON->P0_4  |= (0x7 | (0x1 << 9));  //  LCD_VD_0   R0
    LPC_IOCON->P0_5  |= (0x7 | (0x1 << 9));  //  LCD_VD_1   R1
    LPC_IOCON->P4_28 |= (0x7 | (0x1 << 9));  //  LCD_VD_2   R2
    LPC_IOCON->P4_29 |= (0x7 | (0x1 << 9));  //  LCD_VD_3   R3
    LPC_IOCON->P2_6  |= (0x7 | (0x1 << 9));  //  LCD_VD_4   R4
    LPC_IOCON->P2_7  |= (0x7 | (0x1 << 9));  //  LCD_VD_5   R5
    LPC_IOCON->P2_8  |= (0x7 | (0x1 << 9));  //  LCD_VD_6   R6
    LPC_IOCON->P2_9  |= (0x7 | (0x1 << 9));  //  LCD_VD_7   R7

    LPC_IOCON->P0_6  |= (0x7 | (0x1 << 9));  //  LCD_VD_8   G0
    LPC_IOCON->P0_7  |= (0x7 | (0x1 << 9));  //  LCD_VD_9   G1
    LPC_IOCON->P1_20 |= (0x7 | (0x1 << 9));  //  LCD_VD_10  G2
    LPC_IOCON->P1_21 |= (0x7 | (0x1 << 9));  //  LCD_VD_11  G3
    LPC_IOCON->P1_22 |= (0x7 | (0x1 << 9));  //  LCD_VD_12  G4
    LPC_IOCON->P1_23 |= (0x7 | (0x1 << 9));  //  LCD_VD_13  G5
    LPC_IOCON->P1_24 |= (0x7 | (0x1 << 9));  //  LCD_VD_14  G6
    LPC_IOCON->P1_25 |= (0x7 | (0x1 << 9));  //  LCD_VD_15  G7  

    LPC_IOCON->P0_8  |= (0x7 | (0x1 << 9));  //  LCD_VD_16  B0
    LPC_IOCON->P0_9  |= (0x7 | (0x1 << 9));  //  LCD_VD_17  B1
    LPC_IOCON->P2_12 |= (0x7 | (0x1 << 9));  //  LCD_VD_18  B2
    LPC_IOCON->P2_13 |= (0x7 | (0x1 << 9));  //  LCD_VD_19  B3
    LPC_IOCON->P1_26 |= (0x7 | (0x1 << 9));  //  LCD_VD_20  B4
    LPC_IOCON->P1_27 |= (0x7 | (0x1 << 9));  //  LCD_VD_21  B5
    LPC_IOCON->P1_28 |= (0x7 | (0x1 << 9));  //  LCD_VD_22  B6
    LPC_IOCON->P1_29 |= (0x7 | (0x1 << 9));  //  LCD_VD_23  B7

    LPC_IOCON->P1_7  |= (0x7 | (0x1 << 9));   // LCD_PWR
    LPC_IOCON->P2_0  |= (0x7 | (0x1 << 9));   // LCD_PWR
    LPC_IOCON->P2_2  |= (0x7 | (0x1 << 9));   // LCD_DCLK
    LPC_IOCON->P2_3  |= (0x7 | (0x1 << 9));   // LCD_FP VSYNC
    LPC_IOCON->P2_4  |= (0x7 | (0x1 << 9));   // LCD_ENAB_M DEN
    LPC_IOCON->P2_5  |= (0x7 | (0x1 << 9));   // LCD_LP HSYNC

    LPC_GPIO1->DIR |= (0x1 << 7);
}


void lcd_fill_screen(uint32_t color){
    uint32_t* ram_addr = 0xA0000000;
    for (uint32_t j=0; j<481; j++){
        for (uint32_t i=0; i<800; i++){
            *ram_addr = (uint32_t)0x0000FF00;
            ram_addr++;
        }
    }
}