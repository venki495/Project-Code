//delay.c
#include"delay.h"
#include"types.h"
void delay_us(u32 tdly){
	tdly*=12000000;
	while(tdly--);
}
void delay_ms(u32 tdly){
	tdly*=12000;
	while(tdly--);
}
void delay_s(u32 tdly){
	tdly*=12;
	while(tdly--);
}

//kepad.c
#include<LPC21xx.h>
#include"Keypad.h"
#include"lcd.h"

u8 LUT[][3]={	7,8,9,
							4,5,6,
              1,2,3,
							'C',0,'='};

void KeyPdInit(void)
{
		IODIR1|=((1<<R0)|(1<<R1)|(1<<R2)|(1<<R3));
	    //P1.16-P1.19 are for output functionality
		IOCLR1=((1<<R0)|(1<<R1)|(1<<R2)|(1<<R3));
		//Initializing rows to 0

}
u8 ColStat(void)
{
	if((((IOPIN1>>20)&0x0f)==0x0f))
		return 1;
	else 
		return 0;
}
u8 KeyVal(void)
{
		char row_val=0,col_val=0;
		IOCLR1=(1<<R0);
		IOSET1=((1<<R1)|(1<<R2)|(1<<R3));
		if((((IOPIN1>>20)&0x0f)!=0x0f))
		{
			row_val=0;
			goto colcheck;
		}
		IOCLR1=(1<<R1);
		IOSET1=((1<<R0)|(1<<R2)|(1<<R3));
		if((((IOPIN1>>20)&0x0f)!=0x0f))
		{
			row_val=1;
			goto colcheck;
		}
		IOCLR1=(1<<R2);
		IOSET1=((1<<R0)|(1<<R1)|(1<<R3));
		if((((IOPIN1>>20)&0x0f)!=0x0f))
		{
			row_val=2;
			goto colcheck;
		}
		IOCLR1=(1<<R3);
		IOSET1=((1<<R0)|(1<<R1)|(1<<R2));
		if((((IOPIN1>>20)&0x0f)!=0x0f))
			row_val=3;
		
		colcheck:		
		if(((IOPIN1>>C0)&1)==0)
			col_val=0;
		else if(((IOPIN1>>C1)&1)==0)
			col_val=1;
		else if(((IOPIN1>>C2)&1)==0)
			col_val=2;
		else
			col_val=3;

		IOCLR1=((1<<R0)|(1<<R1)|(1<<R2)|(1<<R3));
	//Initializing rows to 0
		return(LUT[row_val][col_val]);
	
}
 

u32 readnum()
{
	u32 ch=0,key=0;
	while(1)
	{
		while(ColStat());
		key=KeyVal();
		while(!ColStat());
		if(key=='=')
			break;
		else if(key=='C')
		{
			if(ch>0)
			{
				ch/=10;
				cmdLCD(0x10);
				charLCD(' ');
				cmdLCD(0x10);
			}
		}
		else
		{
			integerLCD(key);
			ch=(ch*10)+key;
		}
	} 
	return ch;
}

