#include <stdio.h>
#include <LPC177x_8x.h>
#include "arm_math.h"

#define LCD_I2C_ADDRESS  0x38
#define LCD_REG_TOUCH_XL 0x38

#define HBP_4DLCD   (5U)    // Horizontal back porch in clocks
#define HFP_4DLCD   (40U)   // Horizontal front porch in clocks
#define HSW_4DLCD   (48U)   // HSYNC pulse width in clocks
#define PPL_4DLCD   (800U)  // Actual pixels per line
#define VBP_4DLCD   (31U)   // Vertical back porch in clocks
#define VFP_4DLCD   (200U)  // Vertical front porch in clocks
#define VSW_4DLCD   (3U)    // VSYNC pulse width in clocks
#define LPP_4DLCD   (480U)  // Lines per panel
#define IOE_4DLCD   0x0     // Invert output enable, 1 = invert
#define IPC_4DLCD   0x0     // Invert panel clock, 1 = invert
#define IHS_4DLCD   0x1     // Invert HSYNC, 1 = invert
#define IVS_4DLCD   0x1     // Invert VSYNC, 1 = invert
#define ACB_4DLCD   0x1     // AC bias frequency in clocks (not used)
#define BPP_4DLCD   0x5     // Bits per pixel b101 = 24 bpp 8:8:8 mode
#define BCD_4DLCD   0x1     // Bypass clock divider
#define CLK_4DLCD   0x3     // 1 ~ 50MHz, 2 ~ 25MHz  Optimal clock rate (Hz) according to datasheet
#define LCD_4DLCD   0x1     // Panel type; 1: LCD TFT panel
#define DUAL_4DLCD  0x0     // No dual panel

#define PIXELS_ROW  (480)
#define PIXELS_COL  (800)

#define FFT_GRAPH_ORIGIN_X (236)
#define FFT_GRAPH_ORIGIN_Y (401)
#define FFT_SIGNAL_ORIGIN_X (238)
#define FFT_SIGNAL_ORIGIN_Y (400)
#define FFT_GRAPH_TOP_PIXEL (250)
#define FFT_GRAPH_Y_GRID_SPACING (25)

#define AUDIO_GRAPH_ORIGIN_X (236)
#define AUDIO_GRAPH_ORIGIN_Y (202)
#define AUDIO_GRAPH_TOP_PIXEL (49)
#define AUDIO_SIGNAL_ORIGIN_X (238)
#define AUDIO_SIGNAL_ORIGIN_Y (125)
#define AUDIO_GRAPH_Y_GRID_SPACING (25)

enum {
    DISPLAY_TRULY_240_320,
    DISPLAY_BOARD_480_272,
    DISPLAY_BOARD_800_480
};


typedef struct {
    uint8_t  HBP;   // Horizontal back porch in clocks
    uint8_t  HFP;   // Horizontal front porch in clocks
    uint8_t  HSW;   // HSYNC pulse width in clocks
    uint16_t PPL;   // Pixels per line
    uint8_t  VBP;   // Vertical back porch in clocks
    uint8_t  VFP;   // Vertical front porch in clocks
    uint8_t  VSW;   // VSYNC pulse width in clocks
    uint16_t LPP;   // Lines per panel
    uint8_t  IOE;   // Invert output enable, 1 = invert
    uint8_t  IPC;   // Invert panel clock, 1 = invert
    uint8_t  IHS;   // Invert HSYNC, 1 = invert
    uint8_t  IVS;   // Invert VSYNC, 1 = invert
    uint8_t  ACB;   // AC bias frequency in clocks (not used)
    uint8_t  BPP;   // Maximum bits per pixel the display supports
    uint32_t BCD;   // Bypass clock divider
    uint32_t Clk;   // Optimal clock rate (Hz)
    uint8_t  LCD;   // LCD panel type
    uint8_t  Dual;  // Dual panel, 1 = dual panel display
    uint32_t FREQ;  // LCD Clk frequency
} lcd_param_t;


extern void delay_long(void);
void lcd_test(void);
void lcd_init(void);
void lcd_config(void);
void InitLCDPorts(void);
void lcd_fill_screen(uint32_t color);
void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t thickness, uint32_t color);
void lcd_draw_circle(int16_t xm, int16_t ym, int16_t r, uint32_t color);
void lcd_draw_square(uint16_t x, uint16_t y, uint16_t thickness, uint32_t color);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill, uint8_t width, uint32_t color);
void lcd_draw_text(char *string, uint16_t x, uint16_t y, uint32_t color);
void lcd_draw_time(void);
void lcd_block_test(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void lcd_draw_fft_graph(uint8_t init);
void lcd_draw_fft_bins(float32_t max_val);
void lcd_clear_fft_graph(void);
void lcd_draw_audio_graph(uint8_t init);
void lcd_process_audio_signal(void);
void lcd_draw_audio_signal(void);
void lcd_clear_audio_signal(void);
void lcd_fft_draw_buttons(void);