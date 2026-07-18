#ifndef RTC_H
#define RTC_H

#include "types.h"
void init_RTC(void);
void set_RTC_Time(u8 hour,u8 min,u8 sec);
void set_RTC_Date(u8 dom,u8 month,u32 year);
void set_RTC_Day(u8 day);
void display_RTC_Time_On_LCD(u8 hour,u8 min,u8 sec);
void display_RTC_Date_On_LCD(u8 dom,u8 month,u32 year);
void display_RTC_Day_On_LCD(u32 day);
void display_Temp_Hum(void);
#endif
