#include "LCD.h"
#include "Power.h"
#include "FFT.h"
#include "I2C.h"
#include "RTC.h"
#include "Microphone.h"
#include "Colors.h"
#include "Fonts.h"
#include "arm_math.h"
#include <stdlib.h>



extern volatile unsigned char I2C_MasterBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned char I2C_SlaveBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned int I2C_Count[I2C_PORT_NUM];
extern volatile unsigned int I2C_ReadLength[I2C_PORT_NUM];
extern volatile unsigned int I2C_WriteLength[I2C_PORT_NUM];
extern volatile uint8_t get_touch_coordinates_flag;
extern volatile RTCTime local_time;
typedef uint32_t lcd_arr_t[PIXELS_COL];
extern float32_t fft_bin_output[TEST_LENGTH_SAMPLES/2];
uint16_t fft_bin_output_old[TEST_LENGTH_SAMPLES/2][2];

volatile uint8_t fft_scale_flag = 0;
extern volatile int32_t I2SRXBuffer[MEASUREMENTS_TO_TAKE];
uint16_t audio_signal_output_old[TEST_LENGTH_SAMPLES/2][2];


uint32_t *pFB32, col;
uint16_t touch_x[5], touch_y[5];
lcd_arr_t *lcd_arr = (lcd_arr_t*) 0xA0000000; // TODO was 0xA0000800;


void lcd_draw_time(void){
    char time_old[6]; 
    char time_new[6];
    sprintf(time_old, "%02d:%02d", local_time.RTC_Hour, local_time.RTC_Min); 
    local_time = RTCGetTime();
    sprintf(time_new, "%02d:%02d", local_time.RTC_Hour, local_time.RTC_Min);
    lcd_draw_text(time_old, 730,10,BLACK);
    lcd_draw_text(time_new, 730,10,GRAY_LIGHT);
}

void lcd_draw_text(char *string, uint16_t x, uint16_t y, uint32_t color){
    //uint16_t x = 300, y = 240;
    //uint32_t color = WHITE;
    //printf("printing %s at (%d,%d)\n", string, x, y);
    for (uint8_t l=0; l<strlen(string); l++){
        uint16_t box_w = 0, box_h = 0, ofs_x = 0, ofs_y = 0, ypo = 0, idx = 0, idx_len = 0, bit_idx = 0;
        uint16_t current_bitmap = 0, current_bit = 0, i = 0;
    
        //printf("%c %d %d\n", string[l], string[l], string[l]-31);
        idx = string[l]-31;
        idx_len = glyph_dsc[idx+1].bitmap_index - glyph_dsc[idx].bitmap_index;
        box_w = glyph_dsc[idx].box_w;
        box_h = glyph_dsc[idx].box_h;
        ofs_x = glyph_dsc[idx].ofs_x;
        ofs_y = glyph_dsc[idx].ofs_y;
        ypo = y + ofs_y;
        //printf("0x%02x\n", current_bitmap);
        while (i < idx_len){
            if (i < idx_len){
                current_bitmap = glyph_bitmap[glyph_dsc[idx].bitmap_index+i];
                //printf("0x%02x\n", current_bitmap);
                for (uint8_t j=0; j<box_h; j++){
                    for (uint8_t k=0; k<(box_w); k++){
                        if ((current_bitmap>>(7-bit_idx))&0x01){
                            lcd_draw_pixel(x+k, ypo+j, color);
                            //printf("1");
                        } else {
                            //printf("0");
                        }
                        bit_idx++;
                        if (bit_idx>7){
                            bit_idx = 0;
                            i++;
                            current_bitmap = glyph_bitmap[glyph_dsc[idx].bitmap_index+i];
                        }
                    }
                }
                if (idx = 68){
                    i++;
                }
            }
        }
        x += (box_w + ofs_x);
    }

}

