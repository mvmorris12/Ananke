function updatetime()
{
	var file       = "src/getdatetime_var.h";
	var text_sec   = "#define TIMESTAMP_AT_FLASH_SEC  0x00";
	var text_min   = "#define TIMESTAMP_AT_FLASH_MIN  0x";
	var text_hr    = "#define TIMESTAMP_AT_FLASH_HR   0x";
	var text_day   = "#define TIMESTAMP_AT_FLASH_DAY  0x";
	var text_date  = "#define TIMESTAMP_AT_FLASH_DATE 0x";
	var text_mon   = "#define TIMESTAMP_AT_FLASH_MON  0x";
	var text_yr    = "#define TIMESTAMP_AT_FLASH_YR   0x";
	var d          = new Date();
	var sec        = d.getSeconds();
	var min 	   = d.getMinutes();
	var hr  	   = d.getHours();
	var day 	   = d.getDay();
	var date 	   = d.getDate();
	var mon		   = d.getMonth() + 1;
	var yr 		   = d.getFullYear() - 2000;
	var text_write = (text_sec  + '\n' 
					+ text_min  + (min+1)  + '\n'  
					+ text_hr   + hr   + '\n'
					+ text_day  + day  + '\n'
					+ text_date + date + '\n'
					+ text_mon  + mon  + '\n'
					+ text_yr   + yr);

	CWSys.writeStringToFile(file, text_write); 
}

updatetime();


//var secondsSinceEpoch = Math.round(d / 1000);
//CWSys.writeStringToFile(file, text + (secondsSinceEpoch - 210866785200));
//var d = new Date(secondsSinceEpoch - 210866785200);