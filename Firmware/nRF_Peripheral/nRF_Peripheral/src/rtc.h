#include "stdint.h"

#define RTC_REG_CONTROL1	    0x00
#define RTC_REG_CONTROL2	    0x01
#define RTC_REG_OFFSET		    0x02
#define RTC_REG_RAM		    0x03
#define RTC_REG_SECONDS		    0x04
#define RTC_REG_MINUTES		    0x05
#define RTC_REG_HOURS		    0x06
#define RTC_REG_DATE		    0x07
#define RTC_REG_WEEKDAY		    0x08
#define RTC_REG_MONTH		    0x09
#define RTC_REG_YEAR		    0x0A
#define RTC_REG_SECONDS_ALARM	    0x0B
#define RTC_REG_MINUTES_ALARM	    0x0C
#define RTC_REG_HOURS_ALARM	    0x0D
#define RTC_REG_DATE_ALARM	    0x0E
#define RTC_REG_WEEKDAY_ALARM	    0x0F
#define RTC_REG_TIMER_ALARM	    0x10
#define RTC_REG_TIMER_MOD	    0x11
#define RTC_VAL_CONTROL2	    0x20

void rtc_init(void);
void rtc_write_date(void);
void rtc_read_time(void);
void rtc_set_minute_alarm(void);