//lcd.c
#include<LPC21xx.h>
#include "delay.h"
#include "lcd.h"
#include "defines.h"
#include "lcd_defines.h"
#include "types.h"
void init_LCD(){
	//setting the output direction for the LCD
		IODIR0|=((LCD_DAT<<8)|(1<<RS)|(1<<EN)|(1<<RW));
	//minimum 15msec delay is required after powersupplied
		delay_ms(20);
	//initialize lcd for 8-bit mode
		cmdLCD(0x30);
	//minimum 5msec for instruction to complete
		delay_ms(10);
	//command as per data sheet
		cmdLCD(0x30);
	//minimum 160usec required for instruction to complete
		delay_ms(1);
	//command as per datasheet
		cmdLCD(0x30);
	//minimum 160usec required
		delay_ms(1);
	//set Interface length
		cmdLCD(0x38);
	//turn off the display
		cmdLCD(0x010);
	//clear the display
		cmdLCD(0x001);
	//set cursor move direction
		cmdLCD(0x06);
	//enable display and cursor(optional)
		cmdLCD(0x0c);
}
void displayLCD(u8 data){
	//enbling write mode by clearing the RW to enter data into LCD
		IOCLR0=1<<RW;
	//entering the data into LCD
		WRITEBYTE(IOPIN0,8,data);
	//set enable pin to enter data into LCD
		IOSET0=1<<EN;
	//delay for data to enter into LCD
		delay_ms(1);
	//clear the enable to complete the data writing into LCD
		IOCLR0=1<<EN;
	//delay for LCD to process Data
		delay_ms(5);
}
void cmdLCD(u8 cmd){
	//clearing the RS for command input
		IOCLR0=1<<RS;
	//passing command to Display
		displayLCD(cmd);
}
void charLCD(u8 data){
	//setting the RS for Data input
		IOSET0=1<<RS;
	//passing data to display
		displayLCD(data);
}
void stringLCD(char *p){
	//loop over the string and passing each character to charLCD
		while(*p)
			charLCD(*p++);
}
void integerLCD(s32 data){
	//array to store the integer values in the form of characters
		u8 a[30];
	//variable for index of the array
		s32 i=0;
	//if data is 0 then displaying character '0' and returning control to main
		if(data==0){
			charLCD('0');
			return ;
		}
		//if data is negative displaying character '-' and converting data into positive
		if(data<0){
			charLCD('-');
			data=-data;
		}
		//extracting each digit from number and storing in array in the form of characters in reverse order
		while(data){
			a[i++]=data%10+48;
			data/=10;
		}
		//looping over the array and passing each character to charLCD
		for(i=i-1;i>=0;i--)
			charLCD(a[i]);
}
void floatLCD(f32 f){
	//variables num1 for integer number before decimal point and num2 for after decimal point
		u32 num1=f,num2,n=0;
	//subtracting the num1 from f to get after decimal value
		f=f-num1;
	//looping to send 6 digits after decimal point to before decimal point
		while(n++<6)
			f*=10;
	//storing the value in num2
		num2=f;
	//displaying the num1
		integerLCD(num1);
	//displaying the decimal point
		charLCD('.');
	//displaying the num2
		integerLCD(num2);
}

void custCharDegree(void){
	u8 i,LUT[]={0x0e,0x11,0x11,0x0e,0x00,0x00,0x00,0x00};
	for(i=0;i<8;i++)
		charLCD(LUT[i]);
}

//RTC.c
#include <LPC21xx.h>
#include "RTC_defines.h"
#include "types.h"
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

//main.c
#include <LPC21xx.h>
#include <string.h>
#include "RTC.h"
#include "LCD.h"
#include "keypad_defines.h"
#include "delay.h"
#include "menu.h"
#include "I2C.h"
#include "i2c_eeprom.h"
#include "I2C_eeprom_defines.h"

extern int flag;
int pwd;
char pass[5];
int p;
char tempSPoint=37;
char humiditySPoint=70;

void Enable_EINT0(void);
void itoa(int,char *,int);


int myatoi(const char *str) {
    int i=0;
    int num=0;
    int sign=1;
    if(str[i]=='-'){
        sign=-1;
        i++;
    }else if(str[i]=='+'){
        i++;
    }
    while(str[i]>='0'&&str[i]<='9'){
        num=num*10+(str[i]-'0');
        i++;
    }
    return sign * num;
}

int verifyPWD(void){
	cmdLCD(0x01);
	cmdLCD(0x80);
	stringLCD("PASSWORD: ");
	cmdLCD(0xC0);
	pwd=readnum();
	i2c_eeprom_seqread(I2C_EEPROM_SA1,0x00,pass,4);
	delay_ms(10);
	pass[4]='\0';
	p=myatoi(pass);
	return (pwd==p);
}

int main(){
	init_LCD();
	init_RTC();
	Enable_EINT0();
	KeyPdInit();
	Init_I2C();
	set_RTC_Time(10,20,0);
	set_RTC_Date(12,02,2026);
	itoa(1234,pass,4);
	i2c_eeprom_pagewrite(I2C_EEPROM_SA1,0x00,pass,4);
	delay_ms(10);
	I2C_eeprom_ByteWrite(I2C_EEPROM_SA1,0x20,tempSPoint);
	delay_ms(10);
	I2C_eeprom_ByteWrite(I2C_EEPROM_SA1,0x30,humiditySPoint);
	delay_ms(10);
	while(1)
	{
		display_RTC_Time_On_LCD(HOUR,MIN,SEC);
		display_RTC_Date_On_LCD(DOM,MONTH,YEAR);
		display_Temp_Hum();
		if(flag){
				DisplayMenu();
				implementMenu();
			flag=0;
		}
	}
}

