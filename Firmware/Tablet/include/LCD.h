#include <stdio.h>
#include <LPC177x_8x.h>
#include "arm_math.h"

#define LCD_I2C_ADDRESS  0x38
#define LCD_REG_TOUCH_XL 0x38

#define HBP_4DLCD   (50U)    // Horizontal back porch in clocks
#define HFP_4DLCD   (40U)   // Horizontal front porch in clocks
#define HSW_4DLCD   (48U)   // HSYNC pulse width in clocks
#define PPL_4DLCD   (800U)  // Actual pixels per line
#define VBP_4DLCD   (10U)   // Vertical back porch in clocks
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
#define AUDIO_GRAPH_X_END (786)
#define AUDIO_GRAPH_ORIGIN_Y (202)
#define AUDIO_GRAPH_TOP_PIXEL (49)
#define AUDIO_SIGNAL_ORIGIN_X (238)
#define AUDIO_SIGNAL_ORIGIN_Y (150)
#define AUDIO_GRAPH_Y_GRID_SPACING (25)

#define BUTTON_FFT_SPEECH_X1 (30)
#define BUTTON_FFT_SPEECH_Y1 (16)
#define BUTTON_FFT_SPEECH_X2 (200)
#define BUTTON_FFT_SPEECH_Y2 (116)
#define BUTTON_FFT_SPEECH_TXT_X (81)
#define BUTTON_FFT_SPEECH_TXT_Y (BUTTON_FFT_SPEECH_Y2 - (BUTTON_FFT_SPEECH_Y2 - BUTTON_FFT_SPEECH_Y1)/2) - 7

#define BUTTON_FFT_SCALE_X1 (30)
#define BUTTON_FFT_SCALE_Y1 (248)
#define BUTTON_FFT_SCALE_X2 (200)
#define BUTTON_FFT_SCALE_Y2 (348)
#define BUTTON_FFT_SCALE_TXT_X (53)
#define BUTTON_FFT_SCALE_TXT_Y (BUTTON_FFT_SCALE_Y2 - (BUTTON_FFT_SCALE_Y2 - BUTTON_FFT_SCALE_Y1)/2) - 7

#define BUTTON_FFT_ERASE_X1 (30)
#define BUTTON_FFT_ERASE_Y1 (364)
#define BUTTON_FFT_ERASE_X2 (200)
#define BUTTON_FFT_ERASE_Y2 (465)
#define BUTTON_FFT_ERASE_TXT_X (53)
#define BUTTON_FFT_ERASE_TXT_Y (BUTTON_FFT_ERASE_Y2 - (BUTTON_FFT_ERASE_Y2 - BUTTON_FFT_ERASE_Y1)/2) - 7

#define BUTTON_PAUSE_X1 (30)
#define BUTTON_PAUSE_Y1 (132)
#define BUTTON_PAUSE_X2 (200)
#define BUTTON_PAUSE_Y2 (232)
#define BUTTON_PAUSE_TXT_X (81)
#define BUTTON_PAUSE_TXT_Y (BUTTON_PAUSE_Y2 - (BUTTON_PAUSE_Y2 - BUTTON_PAUSE_Y1)/2) - 7

#define BUTTON_SPEECH_RECORD_X1 (30)
#define BUTTON_SPEECH_RECORD_Y1 (350)
#define BUTTON_SPEECH_RECORD_X2 (200)
#define BUTTON_SPEECH_RECORD_Y2 (450)
#define BUTTON_SPEECH_RECORD_TXT_X (78)
#define BUTTON_SPEECH_RECORD_TXT_Y (393)

#define BUTTON_SPEECH_TO_FFT_X1 (685)
#define BUTTON_SPEECH_TO_FFT_Y1 (50)
#define BUTTON_SPEECH_TO_FFT_X2 (785)
#define BUTTON_SPEECH_TO_FFT_Y2 (100)
#define BUTTON_SPEECH_TO_FFT_TXT_X (720)
#define BUTTON_SPEECH_TO_FFT_TXT_Y (68)

#define SPEECH_GRAPH_RED_X1 (10)
#define SPEECH_GRAPH_RED_Y1 (10)
#define SPEECH_GRAPH_RED_X2 (268)
#define SPEECH_GRAPH_RED_Y2 (155)
#define SPEECH_GRAPH_RED_Y_ORIGIN (73)

#define SPEECH_GRAPH_GREEN_X1 (10)
#define SPEECH_GRAPH_GREEN_Y1 (165)
#define SPEECH_GRAPH_GREEN_X2 (268)
#define SPEECH_GRAPH_GREEN_Y2 (310)
#define SPEECH_GRAPH_GREEN_Y_ORIGIN (228)

#define SPEECH_GRAPH_BLUE_X1 (10)
#define SPEECH_GRAPH_BLUE_Y1 (320)
#define SPEECH_GRAPH_BLUE_X2 (268)
#define SPEECH_GRAPH_BLUE_Y2 (465)
#define SPEECH_GRAPH_BLUE_Y_ORIGIN (383)

#define SPEECH_GRAPH_TEST_X1 (348)
#define SPEECH_GRAPH_TEST_Y1 (320)
#define SPEECH_GRAPH_TEST_X2 (606)
#define SPEECH_GRAPH_TEST_Y2 (465)
#define SPEECH_GRAPH_TEST_Y_ORIGIN (383)

#define SPEECH_BUTTON_MICROPHONE_X1 (685)
#define SPEECH_BUTTON_MICROPHONE_Y1 (365)
#define SPEECH_BUTTON_MICROPHONE_X2 (785)
#define SPEECH_BUTTON_MICROPHONE_Y2 (465)


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

//typedef struct {
//    typedef  struct {
//        uint8_t active;
//     } fft;
//    typedef  struct {
//        uint8_t active;
//     } speech;
//    typedef  struct {
//        uint8_t active;
//     } physics;
//    typedef  struct {
//        uint8_t active;
//     } cube;
//} app_state_t;


extern void delay_long(void);
void touch_check(void);
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
void lcd_clear_screen(void);
void lcd_draw_fft_graph(uint8_t init);
void lcd_draw_fft_bins(float32_t max_val);
void lcd_clear_fft_graph(void);

void lcd_draw_audio_graph(uint8_t init);
void lcd_process_audio_signal(void);
void lcd_draw_audio_signal(void);
void lcd_clear_audio_signal(void);

void lcd_fft_draw_buttons(void);
void lcd_toggle_fft_scale_button(void);
void lcd_toggle_pause_button(void);
void lcd_speech_draw_buttons(void);

void lcd_draw_speech_models(void);
void lcd_draw_speech_buttons(void);
void lcd_toggle_record_button(void);
void lcd_speech_draw_test_signal(void);
void lcd_speech_clear_test_signal(void);