void lcd_test(void){
    lcd_draw_line(50,50,100,400,2,0xFFFFFFFF);
    //delay_long();
    lcd_draw_line(60,50,110,400,2,RED);
    //delay_long();
    lcd_draw_line(70,50,120,400,2,GREEN);
    //delay_long();
    lcd_draw_line(80,50,130,400,2,BLUE);
    //delay_long();
    //lcd_draw_line(50,50,100,400,2,0xFFFFFFFF);
    //lcd_draw_line(50,200,400,200,0,0x00000000);
    //lcd_draw_text("AB!\"#");
    //lcd_draw_line(260,257,799,257,1,RED);
    //lcd_draw_text("0123456789");
}





void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color){
    lcd_arr[y][x] = color;
}


void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t thickness, uint32_t color){
    if (thickness < 1){
        thickness = 1;
    }
    uint32_t white = RGB(255,255,255);
    uint16_t x0_i = x0;
    uint16_t y0_i = y0;    
    int16_t dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int16_t err = dx + dy, e2; /* error value e_xy */ 
    //printf("dx: %x\ndy: %d\nerr: %d\n", dx, dy, err);
    uint16_t i, j;
    for (;;){  /* loop */
        for (i=0; i<thickness; i++){
            for (j=0; j<thickness; j++){
                lcd_draw_pixel(x0+i, y0+j, color);
            }
        }
        if (x0 == x1 && y0 == y1){
            if (i == thickness){
                break;
            } else {
                i++;
                x0 = x0_i;
                y0 = y0_i;
            }
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


void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill, uint8_t width, uint32_t color){
    if (fill){
        for (uint16_t i=x1; i<x2; i++){
            for (uint16_t j=y1; j<y2; j++){
                lcd_draw_pixel(j,i,color);
            }
                
        }
    } else {
        lcd_draw_line(x1,y1,x2,y1,width,color);
        lcd_draw_line(x1,y1,x1,y2,width,color);
        lcd_draw_line(x1,y2,x2,y2,width,color);
        lcd_draw_line(x2,y1,x2,y2,width,color);
    }
}


void lcd_draw_square(uint16_t x, uint16_t y, uint16_t thickness, uint32_t color){
    for (uint16_t i=x-thickness; i<x+thickness; i++){
        for (uint16_t j=y-thickness; j<y+thickness; j++){
            lcd_draw_pixel(j,i,color);
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
    lcd_touch_interrupt_enable();
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
    for (uint32_t j=0; j<480; j++){
        for (uint32_t i=0; i<800; i++){
            *ram_addr = (uint32_t)0x0000FF00;
            ram_addr++;
        }
    }
}


void lcd_draw_fft_graph(uint8_t init){
    if (init){
        lcd_draw_rectangle(FFT_GRAPH_ORIGIN_X,50,750,400,1,0,BLACK); // black fill
        lcd_draw_text("FFT",FFT_GRAPH_ORIGIN_X,FFT_GRAPH_TOP_PIXEL-19,GRAY);
    }
    lcd_draw_line(FFT_GRAPH_ORIGIN_X,FFT_GRAPH_TOP_PIXEL,FFT_GRAPH_ORIGIN_X,400,2,GRAY_DARK);    // y axis
    lcd_draw_line(FFT_GRAPH_ORIGIN_X,401,750,401,2,GRAY_DARK);  // x axis
    lcd_draw_line(750,FFT_GRAPH_TOP_PIXEL,750,401,2,GRAY_DARK);  // right side
    for (uint8_t i=10; i<17; i++){
        lcd_draw_line(FFT_GRAPH_ORIGIN_X,FFT_GRAPH_Y_GRID_SPACING*i,750,FFT_GRAPH_Y_GRID_SPACING*i,2,GRAY_DARK);  // major y axes
    }
}

void lcd_draw_fft_bins(float32_t max_val){
    float32_t scale;
    if (fft_scale_flag){
        scale = 148/(max_val);
    } else {
        scale = 1;
    }
    uint8_t width = 1;
    uint16_t x_coord_0, x_coord_1;
    uint16_t y_coord_0, y_coord_1;
    lcd_clear_fft_graph();
    //delay_long();
    for (uint16_t bin=0; bin<(TEST_LENGTH_SAMPLES/4-1); bin++){
        for (uint8_t i=0; i<width; i++){
            x_coord_0 = FFT_SIGNAL_ORIGIN_X+bin;
            x_coord_1 = x_coord_0+1;
            //float32_t bin_val = fft_bin_output[bin];
            y_coord_0 = floor(FFT_SIGNAL_ORIGIN_Y-(fft_bin_output[bin]*scale));//*scale);
            y_coord_1 = floor(FFT_SIGNAL_ORIGIN_Y-(fft_bin_output[bin+1]*scale));//*scale);
            
            if (y_coord_0 < FFT_GRAPH_TOP_PIXEL){
                y_coord_0 = FFT_GRAPH_TOP_PIXEL;
            }
            if (y_coord_1 < FFT_GRAPH_TOP_PIXEL){ 
                y_coord_1 = FFT_GRAPH_TOP_PIXEL;
            }
            if (x_coord_0 != 0 && y_coord_0 != 0 && x_coord_1 != 0 && y_coord_1 != 0){
                lcd_draw_line(x_coord_0, y_coord_0, x_coord_1, y_coord_1, 1, YELLOW);
            }
            fft_bin_output_old[bin][0] = x_coord_0;
            fft_bin_output_old[bin][1] = y_coord_0;
        }
    }
    fft_bin_output_old[512][0] = 750;
    fft_bin_output_old[512][1] = FFT_SIGNAL_ORIGIN_Y;
}


void lcd_clear_fft_graph(void){
    uint16_t x_coord_0, x_coord_1;
    uint16_t y_coord_0, y_coord_1;
    for (uint16_t bin=0; bin<(TEST_LENGTH_SAMPLES/4-1); bin++){
        x_coord_0 = fft_bin_output_old[bin][0];
        x_coord_1 = x_coord_0+1;
        //float32_t bin_val = fft_bin_output[bin];
        y_coord_0 = fft_bin_output_old[bin][1];//*scale);
        y_coord_1 = fft_bin_output_old[bin+1][1];//*scale);
        if (x_coord_0 != 0 && y_coord_0 != 0 && x_coord_1 != 0 && y_coord_1 != 0){
            lcd_draw_line(x_coord_0, y_coord_0, x_coord_1, y_coord_1, 1, RED);
        }
    }
}



void lcd_draw_audio_graph(uint8_t init){
    if (init){
        lcd_draw_rectangle(AUDIO_GRAPH_ORIGIN_X,49,750,200,1,0,BLACK); // black fill
        lcd_draw_text("SIGNAL",AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_TOP_PIXEL-19,GRAY);
    }
    lcd_draw_line(AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_TOP_PIXEL,AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_ORIGIN_Y,2,GRAY_DARK);    // y axis
    lcd_draw_line(AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_ORIGIN_Y,750,AUDIO_GRAPH_ORIGIN_Y,2,GRAY_DARK);  // x axis
    lcd_draw_line(750,AUDIO_GRAPH_TOP_PIXEL,750,AUDIO_GRAPH_ORIGIN_Y,2,GRAY_DARK);  // right side
    lcd_draw_line(AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_TOP_PIXEL,750,AUDIO_GRAPH_TOP_PIXEL,2,GRAY_DARK);  // x axis
    
    //for (uint8_t i=2; i<9; i++){
    //    lcd_draw_line(AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_Y_GRID_SPACING*i,750,AUDIO_GRAPH_Y_GRID_SPACING*i,2,GRAY_DARKER);  // major y axes
    //}
}


void lcd_draw_audio_signal(void){
    lcd_clear_audio_signal();
    uint8_t width = 1;
    uint16_t x_coord_0, x_coord_1;
    uint16_t y_coord_0, y_coord_1;
    audio_signal_output_old[511][0] = 749;
    audio_signal_output_old[511][1] = AUDIO_GRAPH_ORIGIN_Y;
    audio_signal_output_old[512][0] = 750;
    audio_signal_output_old[512][1] = AUDIO_GRAPH_ORIGIN_Y;
    for (uint16_t bin=150; bin<(TEST_LENGTH_SAMPLES/4-1)+150; bin++){
        for (uint8_t i=0; i<width; i++){
            x_coord_0 = AUDIO_SIGNAL_ORIGIN_X+bin-150;
            x_coord_1 = x_coord_0+1;
            //float32_t bin_val = fft_bin_output[bin];
            //printf("%d\n%d\n\n", AUDIO_SIGNAL_ORIGIN_Y-I2SRXBuffer[bin]/2048,
            //        AUDIO_SIGNAL_ORIGIN_Y-I2SRXBuffer[bin+1]/2048);
            y_coord_0 = floor((float)AUDIO_SIGNAL_ORIGIN_Y-((float)I2SRXBuffer[bin]/2048.0*6.0));//*scale);
            y_coord_1 = floor((float)AUDIO_SIGNAL_ORIGIN_Y-((float)I2SRXBuffer[bin+1]/2048.0*6.0));//*scale);
            
            if (y_coord_0 < 50 || y_coord_0 > 400){
                //y_coord_0 = AUDIO_SIGNAL_ORIGIN_Y;
                y_coord_0 = floor((float)AUDIO_SIGNAL_ORIGIN_Y-(((float)I2SRXBuffer[bin]-262143)/2048.0*6.0));
                //pr
            }
            if (y_coord_1 < 50 || y_coord_1 > 400){ 
                //y_coord_1 = AUDIO_SIGNAL_ORIGIN_Y;
                y_coord_1 = floor((float)AUDIO_SIGNAL_ORIGIN_Y-(((float)I2SRXBuffer[bin+1]-262143)/2048.0*6.0));
            }

            if (y_coord_0 < 50){
                y_coord_0 = 50;
            } else if (y_coord_0 > 200){
                y_coord_0 = 200;
            }

            if (y_coord_1 < 50){
                y_coord_1 = 50;
            } else if (y_coord_1 > 200){
                y_coord_1 = 200;
            }

            
            lcd_draw_line(x_coord_0, y_coord_0, x_coord_1, y_coord_1, 1, YELLOW);
            audio_signal_output_old[bin-150][0] = x_coord_0;
            audio_signal_output_old[bin-150][1] = y_coord_0;
        }
    }
}

void lcd_clear_audio_signal(void){
    uint16_t x_coord_0, x_coord_1;
    uint16_t y_coord_0, y_coord_1;
    for (uint16_t bin=0; bin<(TEST_LENGTH_SAMPLES/4-1); bin++){
        x_coord_0 = audio_signal_output_old[bin][0];
        x_coord_1 = x_coord_0+1;
        //float32_t bin_val = fft_bin_output[bin];
        y_coord_0 = audio_signal_output_old[bin][1];//*scale);
        y_coord_1 = audio_signal_output_old[bin+1][1];//*scale);
        lcd_draw_line(x_coord_0, y_coord_0, x_coord_1, y_coord_1, 1, BLACK);
    }
    lcd_draw_line(748,AUDIO_GRAPH_TOP_PIXEL+2,748,AUDIO_GRAPH_ORIGIN_Y-2,2,BLACK);
    //lcd_draw_audio_graph(0);
}


void lcd_fft_draw_buttons(void){
    // clear left side of screen
    lcd_draw_rectangle(0,0,450,200,1,0,BLACK);
    //erase fft button
    lcd_draw_rectangle(30,225,200,325,0,2,GRAY_DARK);
    lcd_draw_text("FFT SCALE",53,268,GRAY);
    //toggle scale button
    lcd_draw_rectangle(30,350,200,450,0,2,GRAY_DARK);
    lcd_draw_text("FFT ERASE",53,393,GRAY);
}