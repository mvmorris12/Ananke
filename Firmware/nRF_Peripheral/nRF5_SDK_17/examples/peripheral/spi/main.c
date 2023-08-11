/**
 * Copyright (c) 2015 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_gpiote.h"

#define ACC_IRQ NRF_GPIO_PIN_MAP(0, 11)
#define ACC_MISO NRF_GPIO_PIN_MAP(0, 12)
#define ACC_MOSI NRF_GPIO_PIN_MAP(0, 15)
#define ACC_SCK NRF_GPIO_PIN_MAP(0, 16) //18
#define ACC_SS NRF_GPIO_PIN_MAP(0, 17)

#define SPI_READ_BIT                0x80
#define SPI_WRITE_BIT               0x7F
#define ISM330_REG_WHOAMI           0x0F
#define ISM330_REG_FIFO_CTRL1       0x06
#define ISM330_REG_FIFO_CTRL2       0x07
#define ISM330_REG_FIFO_CTRL3       0x08
#define ISM330_REG_FIFO_CTRL4       0x09
#define ISM330_REG_FIFO_CTRL5       0x0A
#define ISM330_REG_CTRL3_C          0x12
#define ISM330_REG_CTRL4_C          0x13
#define ISM330_REG_CTRL5_C          0x14
#define ISM330_REG_CTRL6_C          0x15
#define ISM330_REG_INT1_CTRL        0x0D
#define ISM330_REG_FIFO_STATUS1     0x3A
#define ISM330_REG_FIFO_STATUS2     0x3B
#define ISM330_REG_CTRL1_XL         0x10
#define ISM330_REG_CTRL9_XL         0x18
#define ISM330_REG_FIFO_DATA_OUT_L  0x3E
#define ISM330_REG_FIFO_DATA_OUT_H  0x3F
#define ISM330_REG_OUTX_L_XL        0x28
#define ISM330_REG_OUTX_H_XL        0x29

#define ISM330_VAL_FIFO_ODR_12_5HZ  0b0001
#define ISM330_VAL_FIFO_ODR_26HZ    0b0010
#define ISM330_VAL_FIFO_ODR_52HZ    0b0011
#define ISM330_VAL_FIFO_ODR_104HZ   0b0100
#define ISM330_VAL_FIFO_ODR_208HZ   0b0101
#define ISM330_VAL_FIFO_ODR_416HZ   0b0110
#define ISM330_VAL_FIFO_ODR_833HZ   0b0111
#define ISM330_VAL_FIFO_ODR_1_66KHZ 0b1000
#define ISM330_VAL_FIFO_ODR_3_33KHZ 0b1001
#define ISM330_VAL_FIFO_ODR_6_66KHZ 0b1010
#define ISM330_VAL_FIFO             0b001
#define ISM330_VAL_CONTINUOUS       0b011
#define ISM330_VAL_BYPASS           0b100
#define ISM330_VAL_CONTINUOUS_OVRW  0b110
#define ISM330_VAL_2G               0b00
#define ISM330_VAL_4G               0b10
#define ISM330_VAL_8G               0b11
#define ISM330_VAL_16G              0b01

#define ISM330_SET_FIFO_MODE        ISM330_VAL_FIFO
#define ISM330_SET_FIFO_ODR         ISM330_VAL_FIFO_ODR_6_66KHZ
#define ISM330_SET_ACC_G            ISM330_VAL_2G       

#define SPI_INSTANCE                0 /**< SPI instance index. */

#define LED_PIN_RED                 10
#define LED_PIN_GREEN               9

#define PIN_IN                      11
#define PIN_OUT                     27


int interrupt_count = 0;

uint16_t fifo_data[692];

static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

#define TEST_STRING "Nordic"
volatile uint8_t       m_tx_buf[10];// = TEST_STRING;           /**< TX buffer. */
uint8_t       m_rx_buf[4];    /**< RX buffer. */
volatile uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */

/**
 * @brief SPI user event handler.
 * @param event
 */

 
int cnt=0;
bool ism_fifo_full = false;

