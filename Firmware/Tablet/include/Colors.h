
//#define RGB(r,g,b) (uint32_t)((r<<16) | (g<<24) | (b))
#define RGB(r,g,b) (uint32_t)((r) | (g<<8) | (b<<16))

//#define BLACK 0x00000000
//#define WHITE 0xFFFFFFFF
//#define RED   0x00FF0000
//#define GREEN 0xFF000000
//#define BLUE  0x000000FF
#define BLACK       RGB(0,0,0)
#define WHITE       RGB(255,255,255)
#define RED         RGB(255,0,0)
#define GREEN       RGB(0,255,0)
#define BLUE        RGB(0,0,255)
#define YELLOW      RGB(255,255,0)
#define CYAN        RGB(0,255,255)
#define MAGENTA     RGB(255,0,255)
#define GRAY_LIGHT  RGB(192,192,192)
#define GRAY        RGB(128,128,128)
#define GRAY_DARK   RGB(55,55,55)
#define GRAY_DARKER RGB(25,25,25)
#define MAROON      RGB(128,0,0)
#define ORANGE      RGB(255,100,0)
#define OLIVE       RGB(128,128,0)
#define GREEN_DARK  RGB(0,128,0)
#define PURPLE      RGB(128,0,128)
#define TEAL        RGB(0,128,128)
#define NAVY        RGB(0,0,128)
//#define  RGB
//#define  RGB