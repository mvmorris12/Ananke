#include "RTC.h"
#include "I2C.h"
#include "LPC177x_8x.h"
#include "type.h"
#include "Interrupts.h"
#include "getdatetime_var.h"

extern volatile unsigned char I2C_MasterBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned char I2C_SlaveBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned int I2C_Count[I2C_PORT_NUM];
extern volatile unsigned int I2C_ReadLength[I2C_PORT_NUM];
extern volatile unsigned int I2C_WriteLength[I2C_PORT_NUM];
extern volatile RTCTime local_time;

//void rtc_set_minute_timer(void){
//    I2C_WriteLength[1] = 3;
//    I2C_MasterBuffer[1][0] = (0x51<<1) | 0x00;
//    I2C_MasterBuffer[1][1] = 0x01;
//    I2C_MasterBuffer[1][2] = 0x20;  
//    I2C_Engine(1);  
//}


//uint32_t rtc_read_time(void){
//    I2C_ReadLength[1] = 3;
//    I2C_WriteLength[1] = 2;
//    I2C_MasterBuffer[1][0] = (0x51<<1) | 0x00;
//    I2C_MasterBuffer[1][1] = 0x04;
//    I2C_MasterBuffer[1][2] = (0x51<<1) | 0x01;
//    I2C_Engine(1);
//    //printf("%X\t%02X %02X %02X\n", *I2C_MasterBuffer[1], I2C_SlaveBuffer[1][0], I2C_SlaveBuffer[1][1], I2C_SlaveBuffer[1][2]);
//    uint8_t seconds, minutes, hours;
//    seconds = I2C_SlaveBuffer[1][0] & 0x7F;
//    minutes = I2C_SlaveBuffer[1][1];
//    hours = I2C_SlaveBuffer[1][2];
//    printf("%02X %02X %02X\n", seconds, minutes, hours);
//};


//void rtc_set_time(void){
//    I2C_WriteLength[1] = 9;
//    I2C_MasterBuffer[1][0] = (0x51<<1) | 0x00;
//    I2C_MasterBuffer[1][1] = 0x04;
//    I2C_MasterBuffer[1][2] = TIMESTAMP_AT_FLASH_SEC+5;  
//    I2C_MasterBuffer[1][3] = TIMESTAMP_AT_FLASH_MIN;  
//    I2C_MasterBuffer[1][4] = TIMESTAMP_AT_FLASH_HR;  
//    I2C_MasterBuffer[1][5] = TIMESTAMP_AT_FLASH_DATE;  
//    I2C_MasterBuffer[1][6] = TIMESTAMP_AT_FLASH_DAY;  
//    I2C_MasterBuffer[1][7] = TIMESTAMP_AT_FLASH_MON;  
//    I2C_MasterBuffer[1][8] = TIMESTAMP_AT_FLASH_YR & 0xFF;  
//    I2C_Engine(1);  
//}

volatile uint32_t alarm_on = 0;

/*****************************************************************************
** Function name:		RTC_IRQHandler
**
** Descriptions:		RTC interrupt handler, it executes based on the
**						the alarm setting
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
//void RTC_IRQHandler (void) 
//{  
//  //LPC_RTC->ILR |= ILR_RTCCIF;		/* clear interrupt flag */
//  alarm_on = 1;
//  return;
//}

/*****************************************************************************
** Function name:		RTCInit
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTC_Init( void )
{
  alarm_on = 0;

  /* Enable CLOCK into RTC */
  LPC_SC->PCONP |= (1 << 9);

  /* If RTC is stopped, clear STOP bit. */
  if ( LPC_RTC->RTC_AUX & (0x1<<4) )
  {
	LPC_RTC->RTC_AUX |= (0x1<<4);	
  }
  
  /*--- Initialize registers ---*/    
  LPC_RTC->AMR = 0;
  LPC_RTC->CIIR = 0;
  LPC_RTC->CCR = 0;

  /* Set local time for internal RTC*/
  #if UPDATE_RTC_ON_PWR == 0x1
  local_time.RTC_Sec    = TIMESTAMP_AT_FLASH_SEC;
  local_time.RTC_Min    = TIMESTAMP_AT_FLASH_MIN;
  local_time.RTC_Hour   = TIMESTAMP_AT_FLASH_HR;
  local_time.RTC_Mday   = TIMESTAMP_AT_FLASH_DATE;
  local_time.RTC_Wday   = TIMESTAMP_AT_FLASH_DAY;
  //local_time.RTC_Yday = TIMESTAMP_AT_FLASH_DATE;
  local_time.RTC_Mon    = TIMESTAMP_AT_FLASH_MON;
  local_time.RTC_Year   = TIMESTAMP_AT_FLASH_YR;
  RTCSetTime( local_time );
  #endif                
  return;
}

