/**
 * Copyright (c) 2014 - 2020, Nordic Semiconductor ASA
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
#ifndef PCA10036_H
#define PCA10036_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"
#include "nrfx_saadc.h"

// LEDs definitions for Lagging sensor devboard
#define LEDS_NUMBER 2

#define LED_GREEN NRF_GPIO_PIN_MAP(0, 6)

#define LED_RED NRF_GPIO_PIN_MAP(0, 7)

#define LED_START LED_GREEN
#define LED_STOP LED_RED

#define LEDS_ACTIVE_STATE 1

#define LEDS_INV_MASK LEDS_MASK

#define LEDS_LIST \
  { LED_GREEN, LED_RED }

#define BSP_LED_0 LED_GREEN
#define BSP_LED_1 LED_RED

#define BUTTON_PULL NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 1

#define BUTTONS_LIST \
  {}

#define BUTTONS_NUMBER 1

/* Other pins */
#define VBATMEAS_ON_OFF NRF_GPIO_PIN_MAP(0, 11)

#define LOAD_ON_OFF NRF_GPIO_PIN_MAP(0, 13)

#define POWER_GOOD NRF_GPIO_PIN_MAP(0, 14)

#define ACC_IRQ NRF_GPIO_PIN_MAP(0, 15)
#define ACC_MISO NRF_GPIO_PIN_MAP(0, 16)
#define ACC_MOSI NRF_GPIO_PIN_MAP(0, 17)
#define ACC_SCK NRF_GPIO_PIN_MAP(0, 18)
#define ACC_SS NRF_GPIO_PIN_MAP(0, 19)

#define SWO NRF_GPIO_PIN_MAP(0, 20)

#define RESET NRF_GPIO_PIN_MAP(0, 21)

#define I2C_SCL NRF_GPIO_PIN_MAP(0, 22)

#define I2C_SDA NRF_GPIO_PIN_MAP(0, 23)

#define NOT_USED NRF_GPIO_PIN_MAP(0, 24)

#define RTC_EN NRF_GPIO_PIN_MAP(0, 25)

#define PWR_MONITOR_EN NRF_GPIO_PIN_MAP(0, 26)

#define ADC_ENABLE NRF_GPIO_PIN_MAP(0, 27)

//#define LOAD_DCDC_ON        12

//#define EN_DCDC             13
//#define EN_ZENER            14
//#define EN_BATTERY          15

//#define LOAD_ON             8


// for SIP32432
#define SIP_ENABLE 0
#define SIP_DISABLE 1

