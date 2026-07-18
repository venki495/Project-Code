#include <LPC21xx.h>
#include "rtc_defines.h"
#include "types.h"
#include "rtc.h"
#include "lcd.h"

char week[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

void init_RTC(){
		CCR=RTC_RESET;
	#ifdef LPC2148
		CCR=RTC_ENABLE|RTC_CLKSRC;
	#else
		PREINT=PREINT_VAL;
		PREFRAC=PREFRAC_VAL;
		CCR=RTC_ENABLE;
	#endif
}

void set_RTC_Time(u8 hour,u8 min,u8 sec){
	HOUR=hour;
	MIN=min;
	SEC=sec;
}

void set_RTC_Date(u8 dom,u8 month,u32 year){
	DOM=dom;
	MONTH=month;
	YEAR=year;
}

void set_RTC_Day(u8 day){
		DOW=day;
}

void display_RTC_Time_On_LCD(u8 hour,u8 min,u8 sec){
		cmdLCD(0x80);
		charLCD((hour/10)+48);
		charLCD((hour%10)+48);
		charLCD(':');
		charLCD((min/10)+48);
		charLCD((min%10)+48);
		charLCD(':');
		charLCD((sec/10)+48);
		charLCD((sec%10)+48);
}

void display_RTC_Date_On_LCD(u8 dom,u8 month,u32 year){
		cmdLCD(0xc0);
		charLCD((dom/10)+48);
		charLCD((dom%10)+48);
		charLCD('/');
		charLCD((month/10)+48);
		charLCD((month%10)+48);
		charLCD('/');
		integerLCD(year%100);
}

void display_RTC_Day_On_LCD(u32 day){
		cmdLCD(0xcc);
		stringLCD(week[day]);
}
void display_Temp_Hum(void)
{
	cmdLCD(0x8A);
	stringLCD("T:");
	cmdLCD(0x8D);
	charLCD(223);
	charLCD('C');
	cmdLCD(0xCA);
	stringLCD("H:");
}
