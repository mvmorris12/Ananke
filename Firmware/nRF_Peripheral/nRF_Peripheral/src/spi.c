#include "spi.h"
#include "nrf_gpio.h"
#include "nrf_drv_spi.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
volatile bool spi_xfer_done;

void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    spi_xfer_done = true;
    //NRF_LOG_INFO("Transfer completed.");
    //NRF_LOG_FLUSH();
    //if (m_rx_buf[0] != 0)
    //{
    //    NRF_LOG_INFO(" Received:");
    //    NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));
    //}
}


static ret_code_t fast_hardware_init(void)
{
    ret_code_t err_code;

    //nrf_gpio_cfg_output(LCD_DC);



    nrf_drv_spi_config_t spi_config =  
                 {.sck_pin                 = LCD_CLK, 
                  .miso_pin                = LCD_MOSI, 
                  .miso_pin                = NRF_MISO, 
                  .sck_pin                 = LCD_CS,  
                  .frequency               = SPI_FREQ_4MBPS, 
                  .mode                    = NRF_DRV_SPI_MODE_0, 
                  .bit_order               = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST};
    
    // Initialize the SPI
    err_code = (ret_code_t) spi_master_init(SPI_INSTANCE, &spi_config);

    //fast_spi = true;

    if(err_code != 0){
      err_code = NRF_SUCCESS;
    }
    else{
      err_code = NRF_ERROR_TIMEOUT;
    }
    return err_code;
}