//eint0_test.c
#include <LPC21xx.h>

#include "types.h"
#include "defines.h"
#define EINT0_PIN      16	
#define EINT0_VIC_CHNO 14	
int flag=0;

void eint0_isr(void) __irq
{
	flag=1;
	EXTINT = 1<<0;
	VICVectAddr = 0;
}	


void Enable_EINT0(void)
{
	PINSEL1=((PINSEL1&~(3<<0))|(1<<0));
	VICIntEnable = 1<<EINT0_VIC_CHNO;
	VICVectCntl0 = ((1<<5) | EINT0_VIC_CHNO);
	VICVectAddr0 = (unsigned int)eint0_isr;
	EXTMODE = 1<<0;
}



//menu.c
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

void itoa(int num, char *str, int digits) {
    int i;
    for(i=digits-1;i>=0;i--){
        str[i] =(num%10)+'0';
        num/=10;
    }
    str[digits]='\0';
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

//I2Cc
#include <LPC21xx.h>
#include "I2C_defines.h"

void Init_I2C(void){
		PINSEL0|=((SCL_PIN)|(SDA_PIN));
		I2SCLL=I2C_DIVIDER;
		I2SCLH=I2C_DIVIDER;
		I2CONSET=1<<I2EN_BIT;
}

void I2C_Start(void){
		I2CONSET=1<<STA_BIT;
		while(((I2CONSET>>SI_BIT)&1)==0);
		I2CONCLR=1<<STAC_BIT;
}

void I2C_Restart(void){
		I2CONSET=1<<STA_BIT;
		I2CONCLR=1<<SIC_BIT;
		while(((I2CONSET>>SI_BIT)&1)==0);
		I2CONCLR=1<<STAC_BIT;
}

void I2C_Stop(void){
		I2CONSET=1<<STO_BIT;
		I2CONCLR=1<<SIC_BIT;
}

void I2C_Write(unsigned char data){
		I2DAT=data;
		I2CONCLR=1<<SIC_BIT;
		while(((I2CONSET>>SI_BIT)&1)==0);
}

unsigned char I2C_nack(void){
		I2CONCLR=1<<SIC_BIT;
		while(((I2CONSET>>SI_BIT)&1)==0);
		return I2DAT;
}

unsigned char I2C_mack(void){
		I2CONSET=1<<AA_BIT;
		I2CONCLR=1<<SIC_BIT;
		while(((I2CONSET>>SI_BIT)&1)==0);
		I2CONCLR=1<<AAC_BIT;
		return I2DAT;
}


//i2c_eeprom.c
#include <LPC21XX.h>
#include "I2C.h"
#include "delay.h"

typedef unsigned char u8;
typedef unsigned short int u16;

void I2C_eeprom_ByteWrite(u8 slaveAddr,u16 BuffAddr,u8 data){
		I2C_Start();
		I2C_Write(slaveAddr<<1);
		I2C_Write(BuffAddr>>8);
		I2C_Write(BuffAddr);
		I2C_Write(data);
		I2C_Stop();
}

u8 I2C_eeprom_randomRead(u8 slaveAddr,u16 BuffAddr){
		u8 dat;
		I2C_Start();
		I2C_Write(slaveAddr<<1);
		I2C_Write(BuffAddr>>8);
		I2C_Write(BuffAddr);
		I2C_Restart();
		I2C_Write(slaveAddr<<1|1);
		dat=I2C_nack();
		I2C_Stop();
		return dat;
}

void i2c_eeprom_pagewrite(u8 slaveAddr,
	                        u16 wBufStartAddr,
                          s8 *p,
                          u8 nBytes) 
{ 
  u8 i; 
  I2C_Start();	 
  I2C_Write(slaveAddr<<1);
	I2C_Write(wBufStartAddr>>8);	
  I2C_Write(wBufStartAddr);   
  for(i=0;i<nBytes;i++) 
  { 
   I2C_Write(p[i]);              
  } 
  I2C_Stop(); 
    delay_ms(10); 
}


void i2c_eeprom_seqread(u8 slaveAddr,
	                      u16 rBufStartAddr,
                        s8 *p,
                        u8 nBytes) 
{ 
   u8 i; 
   I2C_Start();	 
   I2C_Write(slaveAddr<<1);
	I2C_Write(rBufStartAddr>>8);
   I2C_Write(rBufStartAddr); 
   I2C_Restart();	 
   I2C_Write(slaveAddr<<1|1);
   for(i=0;i<nBytes-1;i++) 
   { 
     p[i]=I2C_mack();	 
   }
   p[i]=I2C_nack(); 
   I2C_Stop(); 
} 




///types.h
typedef unsigned char u8;
typedef char s8;
typedef unsigned short int u16;
typedef short int s16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;

//delay.h
#include"types.h"
void delay_us(u32);
void delay_ms(u32);
void delay_s(u32);

//lcd.h
#include"types.h"
void init_LCD(void);
void cmdLCD(u8);
void displayLCD(u8);
void charLCD(u8);
void stringLCD(char *);
void integerLCD(s32);
void floatLCD(f32);
void custCharDegree(void);

//KeyPad.h
#include<lpc21xx.h>
#include "delay.h"
#include "types.h"
#include "defines.h"

#define R0  16
#define R1  17
#define R2  18
#define R3  19

#define C0	20
#define C1	21
#define C2	22
#define C3	23

//defines.h
#define SETBIT(WORD,BP) WORD|=(1<<BP)
#define CLRBIT(WORD,BP) WORD&=~(1<<BP)
#define CPLBIT(WORD,BP) WORD^=(1<<BP)
#define WRITEBIT(WORD,BP,BIT) WORD=((WORD&~(1<<BP))|(BIT<<BP)
#define WRITENIBBLE(WORD,SBP,NIBBLE) WORD=((WORD&~(0xf<<SBP))|(NIBBLE<<SBP))
#define WRITEBYTE(WORD,SBP,BYTE) WORD=((WORD&~(0xff<<SBP))|(BYTE<<SBP))
#define WRITEHWORD(WORD,SBP,HWORD) WORD=((WORD&~(0xffff<<SBP))|(HWORD<<SBP))

//lcd_defines.h
#define LCD_DAT 0xff
#define RS 6
#define RW 5
#define EN 7

///RTC_defines.h
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define PREINT_VAL ((int)(PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))

#define RTC_ENABLE (1<<0)
#define RTC_RESET (1<<1)
#define CLK_SRC (1<<4)


///keypad_defines.h
#include"types.h"
void KeyPdInit(void);
u8 ColStat(void);
u8 KeyVal(void);
u32 readnum(void);

//menu.h
void DisplayMenu(void);
void implementMenu(void);

//I2C.H
void Init_I2C(void);
void I2C_Start(void);
void I2C_Restart(void);
void I2C_Stop(void);
void I2C_Write(unsigned char);
unsigned char I2C_nack(void);
unsigned char I2C_mack(void);

//i2c_eeprom.h
#include "types.h"

void I2C_eeprom_ByteWrite(u8,u16,u8);
u8 I2C_eeprom_randomRead(u8,u16);
void i2c_eeprom_pagewrite(u8,u16,s8 *,u8); 
void i2c_eeprom_seqread(u8,u16,s8 *,u8); 


//I2C_eeprom_defines.h

#define I2C_EEPROM_SA1 0X50
#define I2C_EEPROM_SA2 0X51
#define I2C_EEPROM_SA3 0X52
#define I2C_EEPROM_SA4 0X53
#define I2C_EEPROM_SA5 0X54
#define I2C_EEPROM_SA6 0X55
#define I2C_EEPROM_SA7 0X56
#define I2C_EEPROM_SA8 0X57


//I2C_defines.h
#define SCL_PIN (1<<4)
#define SDA_PIN	(1<<6)

#define FOSC 					12000000
#define CCLK 					(FOSC*5)
#define PCLK 					(CCLK/4)
#define I2C_SPEED		 	100000
#define I2C_DIVIDER 	((PCLK/I2C_SPEED)/2)

#define AA_BIT		2
#define SI_BIT		3
#define STO_BIT		4
#define STA_BIT		5
#define I2EN_BIT	6

#define AAC_BIT		2
#define SIC_BIT		3
#define STAC_BIT	5
#define I2ENC_BIT	6



//
