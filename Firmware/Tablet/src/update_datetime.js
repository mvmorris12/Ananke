function updatetime()
{
	var file       = "src/getdatetime_var.h";
	var text_sec   = "#define TIMESTAMP_AT_FLASH_SEC  (";
	var text_min   = "#define TIMESTAMP_AT_FLASH_MIN  (";
	var text_hr    = "#define TIMESTAMP_AT_FLASH_HR   (";
	var text_day   = "#define TIMESTAMP_AT_FLASH_DAY  (";
	var text_date  = "#define TIMESTAMP_AT_FLASH_DATE (";
	var text_mon   = "#define TIMESTAMP_AT_FLASH_MON  (";
	var text_yr    = "#define TIMESTAMP_AT_FLASH_YR   (";
	var d          = new Date();
	var sec        = d.getSeconds();
	var min 	   = d.getMinutes();
	var hr  	   = d.getHours();
	var day 	   = d.getDay();
	var date 	   = d.getDate();
	var mon		   = d.getMonth() + 1;
	var yr 		   = d.getFullYear();
	var text_write = (text_sec  + sec  + ')\n' 
					+ text_min  + min  + ')\n'
					+ text_hr   + hr   + ')\n'
					+ text_day  + day  + ')\n'
					+ text_date + date + ')\n'
					+ text_mon  + mon  + ')\n'
					+ text_yr   + yr   +  ')');

	CWSys.writeStringToFile(file, text_write); 
}

updatetime();


//var secondsSinceEpoch = Math.round(d / 1000);
//CWSys.writeStringToFile(file, text + (secondsSinceEpoch - 210866785200));
//var d = new Date(secondsSinceEpoch - 210866785200);