/*****************************************************************************
** Function name:		RTCStart
**
** Descriptions:		Start RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStart( void ) 
{
  /*--- Start RTC counters ---*/
  LPC_RTC->CCR |= CCR_CLKEN;
  LPC_RTC->ILR = ILR_RTCCIF;
  rtc_interrupt_enable();
  return;
}

/*****************************************************************************
** Function name:		RTCStop
**
** Descriptions:		Stop RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStop( void )
{   
  /*--- Stop RTC counters ---*/
  LPC_RTC->CCR &= ~CCR_CLKEN;
  return;
} 

/*****************************************************************************
** Function name:		RTC_CTCReset
**
** Descriptions:		Reset RTC clock tick counter
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTC_CTCReset( void )
{   
  /*--- Reset CTC ---*/
  LPC_RTC->CCR |= CCR_CTCRST;
  return;
}

/*****************************************************************************
** Function name:		RTCSetTime
**
** Descriptions:		Setup RTC timer value
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetTime( RTCTime Time ) 
{
  LPC_RTC->SEC = Time.RTC_Sec;
  LPC_RTC->MIN = Time.RTC_Min;
  LPC_RTC->HOUR = Time.RTC_Hour;
  LPC_RTC->DOM = Time.RTC_Mday;
  LPC_RTC->DOW = Time.RTC_Wday;
  LPC_RTC->DOY = Time.RTC_Yday;
  LPC_RTC->MONTH = Time.RTC_Mon;
  LPC_RTC->YEAR = Time.RTC_Year;    
  return;
}

/*****************************************************************************
** Function name:		RTCSetAlarm
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetAlarm( RTCTime Alarm ) 
{   
  LPC_RTC->ALSEC = Alarm.RTC_Sec;
  LPC_RTC->ALMIN = Alarm.RTC_Min;
  LPC_RTC->ALHOUR = Alarm.RTC_Hour;
  LPC_RTC->ALDOM = Alarm.RTC_Mday;
  LPC_RTC->ALDOW = Alarm.RTC_Wday;
  LPC_RTC->ALDOY = Alarm.RTC_Yday;
  LPC_RTC->ALMON = Alarm.RTC_Mon;
  LPC_RTC->ALYEAR = Alarm.RTC_Year;    
  return;
}

/*****************************************************************************
** Function name:		RTCGetTime
**
** Descriptions:		Get RTC timer value
**
** parameters:			None
** Returned value:		The data structure of the RTC time table
** 
*****************************************************************************/
RTCTime RTCGetTime( void ) 
{
  RTCTime LocalTime;
    
  LocalTime.RTC_Sec = LPC_RTC->SEC;
  LocalTime.RTC_Min = LPC_RTC->MIN;
  LocalTime.RTC_Hour = LPC_RTC->HOUR;
  LocalTime.RTC_Mday = LPC_RTC->DOM;
  LocalTime.RTC_Wday = LPC_RTC->DOW;
  LocalTime.RTC_Yday = LPC_RTC->DOY;
  LocalTime.RTC_Mon = LPC_RTC->MONTH;
  LocalTime.RTC_Year = LPC_RTC->YEAR;
  return ( LocalTime );    
}

/*****************************************************************************
** Function name:		RTCSetAlarmMask
**
** Descriptions:		Set RTC timer alarm mask
**
** parameters:			Alarm mask setting
** Returned value:		None
** 
*****************************************************************************/
void RTCSetAlarmMask( uint32_t AlarmMask ) 
{
  /*--- Set alarm mask ---*/    
  LPC_RTC->AMR = AlarmMask;
  return;
}


void print_time(void){
    printf("Sec: %d\n", LPC_RTC->SEC);
    printf("Min: %d\n", LPC_RTC->MIN);
    printf("Hr:  %d\n", LPC_RTC->HOUR);
    printf("DOM: %d\n", LPC_RTC->DOM);
    printf("DOW: %d\n", LPC_RTC->DOW);
    printf("DOY: %d\n", LPC_RTC->DOY);
    printf("Mon: %d\n", LPC_RTC->MONTH);
    printf("Yr:  %d\n", LPC_RTC->YEAR);
}
