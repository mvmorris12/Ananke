#include "stdint.h"

#define SPI_INSTANCE  0 /**< SPI instance index. */

#define NRF_MISO      15
#define LCD_MOSI      14
#define LCD_DC	      12
#define LCD_CS	      0
#define LCD_RES	      11
#define LCD_CLK	      13

typedef enum
{
    SPI_FREQ_125KBPS = 0x02,           /*!< drive SClk with frequency 125Kbps */
    SPI_FREQ_250KBPS = 0x04,           /*!< drive SClk with frequency 250Kbps */
    SPI_FREQ_500KBPS = 0x08,           /*!< drive SClk with frequency 500Kbps */
    SPI_FREQ_1MBPS = 0x10,             /*!< drive SClk with frequency 1Mbps */
    SPI_FREQ_2MBPS = 0x20,             /*!< drive SClk with frequency 2Mbps */
    SPI_FREQ_4MBPS = 0x40,             /*!< drive SClk with frequency 4Mbps */
    SPI_FREQ_8MBPS = 0x80              /*!< drive SClk with frequency 8Mbps */
} SPI_frequency_t;

void ism_write(uint8_t reg, uint8_t value);
int ism_read(uint8_t reg);

