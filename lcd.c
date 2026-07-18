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

// Create and store a custom degree symbol in CGRAM
void custCharDegree(void){
    u8 i, LUT[]={0x0e,0x11,0x11,0x0e,0x00,0x00,0x00,0x00};

    for(i=0;i<8;i++)
        charLCD(LUT[i]);          // Write each row of the custom character
}
