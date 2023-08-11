#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "nrf_pwr_mgmt.h"
#include "app_timer.h"
#include "boards.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_db_discovery.h"
#include "ble_lbs_c.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_scan.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_delay.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_timer.h"

#include "nrf_drv_rtc.h"

//#include "C:\Users\mimorris\Documents\Sensors\LaggingWear\Firmware\laggingwear-\nRF5_SDK_17\components\libraries\simple_timer\app_simple_timer.h"

#include "nrf_drv_spi.h"

#define SYNC_INTERRUPT_PIN        31U


#define CENTRAL_SCANNING_LED            BSP_BOARD_LED_0                     /**< Scanning LED will be on when the device is scanning. */
#define CENTRAL_CONNECTED_LED           BSP_BOARD_LED_1                     /**< Connected LED will be on when the device is connected. */
#define LEDBUTTON_LED                   BSP_BOARD_LED_2                     /**< LED to indicate a change of state of the the Button characteristic on the peer. */

#define SCAN_INTERVAL                   0x0AA0                              /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                     0x0050                              /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_DURATION                   0x0000                              /**< Timout when scanning. 0x0000 disables timeout. */

#define MIN_CONNECTION_INTERVAL         MSEC_TO_UNITS(7.5, UNIT_1_25_MS)    /**< Determines minimum connection interval in milliseconds. */
#define MAX_CONNECTION_INTERVAL         MSEC_TO_UNITS(1000, UNIT_1_25_MS)     /**< Determines maximum connection interval in milliseconds. */
#define SLAVE_LATENCY                   0                                   /**< Determines slave latency in terms of connection events. */
#define SUPERVISION_TIMEOUT             MSEC_TO_UNITS(4000, UNIT_10_MS)     /**< Determines supervision time-out in units of 10 milliseconds. */

#define LEDBUTTON_BUTTON_PIN            BSP_BUTTON_0                        /**< Button that will write to the LED characteristic of the peer */
#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50)                 /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define APP_BLE_CONN_CFG_TAG            1                                   /**< A tag identifying the SoftDevice BLE configuration. */
#define APP_BLE_OBSERVER_PRIO           3                                   /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define COMPARE_COUNTERTIME  (3UL)  
#define COMPARE_EVENT_OUTPUT   26


#define PIN_IN_COMPARATOR_IRQ           31
#define PIN_OUT                         27  // not used for anything yet

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

#define PIN_IN_ACC_IRQ              11

#define ACC_IRQ NRF_GPIO_PIN_MAP(0, 11)
#define ACC_MISO NRF_GPIO_PIN_MAP(0, 12)
#define ACC_MOSI NRF_GPIO_PIN_MAP(0, 15)
#define ACC_SCK NRF_GPIO_PIN_MAP(0, 16) //18
#define ACC_SS NRF_GPIO_PIN_MAP(0, 17)





uint16_t fifo_data[692];

static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

#define TEST_STRING "Nordic"
volatile uint8_t       m_tx_buf[10];// = TEST_STRING;           /**< TX buffer. */
uint8_t       m_rx_buf[4];    /**< RX buffer. */
volatile uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */



NRF_BLE_SCAN_DEF(m_scan);                                       /**< Scanning module instance. */
BLE_LBS_C_DEF(m_ble_lbs_c);                                     /**< Main structure used by the LBS client module. */
NRF_BLE_GATT_DEF(m_gatt);                                       /**< GATT module instance. */
BLE_DB_DISCOVERY_DEF(m_db_disc);                                /**< DB discovery module instance. */
NRF_BLE_GQ_DEF(m_ble_gatt_queue,                                /**< BLE GATT Queue instance. */
               NRF_SDH_BLE_CENTRAL_LINK_COUNT,
               NRF_BLE_GQ_QUEUE_SIZE);

static char const m_target_periph_name[] = "BLE_SYNC_P";     /**< Name of the device we try to connect to. This name is searched in the scan report data*/

int interrupt_count = 0;

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(2); /**< Declaring an instance of nrf_drv_rtc for RTC2. */

volatile bool sync_time_flag = false;