void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    spi_xfer_done = true;
    //NRF_LOG_INFO("Transfer completed.");
    //if (m_rx_buf[1] != 0)
    //{
        //NRF_LOG_INFO("%d\t Received: 0x%02x", cnt++, m_rx_buf[1]);
        //NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));
        //m_rx_buf[1] = 0x00;
        //if (m_rx_buf[1] == 0x27){
        //    ism_fifo_full = true;
        //}
    //}
}

void ism_read(uint8_t reg){
    nrf_delay_ms(2);
    memset(m_rx_buf, 0, m_length);
    spi_xfer_done = false;
    uint8_t test_rx[] = {(SPI_READ_BIT | reg), 0x00};
    m_length = sizeof(test_rx);
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, test_rx, m_length, m_rx_buf, m_length));
}

void ism_write(uint8_t reg, uint8_t data){
    //NRF_LOG_INFO("writing 0x%02x to reg 0x%02x", data, reg);    
    while (!spi_xfer_done)
    {
        __WFE();
    }
    memset(m_rx_buf, 0, m_length);
    spi_xfer_done = false;
    uint8_t test_rx[] = {(SPI_WRITE_BIT & reg), data};
    m_length = sizeof(test_rx);
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, test_rx, (m_length), m_rx_buf, m_length));
}

int intr = 0;

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    NRF_LOG_INFO("acc irq received\t%d", intr++);
    ism_fifo_full = true;
    nrf_gpio_pin_clear(LED_PIN_RED);
    nrf_gpio_pin_set(LED_PIN_GREEN);
    nrf_delay_ms(25);
    nrf_gpio_pin_clear(LED_PIN_GREEN);
    nrf_drv_gpiote_out_toggle(PIN_OUT);
    //nrf_delay_ms(100);
    //NRF_LOG_INFO("done");
}

void gpiote_init(void)
{

    ret_code_t err_code;
    NRF_LOG_INFO("gpiote init start");

    nrf_drv_gpiote_init();
    //APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);

    err_code = nrf_drv_gpiote_out_init(PIN_OUT, &out_config);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;
    in_config.sense = GPIOTE_CONFIG_POLARITY_LoToHi;
    in_config.is_watcher = false;
    in_config.hi_accuracy = false;

    err_code = nrf_drv_gpiote_in_init(PIN_IN, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);

}

void gpiote_uninit(void){
    nrf_drv_gpiote_uninit();
}

void gpiote_start(void){
    nrf_drv_gpiote_in_event_enable(PIN_IN, true);
}

void gpiote_stop(void){
    nrf_drv_gpiote_in_event_disable(PIN_IN);
}

void ism_init(void){
    nrf_gpio_cfg_output(19);
    nrf_gpio_pin_set(19);
    ism_write(ISM330_REG_CTRL1_XL, ((ISM330_SET_FIFO_ODR << 4) | (ISM330_SET_ACC_G << 2)));
    ism_write(ISM330_REG_FIFO_CTRL1, 0b11111111);
    ism_write(ISM330_REG_FIFO_CTRL2, 0b00000111);
    ism_write(ISM330_REG_FIFO_CTRL3, 0b00000001);
    ism_write(ISM330_REG_FIFO_CTRL4, 0b00000000);
    ism_write(ISM330_REG_CTRL3_C, 0b01000100);
    ism_write(ISM330_REG_CTRL4_C, 0b10010100);
    ism_write(ISM330_REG_CTRL5_C, 0b00000000);
    ism_write(ISM330_REG_INT1_CTRL, 0b00100000);
    ism_write(ISM330_REG_CTRL9_XL, 0b10000000);
    ism_read(ISM330_REG_CTRL3_C);
    ism_read(ISM330_REG_CTRL4_C);
    ism_read(ISM330_REG_CTRL5_C);
    ism_read(ISM330_REG_FIFO_CTRL1);
    ism_read(ISM330_REG_FIFO_CTRL2);
    ism_read(ISM330_REG_FIFO_CTRL5);
    ism_read(ISM330_REG_INT1_CTRL);
    ism_read(ISM330_REG_CTRL9_XL);
    ism_write(ISM330_REG_FIFO_CTRL5, ((ISM330_SET_FIFO_ODR << 4) | (0x00)));
    ism_write(ISM330_REG_FIFO_CTRL5, ((ISM330_SET_FIFO_ODR << 4) | (ISM330_SET_FIFO_MODE)));
    nrf_gpio_pin_set(LED_PIN_RED);
    
    //write 000 to fifo_ctrl5
    //write 001 to fifo_ctrl5
}


