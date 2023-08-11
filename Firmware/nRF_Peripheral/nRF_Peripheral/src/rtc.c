#include "rtc.h"
#include "i2c.h"
#include "getdatetime_var.h"


extern struct Sensor_Data sensor_data;

void rtc_init(void){
    rtc_write_date();
    rtc_set_minute_alarm();
}


void rtc_write_date(void){
    //uint8_t rtc_sec = sprintf("0x%02x", TIMESTAMP_AT_FLASH_SEC);
    uint8_t date_arr[7] = {TIMESTAMP_AT_FLASH_SEC,
	    TIMESTAMP_AT_FLASH_MIN,
	    TIMESTAMP_AT_FLASH_HR,
	    TIMESTAMP_AT_FLASH_DATE,
	    TIMESTAMP_AT_FLASH_DAY,
	    TIMESTAMP_AT_FLASH_MON,
	    TIMESTAMP_AT_FLASH_YR};
    twi_write(TWI_ADDR_RTC, RTC_REG_SECONDS, date_arr, sizeof(date_arr));
    NRF_LOG_INFO("RTC initialized");
}


void rtc_read_time(void){
    uint8_t time_data[7];
    char date_str[20];
    twi_read(TWI_ADDR_RTC, RTC_REG_SECONDS, time_data, sizeof(time_data));
    //sprintf(date_str, "%02x:%02x:%02x %02x-%02x-20%x", 
	   // time_data[2], time_data[1], time_data[0], time_data[5], time_data[3], time_data[6]);
    sensor_data.rtc.year = time_data[6];
    sensor_data.rtc.month = time_data[5];
    sensor_data.rtc.day = time_data[3];
    sensor_data.rtc.hour = time_data[2];
    sensor_data.rtc.minute = time_data[1];
    sensor_data.rtc.second = time_data[0];
    //sensor_data.rtc.epoch_s =
    //    (((sensor_data.rtc.year/16*10 + (sensor_data.rtc.year%16) + 30)) * 31556926) +
    //    (((sensor_data.rtc.month/16*10 + (sensor_data.rtc.month%16) - 1)) * 2629743) +
    //    (((sensor_data.rtc.day/16*10 + (sensor_data.rtc.day%16) - 1)) * 86400) +
    //    (((sensor_data.rtc.hour/16*10 + (sensor_data.rtc.hour%16) - 1)) * 3600) +
    //    (((sensor_data.rtc.minute/16*10 + (sensor_data.rtc.minute%16)) * 60));

    NRF_LOG_INFO("%02x:%02x:%02x %02x-%02x-20%x", 
        sensor_data.rtc.hour,
        sensor_data.rtc.minute,
        sensor_data.rtc.second,
        sensor_data.rtc.month,
        sensor_data.rtc.day,
        sensor_data.rtc.year);
    //NRF_LOG_INFO("%d", sensor_data.rtc.epoch_s);
    //NRF_LOG_INFO("%X", sensor_data.rtc.epoch_s);
}


void rtc_set_minute_alarm(void){
    uint8_t tx_data = RTC_VAL_CONTROL2;
    twi_write(TWI_ADDR_RTC, RTC_REG_CONTROL2, &tx_data, sizeof(tx_data));
}