APP_TIMER_DEF(sync_timer_id); 









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
    //nrf_delay_ms(2);
    memset(m_rx_buf, 0, m_length);
    spi_xfer_done = false;
    uint8_t test_rx[] = {(SPI_READ_BIT | reg), 0x00};
    m_length = sizeof(test_rx);
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, test_rx, m_length, m_rx_buf, m_length));
}

void ism_write(uint8_t reg, uint8_t data){
    //NRF_LOG_INFO("writing 0x%02x to reg 0x%02x", data, reg);    
    //while (!spi_xfer_done)
    //{
    //    //__WFE();
    //}
    memset(m_rx_buf, 0, m_length);
    spi_xfer_done = false;
    uint8_t test_rx[] = {(SPI_WRITE_BIT & reg), data};
    m_length = sizeof(test_rx);
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, test_rx, (m_length), m_rx_buf, m_length));
}

int intr = 0;

void in_pin_handler_acc(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    NRF_LOG_INFO("acc irq received\t%d", intr++);
    ism_fifo_full = true;
    nrf_gpio_pin_clear(LED_PIN_RED);
    nrf_gpio_pin_set(LED_PIN_GREEN);
    nrf_delay_ms(25);
    nrf_gpio_pin_clear(LED_PIN_GREEN);
    //nrf_drv_gpiote_out_toggle(PIN_OUT);
    //nrf_delay_ms(100);
    //NRF_LOG_INFO("done");
}

//void gpiote_init(void)
//{

//    ret_code_t err_code;
//    NRF_LOG_INFO("gpiote init start");

//    nrf_drv_gpiote_init();
//    //APP_ERROR_CHECK(err_code);

//    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);

//    err_code = nrf_drv_gpiote_out_init(PIN_OUT, &out_config);
//    APP_ERROR_CHECK(err_code);

//    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
//    in_config.pull = NRF_GPIO_PIN_PULLDOWN;
//    in_config.sense = GPIOTE_CONFIG_POLARITY_LoToHi;
//    in_config.is_watcher = false;
//    in_config.hi_accuracy = false;

//    err_code = nrf_drv_gpiote_in_init(PIN_IN, &in_config, in_pin_handler_acc);
//    APP_ERROR_CHECK(err_code);

//}

//void gpiote_uninit(void){
//    nrf_drv_gpiote_uninit();
//}

void gpiote_acc_start(void){
    nrf_drv_gpiote_in_event_enable(ACC_IRQ, true);
}

void gpiote_acc_stop(void){
    nrf_drv_gpiote_in_event_disable(ACC_IRQ);
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
    //ism_write(ISM330_REG_FIFO_CTRL5, ((ISM330_SET_FIFO_ODR << 4) | (ISM330_SET_FIFO_MODE)));  // start measurements
    //nrf_gpio_pin_set(LED_PIN_RED);
    
    //write 000 to fifo_ctrl5
    //write 001 to fifo_ctrl5
}


void ism_start_measurements(void){
    ism_write(ISM330_REG_FIFO_CTRL5, ((ISM330_SET_FIFO_ODR << 4) | (ISM330_SET_FIFO_MODE)));  // start measurements
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
            //__WFE();
        }
        if (count == 1){
            fifo_data[idx] = (uint16_t*)((m_rx_buf[2] << 8) | (m_rx_buf[1]));
            NRF_LOG_INFO("fifo_data[%d]: %d", idx, fifo_data[idx++]);
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











uint32_t temp = 0;

static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
    //if (int_type == NRF_DRV_RTC_INT_COMPARE0)
    //{
    //    nrf_gpio_pin_toggle(COMPARE_EVENT_OUTPUT);
    //}
    //else if (int_type == NRF_DRV_RTC_INT_TICK)
    //{
    //    nrf_gpio_pin_toggle(TICK_EVENT_OUTPUT);
    //}

    nrf_drv_rtc_t const *const 	p_instance;
    temp = NRF_RTC2->COUNTER;
    //printf("current value is=%d\r\n",temp);
    if (temp%16384==0){
        //NRF_LOG_INFO("current value is = %d",temp);
        //nrf_drv_rtc_counter_clear(&rtc);
        nrf_gpio_pin_set(25);
        nrf_gpio_pin_clear(25);
    }
}