// ATTENTION: SIP32432 used on all boards
/* SIP used on board - set this to SIP32431_ON (CTC PCB) or SIP32432_ON (MGH PCB) */
//#define SIP3243x_ON SIP32432_ON
//#define SIP32431_ON 1
//#define SIP32432_ON 0
/* The rest is copied but not used yet */
//
//#define RX_PIN_NUMBER 8
//#define TX_PIN_NUMBER 6
//#define CTS_PIN_NUMBER 7
//#define RTS_PIN_NUMBER 5
//#define HWFC true
//
//#define SPIS_MISO_PIN 28 // SPI MISO signal.
//#define SPIS_CSN_PIN 12 // SPI CSN signal.
//#define SPIS_MOSI_PIN 25 // SPI MOSI signal.
//#define SPIS_SCK_PIN 29 // SPI SCK signal.
//
//#define SPIM0_SCK_PIN 29 // SPI clock GPIO pin number.
//#define SPIM0_MOSI_PIN 25 // SPI Master Out Slave In GPIO pin number.
//#define SPIM0_MISO_PIN 28 // SPI Master In Slave Out GPIO pin number.
//#define SPIM0_SS_PIN 12 // SPI Slave Select GPIO pin number.
//
//#define SPIM1_SCK_PIN 2 // SPI clock GPIO pin number.
//#define SPIM1_MOSI_PIN 3 // SPI Master Out Slave In GPIO pin number.
//#define SPIM1_MISO_PIN 4 // SPI Master In Slave Out GPIO pin number.
//#define SPIM1_SS_PIN 5 // SPI Slave Select GPIO pin number.
//
//#define SPIM2_SCK_PIN 12 // SPI clock GPIO pin number.
//#define SPIM2_MOSI_PIN 13 // SPI Master Out Slave In GPIO pin number.
//#define SPIM2_MISO_PIN 14 // SPI Master In Slave Out GPIO pin number.
//#define SPIM2_SS_PIN 15 // SPI Slave Select GPIO pin number.
//
//// serialization APPLICATION board - temp. setup for running serialized MEMU tests
//#define SER_APP_RX_PIN 23 // UART RX pin number.
//#define SER_APP_TX_PIN 24 // UART TX pin number.
//#define SER_APP_CTS_PIN 2 // UART Clear To Send pin number.
//#define SER_APP_RTS_PIN 25 // UART Request To Send pin number.
//
//#define SER_APP_SPIM0_SCK_PIN 27 // SPI clock GPIO pin number.
//#define SER_APP_SPIM0_MOSI_PIN 2 // SPI Master Out Slave In GPIO pin number
//#define SER_APP_SPIM0_MISO_PIN 26 // SPI Master In Slave Out GPIO pin number
//#define SER_APP_SPIM0_SS_PIN 23 // SPI Slave Select GPIO pin number
//#define SER_APP_SPIM0_RDY_PIN 25 // SPI READY GPIO pin number
//#define SER_APP_SPIM0_REQ_PIN 24 // SPI REQUEST GPIO pin number
//
//// serialization CONNECTIVITY board
//#define SER_CON_RX_PIN 24 // UART RX pin number.
//#define SER_CON_TX_PIN 23 // UART TX pin number.
//#define SER_CON_CTS_PIN 25 // UART Clear To Send pin number. Not used if HWFC is set to false.
//#define SER_CON_RTS_PIN 2 // UART Request To Send pin number. Not used if HWFC is set to false.
//
//#define SER_CON_SPIS_SCK_PIN 27 // SPI SCK signal.
//#define SER_CON_SPIS_MOSI_PIN 2 // SPI MOSI signal.
//#define SER_CON_SPIS_MISO_PIN 26 // SPI MISO signal.
//#define SER_CON_SPIS_CSN_PIN 23 // SPI CSN signal.
//#define SER_CON_SPIS_RDY_PIN 25 // SPI READY GPIO pin number.
//#define SER_CON_SPIS_REQ_PIN 24 // SPI REQUEST GPIO pin number.
//
//#define SER_CONN_CHIP_RESET_PIN 11 // Pin used to reset connectivity chip
//
//// Arduino board mappings
//#define ARDUINO_SCL_PIN 27 // SCL signal pin
//#define ARDUINO_SDA_PIN 26 // SDA signal pin
//#define ARDUINO_AREF_PIN 2 // Aref pin
//#define ARDUINO_13_PIN 25 // Digital pin 13
//#define ARDUINO_12_PIN 24 // Digital pin 12
//#define ARDUINO_11_PIN 23 // Digital pin 11
//#define ARDUINO_10_PIN 22 // Digital pin 10
//#define ARDUINO_9_PIN 20 // Digital pin 9
//#define ARDUINO_8_PIN 19 // Digital pin 8
//
//#define ARDUINO_7_PIN 18 // Digital pin 7
//#define ARDUINO_6_PIN 17 // Digital pin 6
//#define ARDUINO_5_PIN 16 // Digital pin 5
//#define ARDUINO_4_PIN 15 // Digital pin 4
//#define ARDUINO_3_PIN 14 // Digital pin 3
//#define ARDUINO_2_PIN 13 // Digital pin 2
//#define ARDUINO_1_PIN 12 // Digital pin 1
//#define ARDUINO_0_PIN 11 // Digital pin 0
//
//#define ARDUINO_A0_PIN 3 // Analog channel 0
//#define ARDUINO_A1_PIN 4 // Analog channel 1
//#define ARDUINO_A2_PIN 28 // Analog channel 2
//#define ARDUINO_A3_PIN 29 // Analog channel 3
//#define ARDUINO_A4_PIN 30 // Analog channel 4
//#define ARDUINO_A5_PIN 31 // Analog channel 5

#ifdef __cplusplus
}
#endif

#endif // PCA10036_H