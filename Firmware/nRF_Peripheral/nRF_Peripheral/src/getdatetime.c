#include "getdatetime.h"
#include "getdatetime_var.h"

void getdatetime(uint32_t *datetime_arr){
	
	datetime_arr[0] = TIMESTAMP_AT_FLASH_SEC; 
	datetime_arr[1] = TIMESTAMP_AT_FLASH_MIN; 
	datetime_arr[2] = TIMESTAMP_AT_FLASH_HR; 
	datetime_arr[3] = TIMESTAMP_AT_FLASH_DAY; 
	datetime_arr[4] = TIMESTAMP_AT_FLASH_MON; 
	datetime_arr[5] = TIMESTAMP_AT_FLASH_YR; 
}