static void lfclk_config(void)
{
    //ret_code_t err_code = nrf_drv_clock_init();
    //APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}


void rtc_config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    //config.prescaler = 4095;
    config.prescaler = 0;
    err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc,true);

    //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
    err_code = nrf_drv_rtc_cc_set(&rtc,0,COMPARE_COUNTERTIME * 8,true);
    APP_ERROR_CHECK(err_code);

    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);
}





static void led_green_on(void){
    nrf_gpio_pin_set(9);
}

static void led_green_off(void){
    nrf_gpio_pin_clear(9);
}

static void led_red_on(void){
    nrf_gpio_pin_set(10);
}

static void led_red_off(void){
    nrf_gpio_pin_clear(10);
}

static void led_flash(uint8_t times){
    for (uint8_t i=0; i<times; i++){
        led_red_on();
        led_green_on();
        nrf_delay_ms(125);
        led_red_off();
        led_green_off();
        if (times > 1){
            nrf_delay_ms(125);
        }
    }
}


void timeout_handler(void * p_context);

uint8_t ble_sync_send_val = 1;

void timeout_handler(void * p_context)
{
    //NRF_LOG_INFO("timer done");
    //ble_sync_send_val = ble_sync_send_val > 0 ? 0 : 1;
    ret_code_t err_code = ble_lbs_led_status_send(&m_ble_lbs_c, ble_sync_send_val);
    if (err_code != NRF_SUCCESS &&
        err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
        err_code != NRF_ERROR_INVALID_STATE)
    {
        APP_ERROR_CHECK(err_code);
    } else {
        led_flash(1);
    }
}




static void create_timers()
{
    ret_code_t err_code;

    // Create timers
    err_code = app_timer_create(&sync_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                timeout_handler);
    APP_ERROR_CHECK(err_code);
}






static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}








/**@brief Function to handle asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num     Line number of the failing ASSERT call.
 * @param[in] p_file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}


/**@brief Function for handling the LED Button Service client errors.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void lbs_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for the LEDs initialization.
 *
 * @details Initializes all LEDs used by the application.
 */
static void leds_init(void)
{
    //bsp_board_init(BSP_INIT_LEDS);
    nrf_gpio_cfg_output(9);
    nrf_gpio_cfg_output(10);
}


/**@brief Function to start scanning.
 */
static void scan_start(void)
{
    ret_code_t err_code;

    err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(err_code);

    //bsp_board_led_off(CENTRAL_CONNECTED_LED);
    //bsp_board_led_on(CENTRAL_SCANNING_LED);
}


/**@brief Handles events coming from the LED Button central module.
 */
