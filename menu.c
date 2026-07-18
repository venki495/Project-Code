#include "lcd.h"
#include "keypad_defines.h"
#include "delay.h"
#include "i2c_eeprom.h"
#include "i2c_eeprom_defines.h"

int verifyPWD(void);
extern char tempSPoint;
extern char humiditySPoint;
extern int pwd;
extern char pass[5];

// Convert an integer to a fixed-length string
void itoa(int num, char *str, int digits)
{
    int i;
    for(i=digits-1; i>=0; i--){
        str[i] = (num % 10) + '0';   // Convert digit to character
        num /= 10;                   // Move to the next digit
    }
    str[digits] = '\0';              // Null-terminate the string
}

void DisplayMenu(void){
	cmdLCD(0x01);
	cmdLCD(0x80);
	stringLCD("1)CHANGE SPOINT");
	cmdLCD(0xC0);
	stringLCD("2)CHANGEPWD 3)EX");
}

void EditTempSPoint(void){
	TEMP:		cmdLCD(0x01);
					cmdLCD(0x80);
					stringLCD("ENTER TMPSPOINT:");
					cmdLCD(0xC0);
					tempSPoint=readnum();
					if(tempSPoint>100){
												cmdLCD(0x01);
												cmdLCD(0x80);
												stringLCD("INVALID TEMP");
												cmdLCD(0xC0);
												stringLCD("SETPOINT...");
												delay_ms(500);
											goto TEMP;
										}
					I2C_eeprom_ByteWrite(I2C_EEPROM_SA1,0x20,tempSPoint);
					cmdLCD(0x01);
					cmdLCD(0x80);
					stringLCD("TEMP SPOINT");
					cmdLCD(0xC0);
					stringLCD("MODIFIED...");
					delay_ms(500);
}

void EditPassword(void){
	check:		cmdLCD(0x01);
						cmdLCD(0x80);
						stringLCD("ENTER NEW PWD:");
						cmdLCD(0xC0);
						pwd=readnum();
						if(pwd>9999){
							cmdLCD(0x01);
							cmdLCD(0x80);
							stringLCD("ENTER 4 DIGITS");
							cmdLCD(0xC0);
							stringLCD("ONLY...");
							delay_ms(500);
							goto check;
						}else{
								itoa(pwd,pass,4);
								i2c_eeprom_pagewrite(I2C_EEPROM_SA1,0x00,pass,4);
								cmdLCD(0x01);
								cmdLCD(0x80);
								stringLCD("PWD UPDATED...");
								delay_ms(500);
						}
}

void EditHumiditySPoint(void){
	HUMIDITY:		cmdLCD(0x01);
							cmdLCD(0x80);
							stringLCD("ENTER HMDSPOINT:");
							cmdLCD(0xC0);
							humiditySPoint=readnum();
							if(humiditySPoint>100){
									cmdLCD(0x01);
									cmdLCD(0x80);
									stringLCD("INVALID HUMIDTY");
									cmdLCD(0xC0);
									stringLCD("SETPOINT...");
									delay_ms(500);
								goto HUMIDITY;
							}
							I2C_eeprom_ByteWrite(I2C_EEPROM_SA1,0x20,humiditySPoint);
							cmdLCD(0x01);
							cmdLCD(0x80);
							stringLCD("HUMIDITY SPOINT");
							cmdLCD(0xC0);
							stringLCD("MODIFIED...");
							delay_ms(500);
}

void DisplayMenu2(void){
		cmdLCD(0x01);
		cmdLCD(0x80);
		stringLCD("1)TEMPERATURE");
		cmdLCD(0xC0);
		stringLCD("2)HUMIDITY 3)EXT");
}

void implementMenu2(void){
		u32 key;
	while(1){
			while(ColStat());
			key=KeyVal();
			while(!ColStat());
			switch(key){
				case 1:
						EditTempSPoint();
						DisplayMenu2();
						break;
				case 2:
						EditHumiditySPoint();
						DisplayMenu2();						
						break;
				case 3:
						cmdLCD(0x01);
						return ;
				default:
						cmdLCD(0x01);
						cmdLCD(0x80);
						stringLCD("INVALID OPTION..");
						DisplayMenu2();
			}
	}
}

void implementMenu(){
		u32 key;
		while(1){
			while(ColStat());
			key=KeyVal();
			while(!ColStat());
			switch(key){
				case 1:
					if(verifyPWD()){
						DisplayMenu2();
						implementMenu2();
					}else{
						cmdLCD(0x01);
						cmdLCD(0x80);
						stringLCD("INVALID PASSWORD");
						delay_ms(500);
					}
					DisplayMenu();
					break;
				case 2:
					if(verifyPWD()){
						EditPassword();
					}else{
						cmdLCD(0x01);
						cmdLCD(0x80);
						stringLCD("INVALID PASSWORD");
						delay_ms(500);
					}
					DisplayMenu();
					break;
				case 3:
					cmdLCD(0x01);
					return;
				default:
				  cmdLCD(0x01);
					stringLCD("INVALID OPTION");
					delay_s(1);
					DisplayMenu();
			}
		}
}
