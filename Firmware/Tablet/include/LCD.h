#include <stdio.h>
#include <LPC177x_8x.h>

#define LCD_I2C_ADDRESS  0x38
#define LCD_REG_TOUCH_XL 0x38


#define HBP_4DLCD   (5U)   // Horizontal back porch in clocks
#define HFP_4DLCD   (40U)   // Horizontal front porch in clocks
#define HSW_4DLCD   (48U)    // HSYNC pulse width in clocks
#define PPL_4DLCD   (800U)  // Actual pixels per line
//#define PPL_4DLCD   (100U)  // Pixels per line
#define VBP_4DLCD   (31U)   // Vertical back porch in clocks
#define VFP_4DLCD   (200U)   // Vertical front porch in clocks
#define VSW_4DLCD   (3U)    // VSYNC pulse width in clocks
#define LPP_4DLCD   (480U)   // Lines per panel
//#define LPP_4DLCD   (10U)   // Lines per panel
#define IOE_4DLCD   0x0     // Invert output enable, 1 = invert
#define IPC_4DLCD   0x0     // Invert panel clock, 1 = invert
#define IHS_4DLCD   0x1     // Invert HSYNC, 1 = invert
#define IVS_4DLCD   0x1     // Invert VSYNC, 1 = invert
#define ACB_4DLCD   0x1     // AC bias frequency in clocks (not used)
#define BPP_4DLCD   0x5     // Bits per pixel b101 = 24 bpp 8:8:8 mode
#define BCD_4DLCD   0x1     // Bypass clock divider
//#define CLK_4DLCD   0x01     // 1 ~ 50MHz, 2 ~ 25MHz  Optimal clock rate (Hz) according to datasheet
#define CLK_4DLCD   0x3     // 1 ~ 50MHz, 2 ~ 25MHz  Optimal clock rate (Hz) according to datasheet
#define LCD_4DLCD   0x1     // Panel type; 1: LCD TFT panel
#define DUAL_4DLCD  0x0     // No dual panel


enum {
    DISPLAY_TRULY_240_320,
    DISPLAY_BOARD_480_272,
    DISPLAY_BOARD_800_480
};


typedef struct {
uint8_t  HBP; // Horizontal back porch in clocks
uint8_t  HFP; // Horizontal front porch in clocks
uint8_t  HSW; // HSYNC pulse width in clocks
uint16_t PPL; // Pixels per line
uint8_t  VBP; // Vertical back porch in clocks
uint8_t  VFP; // Vertical front porch in clocks
uint8_t  VSW; // VSYNC pulse width in clocks
uint16_t LPP; // Lines per panel
uint8_t  IOE; // Invert output enable, 1 = invert
uint8_t  IPC; // Invert panel clock, 1 = invert
uint8_t  IHS; // Invert HSYNC, 1 = invert
uint8_t  IVS; // Invert VSYNC, 1 = invert
uint8_t  ACB; // AC bias frequency in clocks (not used)
uint8_t  BPP; // Maximum bits per pixel the display supports
uint32_t  BCD; // Bypass clock divider
uint32_t Clk; // Optimal clock rate (Hz)
uint8_t  LCD; // LCD panel type
uint8_t  Dual; // Dual panel, 1 = dual panel display
uint32_t FREQ; // LCD Clk frequency
} lcd_param_t;


extern void delay_long(void);
void lcd_init(void);
void lcd_config(void);
void InitLCDPorts(void);
void lcd_fill_screen(uint32_t color);
void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t val);
void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void lcd_draw_circle(int16_t xm, int16_t ym, int16_t r, uint32_t color);