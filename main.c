#include <LPC21xx.h>				                                                                        
#include <string.h>
#include "rtc.h"
#include "lcd.h"
#include "keypad_defines.h"
#include "delay.h"
#include "menu.h"
#include "i2c.h"
#include "i2c_eeprom.h"
#include "i2c_eeprom_defines.h"
#include "dht11.h"
#include "uart.h"

int pwd;
char pass[5]="1234";
int p;
char lastTemp = 0;
char tempSPoint=37;
char humiditySPoint=70;
char mobile_number[15] = "9550673303";

void gsm_init(void);
void Enable_EINT0(void);
void itoa(int,char *,int);
unsigned char humidity_integer, humidity_decimal, temp_integer, temp_decimal, checksum;

// Convert a numeric string to an integer
int myatoi(const char *str)
{
    int i=0;
    int num=0;
    int sign=1;
    if(str[i]=='-'){
        sign=-1;                  // Check for negative sign
        i++;
    }
    else if(str[i]=='+'){
        i++;                      // Skip positive sign
    }
    while(str[i]>='0' && str[i]<='9'){
        num = num*10 + (str[i]-'0');  // Convert character to digit
        i++;
    }
    return sign * num;            // Return final integer value
}

// Verify the entered password with the stored EEPROM password
int verifyPWD(void)
{
    cmdLCD(0x01);                              // Clear LCD display
    cmdLCD(0x80);                              // Move cursor to first line
    stringLCD("PASSWORD: ");                   // Display password prompt
    cmdLCD(0xC0);                              // Move cursor to second line
    pwd = readnum();                           // Read password from keypad
    i2c_eeprom_seqread(I2C_EEPROM_SA1,0x00,pass,4); // Read stored password from EEPROM
    delay_ms(10);                              // Wait for EEPROM read completion
    pass[4] = '\0';                            // Null-terminate the password string
    p = myatoi(pass);                          // Convert password string to integer
    return (pwd == p);                         // Return 1 if passwords match, else 0
}

// Main program entry point
int main()
{
    delay_s(10);                                      // Initial power-on delay

    init_LCD();                                       // Initialize LCD
    init_RTC();                                       // Initialize RTC
    KeyPdInit();                                      // Initialize keypad
    Init_I2C();                                       // Initialize I2C
    InitUART0();                                      // Initialize UART0

    delay_s(5);                                       // Wait before GSM initialization
    gsm_init();                                       // Initialize GSM module

    IODIR0 |= 1<<17;                                  // Configure humidity control pin as output
    IODIR0 |= 1<<18;                                  // Configure temperature control pin as output

    Enable_EINT0();                                   // Enable external interrupt

    set_RTC_Time(10,20,0);                            // Set initial RTC time
    set_RTC_Date(12,02,2026);                         // Set initial RTC date

    i2c_eeprom_pagewrite(I2C_EEPROM_SA1,0x00,pass,4); // Store password in EEPROM
    delay_ms(10);

    I2C_eeprom_ByteWrite(I2C_EEPROM_SA1,0x20,tempSPoint);      // Store temperature set point
    delay_ms(10);

    I2C_eeprom_ByteWrite(I2C_EEPROM_SA1,0x30,humiditySPoint);  // Store humidity set point
    delay_ms(10);

    i2c_eeprom_pagewrite(I2C_EEPROM_SA1,0x40,mobile_number,10);// Store mobile number
    delay_ms(10);

    while(1)
    {
        handle_uart_sms();                            // Process received SMS commands
        display_RTC_Time_On_LCD(HOUR,MIN,SEC);        // Display current time
        display_RTC_Date_On_LCD(DOM,MONTH,YEAR);      // Display current date
        display_Temp_Hum();                           // Display labels
        dht11_request();                              // Send request to DHT11
        dht11_response();                             // Wait for sensor response
        humidity_integer = dht11_data();              // Read humidity integer
        humidity_decimal = dht11_data();              // Read humidity decimal
        temp_integer = dht11_data();                  // Read temperature integer
        temp_decimal = dht11_data();                  // Read temperature decimal
        checksum = dht11_data();                      // Read checksum
        // Verify sensor data
        if((humidity_integer + humidity_decimal + temp_integer + temp_decimal) != checksum)
        {
            cmdLCD(0x01);                             // Clear LCD
            stringLCD("Checksum Error");              // Display error
            delay_s(1);
            continue;
        }
        // Display humidity
        cmdLCD(0x89);
        stringLCD("H:");
        integerLCD(humidity_integer);
        charLCD('.');
        integerLCD(humidity_decimal);
        stringLCD("%RH");
        // Display temperature
        cmdLCD(0xC9);
        stringLCD("T:");
        integerLCD(temp_integer);
        charLCD('.');
        integerLCD(temp_decimal);
        charLCD(223);                                // Degree symbol
        stringLCD("C");
        delay_ms(900);
        // Check temperature threshold
        if(temp_integer > tempSPoint)
        {
            if(temp_integer != lastTemp)
            {
                IOCLR0 = 1<<18;                       // Turn ON temperature output
                UART0_Str("AT+CMGF=1\r\n");           // Set SMS text mode
                delay_ms(1000);
                UART0_Str("AT+CMGS=\"");             // Send SMS command
                UART0_Str(mobile_number);
                UART0_Str("\"\r\n");
                delay_ms(1000);
                UART0_Str("ALERT: TEMP HIGH TEMP: ");
                UART0_Int(temp_integer);
                UART0_Tx(' ');
                UART0_Tx('C');
                UART0_Tx('\n');
                UART0_Str("TIME: ");                  // Send current time
                UART0_Tx((HOUR/10)+48);
                UART0_Tx((HOUR%10)+48);
                UART0_Tx(':');
                UART0_Tx((MIN/10)+48);
                UART0_Tx((MIN%10)+48);
                UART0_Tx(':');
                UART0_Tx((SEC/10)+48);
                UART0_Tx((SEC%10)+48);
                UART0_Tx('\t');
                UART0_Str("Date: ");                  // Send current date
                UART0_Tx((DOM/10)+48);
                UART0_Tx((DOM%10)+48);
                UART0_Tx('/');
                UART0_Tx((MONTH/10)+48);
                UART0_Tx((MONTH%10)+48);
                UART0_Tx('/');
                UART0_Int(YEAR%100);
                UART0_Tx(0x1A);                       // Send SMS
                delay_s(3);
                UART0_Str("\r\n");
                UART0_Str("AT+CMGD=1\r\n");           // Delete SMS from memory
                lastTemp = temp_integer;             // Update last temperature
            }
        }
        else
        {
            lastTemp = 0;                            // Reset last temperature
            IOSET0 = 1<<18;                          // Turn OFF temperature output
            delay_ms(200);
        }

        // Check humidity threshold
        if(humidity_integer > humiditySPoint)
        {
            IOCLR0 = 1<<17;                          // Turn ON humidity output
            delay_ms(200);
        }
        else
        {
            IOSET0 = 1<<17;                          // Turn OFF humidity output
            delay_ms(200);
        }
    }
}