void ism_remaining_fifo_samples(void){
    //NRF_LOG_INFO("reading # of samples left in fifo buffer..");
    uint8_t test_rx[] = {(SPI_READ_BIT | ISM330_REG_FIFO_STATUS1), 0x00, 0x00};
    m_length = sizeof(test_rx);
    memset(m_rx_buf, 0, m_length);
    spi_xfer_done = false;
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, test_rx, m_length, m_rx_buf, m_length));
    while (!spi_xfer_done)
    {
        __WFE();
    }
    printf("0x%02x 0x%02x : %d\n", m_rx_buf[2], m_rx_buf[1], (((m_rx_buf[2] & 0x07) << 8) | (m_rx_buf[1])));    
}



void ism_read_fifo(void){
    NRF_LOG_INFO("tabulating FIFO buffer..");
    uint8_t test_rx[] = {(SPI_READ_BIT | ISM330_REG_FIFO_DATA_OUT_L), 0x00, 0x00};
    m_length = sizeof(test_rx);
    uint8_t count = 0;
    uint16_t idx = 0;
    for (uint16_t i=0; i<2045; i++){
        count++;
        memset(m_rx_buf, 0, m_length);
        spi_xfer_done = false;
        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, test_rx, m_length, m_rx_buf, m_length));
        while (!spi_xfer_done)
        {
            __WFE();
        }
        if (count == 1){
            fifo_data[idx] = (uint16_t*)((m_rx_buf[2] << 8) | (m_rx_buf[1]));
            NRF_LOG_INFO("idx: %d  \tfifo_data[]: %d \tval: %d", idx, fifo_data[idx++], ((m_rx_buf[2] << 8) | (m_rx_buf[1])));
        } else if (count == 3){
            count = 0;
        }
        //ism_remaining_fifo_samples();
    }
}

//void ism_test_read(){
//    NRF_LOG_INFO("reading accelerometer..");
//    uint8_t test_rx[] = {(SPI_READ_BIT | ISM330_REG_OUTX_L_XL), 0x00, 0x00};
//    m_length = sizeof(test_rx);
//    for (uint16_t i=0; i<2048; i++){
//        memset(m_rx_buf, 0, m_length);
//        spi_xfer_done = false;
//        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, test_rx, m_length, m_rx_buf, m_length));
//        while (!spi_xfer_done)
//        {
//            __WFE();
//        }
//        printf("0x%02x 0x%02x\n", m_rx_buf[2], m_rx_buf[1]);
//        nrf_delay_ms(1000);
//    }
//}

void led_init(void){
    nrf_gpio_cfg_output(LED_PIN_GREEN);
    nrf_gpio_cfg_output(LED_PIN_RED);
}


int main(void)
{
    bsp_board_init(BSP_INIT_LEDS);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = ACC_SS; //31
    spi_config.miso_pin = ACC_MISO; //30
    spi_config.mosi_pin = ACC_MOSI; //29
    spi_config.sck_pin  = ACC_SCK; //26
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));

    gpiote_init();
    gpiote_start();
    NRF_LOG_FLUSH();

    led_init();

    NRF_LOG_INFO("SPI example started.");
    ism_read(ISM330_REG_WHOAMI);
    ism_init();
    memset(fifo_data, 1, sizeof(uint16_t));
    while (1)
    {
        // Reset rx buffer and transfer done flag
        ism_read(ISM330_REG_FIFO_STATUS2);

        while (!spi_xfer_done)
        {
            __WFE();
        }
        if (ism_fifo_full){ 
            ism_read_fifo();
            ism_write(ISM330_REG_FIFO_CTRL5, (ISM330_SET_FIFO_ODR << 4));
            ism_write(ISM330_REG_FIFO_CTRL5, ((ISM330_SET_FIFO_ODR << 4) | (ISM330_SET_FIFO_MODE)));
            nrf_gpio_pin_set(LED_PIN_RED);
            ism_fifo_full = false;
        }    

        //ism_test_read();

        //bsp_board_led_invert(BSP_BOARD_LED_0);
        //nrf_delay_ms(1000);
    }
}