static void lbs_c_evt_handler(ble_lbs_c_t * p_lbs_c, ble_lbs_c_evt_t * p_lbs_c_evt)
{
    switch (p_lbs_c_evt->evt_type)
    {
        case BLE_LBS_C_EVT_DISCOVERY_COMPLETE:
        {
            ret_code_t err_code;

            err_code = ble_lbs_c_handles_assign(&m_ble_lbs_c,
                                                p_lbs_c_evt->conn_handle,
                                                &p_lbs_c_evt->params.peer_db);


            //BLE connections switch to 3200 ms intervals here ////////////////////////////////////////////////////
            //gpiote_init();
            //app_timer_start(sync_timer_id, APP_TIMER_TICKS(50), NULL);
            //nrf_delay_ms(25);
            //led_flash(1);

            


            err_code = app_button_enable();
            APP_ERROR_CHECK(err_code);

            // LED Button service discovered. Enable notification of Button.
            err_code = ble_lbs_c_button_notif_enable(p_lbs_c);
            APP_ERROR_CHECK(err_code);
        } break; // BLE_LBS_C_EVT_DISCOVERY_COMPLETE

        case BLE_LBS_C_EVT_BUTTON_NOTIFICATION:
        {
            NRF_LOG_INFO("Button state changed on peer to 0x%x.", p_lbs_c_evt->params.button.button_state);
            if (p_lbs_c_evt->params.button.button_state)
            {
                //bsp_board_led_on(LEDBUTTON_LED);
            }
            else
            {
                //bsp_board_led_off(LEDBUTTON_LED);
            }
        } break; // BLE_LBS_C_EVT_BUTTON_NOTIFICATION


        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code;

    // For readability.
    ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id)
    {
        // Upon connection, check which peripheral has connected (HR or RSC), initiate DB
        // discovery, update LEDs status and resume scanning if necessary. */
        case BLE_GAP_EVT_CONNECTED:
        {
            
            //nrf_drv_rtc_counter_clear(&rtc);            

            NRF_LOG_INFO("Connected.");
            
            err_code = ble_lbs_c_handles_assign(&m_ble_lbs_c, p_gap_evt->conn_handle, NULL);
            APP_ERROR_CHECK(err_code);

            err_code = ble_db_discovery_start(&m_db_disc, p_gap_evt->conn_handle);
            APP_ERROR_CHECK(err_code);
            //nrf_delay_ms(25);
            led_flash(1);
            nrf_delay_ms(50);
            gpiote_start();
            sync_time_flag = true;
            //send_second_signal = 0;
            //gpiote_init();
            //nrf_drv_timer_enable(&sync_timer);
            //err_code = app_simple_timer_start(APP_SIMPLE_TIMER_MODE_SINGLE_SHOT, timeout_handler, TIMEOUT_VALUE, NULL);
            //APP_ERROR_CHECK(err_code);

        } break;

        // Upon disconnection, reset the connection handle of the peer which disconnected, update
        // the LEDs status and start scanning again.
        case BLE_GAP_EVT_DISCONNECTED:
        {
            NRF_LOG_INFO("Disconnected.");
            //send_second_signal = 0;
            interrupt_count = 0;
            gpiote_uninit();
            scan_start();
            //nrf_drv_timer_disable(&sync_timer);
        } break;

        case BLE_GAP_EVT_TIMEOUT:
        {
            // We have not specified a timeout for scanning, so only connection attemps can timeout.
            if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN)
            {
                NRF_LOG_DEBUG("Connection request timed out.");
            }
        } break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
        {
            // Accept parameters requested by peer.
            err_code = sd_ble_gap_conn_param_update(p_gap_evt->conn_handle,
                                        &p_gap_evt->params.conn_param_update_request.conn_params);
            APP_ERROR_CHECK(err_code);
            //NRF_LOG_INFO("update params here????????");
        } break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_INFO("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTC_EVT_TIMEOUT:
        {
            // Disconnect on GATT Client timeout event.
            NRF_LOG_INFO("GATT Client Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTS_EVT_TIMEOUT:
        {
            // Disconnect on GATT Server timeout event.
            NRF_LOG_INFO("GATT Server Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
        } break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief LED Button client initialization.
 */
static void lbs_c_init(void)
{
    ret_code_t       err_code;
    ble_lbs_c_init_t lbs_c_init_obj;

    lbs_c_init_obj.evt_handler   = lbs_c_evt_handler;
    lbs_c_init_obj.p_gatt_queue  = &m_ble_gatt_queue;
    lbs_c_init_obj.error_handler = lbs_error_handler;

    err_code = ble_lbs_c_init(&m_ble_lbs_c, &lbs_c_init_obj);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupts.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


/**@brief Function for handling events from the button handler module.
 *
 * @param[in] pin_no        The pin that the event applies to.
 * @param[in] button_action The button action (press/release).
 */
static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    ret_code_t err_code;

    switch (pin_no)
    {
        case LEDBUTTON_BUTTON_PIN:
            err_code = ble_lbs_led_status_send(&m_ble_lbs_c, button_action);
            if (err_code != NRF_SUCCESS &&
                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
                err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            if (err_code == NRF_SUCCESS)
            {
                NRF_LOG_INFO("LBS write LED state %d", button_action);
            }
            break;

        default:
            APP_ERROR_HANDLER(pin_no);
            break;
    }
}


/**@brief Function for handling Scaning events.
 *
 * @param[in]   p_scan_evt   Scanning event.
 */
static void scan_evt_handler(scan_evt_t const * p_scan_evt)
{
    ret_code_t err_code;

    switch(p_scan_evt->scan_evt_id)
    {
        case NRF_BLE_SCAN_EVT_CONNECTING_ERROR:
            err_code = p_scan_evt->params.connecting_err.err_code;
            APP_ERROR_CHECK(err_code);
            break;
        default:
          break;
    }
}



/**@brief Function for initializing the button handler module.
 */
static void buttons_init(void)
{
    ret_code_t err_code;

    //The array must be static because a pointer to it will be saved in the button handler module.
    static app_button_cfg_t buttons[] =
    {
        {LEDBUTTON_BUTTON_PIN, false, BUTTON_PULL, button_event_handler}
    };

    err_code = app_button_init(buttons, ARRAY_SIZE(buttons),
                               BUTTON_DETECTION_DELAY);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling database discovery events.
 *
 * @details This function is callback function to handle events from the database discovery module.
 *          Depending on the UUIDs that are discovered, this function should forward the events
 *          to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
    ble_lbs_on_db_disc_evt(&m_ble_lbs_c, p_evt);
}


/**@brief Database discovery initialization.
 */
static void db_discovery_init(void)
{
    ble_db_discovery_init_t db_init;

    memset(&db_init, 0, sizeof(db_init));

    db_init.evt_handler  = db_disc_handler;
    db_init.p_gatt_queue = &m_ble_gatt_queue;

    ret_code_t err_code = ble_db_discovery_init(&db_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the log.
 */
//static void log_init(void)
//{
//    ret_code_t err_code = NRF_LOG_INIT(NULL);
//    APP_ERROR_CHECK(err_code);

//    NRF_LOG_DEFAULT_BACKENDS_INIT();
//}


/**@brief Function for initializing the timer.
 */
static void timer_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Power manager. */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


static void scan_init(void)
{
    ret_code_t          err_code;
    nrf_ble_scan_init_t init_scan;

    memset(&init_scan, 0, sizeof(init_scan));

    init_scan.connect_if_match = true;
    init_scan.conn_cfg_tag     = APP_BLE_CONN_CFG_TAG;

    err_code = nrf_ble_scan_init(&m_scan, &init_scan, scan_evt_handler);
    APP_ERROR_CHECK(err_code);

    // Setting filters for scanning.
    err_code = nrf_ble_scan_filters_enable(&m_scan, NRF_BLE_SCAN_NAME_FILTER, false);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_scan_filter_set(&m_scan, SCAN_NAME_FILTER, m_target_periph_name);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details Handle any pending log operation(s), then sleep until the next event occurs.
 */
static void idle_state_handle(void)
{
    NRF_LOG_FLUSH();
    nrf_pwr_mgmt_run();
}

static void opamp_init(void)
{
    nrf_gpio_cfg_output(2);
    nrf_gpio_pin_set(2);
    NRF_LOG_INFO("OpAmp initialized");
}

static void opamp_uninit(void)
{
    nrf_gpio_cfg_output(2);
    nrf_gpio_pin_clear(2);
    NRF_LOG_INFO("Op Amp Disabled");
}

static void rtc_clockout_enable(void)
{
    nrf_gpio_cfg_output(5);
    nrf_gpio_pin_set(5);
    NRF_LOG_INFO("RTC 32.768kHz Clock Out enabled");
}


void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    //NRF_LOG_INFO("pin change\t%d", intr++);
    //nrf_drv_gpiote_out_toggle(PIN_OUT);
    //nrf_delay_ms(100);
    //NRF_LOG_INFO("done");
    if ((sync_time_flag) && (interrupt_count++ > 1)){
        nrf_drv_rtc_counter_clear(&rtc);
        nrf_gpio_pin_set(25);
        nrf_gpio_pin_clear(25);
        nrf_gpio_pin_set(25);
        nrf_gpio_pin_clear(25);
        nrf_gpio_pin_set(25);
        nrf_gpio_pin_clear(25);
        sync_time_flag = false;
        gpiote_stop();
        led_green_on();
        nrf_delay_ms(20);
        led_green_off();
        //NRF_LOG_INFO("sync now");
    }
}

void gpiote_init()
{
    ret_code_t err_code;

    nrf_drv_gpiote_init();
    //APP_ERROR_CHECK(err_code);

    //nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);

    //err_code = nrf_drv_gpiote_out_init(27, &out_config);
    //APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;
    in_config.sense = GPIOTE_CONFIG_POLARITY_HiToLo;
    in_config.is_watcher = false;
    in_config.hi_accuracy = false;
    

    err_code = nrf_drv_gpiote_in_init(SYNC_INTERRUPT_PIN, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);


    in_config.pull = NRF_GPIO_PIN_PULLDOWN;
    in_config.sense = GPIOTE_CONFIG_POLARITY_LoToHi;
    in_config.is_watcher = false;
    in_config.hi_accuracy = false;
    
    //err_code = nrf_drv_gpiote_out_init(26, &out_config);
    //APP_ERROR_CHECK(err_code);

    in_config.sense = GPIOTE_CONFIG_POLARITY_LoToHi;
    err_code = nrf_drv_gpiote_in_init(ACC_IRQ, &in_config, in_pin_handler_acc);
    APP_ERROR_CHECK(err_code);
    
    NRF_LOG_INFO("GPIOTE initialized");

}

void gpiote_start(void){
    nrf_drv_gpiote_in_event_enable(SYNC_INTERRUPT_PIN, true);
    NRF_LOG_INFO("GPIOTE started");
}

void gpiote_stop(void){
    nrf_drv_gpiote_in_event_disable(SYNC_INTERRUPT_PIN);
    NRF_LOG_INFO("GPIOTE stopped");
}


void gpiote_uninit(void){
    if(!nrf_drv_gpiote_is_init()){
        nrf_drv_gpiote_uninit();
        NRF_LOG_INFO("GPIOTE uninitialized");
    } else { 
        NRF_LOG_INFO("GPIOTE already uninitialized");
        nrf_drv_gpiote_in_event_disable(SYNC_INTERRUPT_PIN);
    }
}






void pin_config(void){
    nrf_gpio_cfg_output(25);
    nrf_gpio_pin_clear(25);
}










int main(void)
{
    // Initialize.
    leds_init();
    led_flash(1);
    log_init();
    NRF_LOG_INFO("<~~~ BLE Sync CENTRAL ~~~>");
    timer_init();

    //rtc_clockout_enable();

    buttons_init();


    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = ACC_SS; //31
    spi_config.miso_pin = ACC_MISO; //30
    spi_config.mosi_pin = ACC_MOSI; //29
    spi_config.sck_pin  = ACC_SCK; //26
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));

    power_management_init();
    ble_stack_init();
    scan_init();
    gatt_init();
    db_discovery_init();
    lbs_c_init();
    gpiote_init();
    opamp_init();

    scan_start();

    pin_config();

    //nrf_gpio_pin_clear(9);
    //nrf_gpio_pin_clear(10);

    uint32_t err_code = app_simple_timer_init();
    APP_ERROR_CHECK(err_code);
    
    create_timers();

    //lfclk_config();

    //rtc_config();



    //ism_read(ISM330_REG_WHOAMI); // this breaks ble

    //ism_init();
    //ism_start_measurements();
    //gpiote_acc_start();


    NRF_LOG_INFO("start loop");
    nrf_gpio_pin_clear(25);
    for (;;)
    {
        idle_state_handle();
        if (ism_fifo_full){ 
            ism_read_fifo();
            ism_write(ISM330_REG_FIFO_CTRL5, (ISM330_SET_FIFO_ODR << 4));
            ism_write(ISM330_REG_FIFO_CTRL5, ((ISM330_SET_FIFO_ODR << 4) | (ISM330_SET_FIFO_MODE)));
            nrf_gpio_pin_set(LED_PIN_RED);
            ism_fifo_full = false;
        }    

    }
}
