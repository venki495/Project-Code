#include <LPC21xx.h>
#include <string.h>
#include <stdlib.h>
#include "delay.h"
#include "i2c_eeprom.h"
#include "i2c_eeprom_defines.h"

#define MAX_BUF 200

volatile char rx_buf[MAX_BUF];
volatile int rx_index = 0;
volatile int msg_ready = 0;

extern char pass[5];
extern char mobile_number[15];
extern char tempSPoint;
extern char humiditySPoint;
extern unsigned char temp_integer, humidity_integer;

char sender_number[15];
char sms_data[100];


// ================= UART ISR =================
void UART0_ISR(void) __irq
{
    char ch;

    while (U0LSR & 0x01)
    {
        ch = U0RBR;

        if (rx_index < MAX_BUF - 1)
        {
            rx_buf[rx_index++] = ch;

            // End of user command
            if (ch == '$')
            {
                rx_buf[rx_index] = '\0';
                msg_ready = 1;
                break;
            }
        }
        else
        {
            rx_index = 0;
        }
    }

    VICVectAddr = 0;
}


// ================= UART INIT =================
void InitUART0(void)
{
    PINSEL0 |= 0x00000005;

    U0LCR = 0x83;
    U0DLL = 90;
    U0LCR = 0x03;

    VICIntSelect = 0x00;
    VICVectAddr0 = (unsigned long)UART0_ISR;
    VICVectCntl0 = 0x20 | 6;
    VICIntEnable = 1 << 6;

    U0IER = 0x01;
}


// ================= UART TX =================
void UART0_Tx(char ch)
{
    while (!(U0LSR & 0x20));
    U0THR = ch;
}


// ================= UART STRING =================
void UART0_Str(char *s)
{
    while (*s)
        UART0_Tx(*s++);
}


// ================= UART INTEGER =================
void UART0_Int(unsigned int n)
{
    char buf[10];
    int i = 0;

    if (n == 0)
    {
        UART0_Tx('0');
        return;
    }

    while (n > 0)
    {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }

    while (i--)
        UART0_Tx(buf[i]);
}


// ================= EXTRACT SENDER =================
void extract_sender(volatile char *msg)
{
    char *start;
    char *end;
    int len;

    sender_number[0] = '\0';

    start = strchr((char *)msg, '"');

    if (start)
    {
        end = strchr(start + 1, '"');

        if (end)
        {
            len = end - (start + 1);

            if (len >= sizeof(sender_number))
                len = sizeof(sender_number) - 1;

            strncpy(sender_number, start + 1, len);
            sender_number[len] = '\0';
        }
    }
}


// ================= EXTRACT SMS =================
void extract_sms(volatile char *msg)
{
    char *p;

    p = strchr((char *)msg, '\n');

    if (p)
    {
        p++;

        while (*p == '\r' || *p == '\n')
            p++;

        strncpy(sms_data, p, sizeof(sms_data)-1);
        sms_data[sizeof(sms_data)-1] = '\0';

        sms_data[strcspn(sms_data, "\r\n")] = '\0';
    }
    else
    {
        sms_data[0] = '\0';
    }
}


// ================= AUTHORIZATION CHECK =================
int is_authorized()
{
    int len;

    len = strlen(sender_number);

    if (len >= 10)
    {
        return strcmp(&sender_number[len - 10], mobile_number) == 0;
    }

    return 0;
}


// ================= VALIDATE SMS FORMAT =================
int validate_syntax(char *msg)
{
    int len;

    len = strlen(msg);

    if (len < 6)
        return 0;

    if (msg[len - 1] != '$')
        return 0;

    if (msg[4] != 'T' && msg[4] != 'M' && msg[4] != 'I')
        return 0;

    return 1;
}


// ================= DELETE SMS =================
void delete_sms()
{
    UART0_Str("AT+CMGD=1\r");
    delay_ms(1000);
}


// ================= UNAUTHORIZED ALERT =================
void send_alert()
{
    UART0_Str("AT+CMGF=1\r");
    delay_ms(1000);

    UART0_Str("AT+CMGS=\"");
    UART0_Str(mobile_number);
    UART0_Str("\"\r");
    delay_ms(1000);

    UART0_Str("Unauthorized access attempt");

    UART0_Tx(26);
    delay_ms(3000);
}


// ================= PROCESS SMS COMMAND =================
void process_message(char *msg)
{
    char pwd[5];
    char data[20];
    char cmd;
    int len;

    strncpy(pwd, msg, 4);
    pwd[4] = '\0';

    if (strcmp(pwd, pass) != 0)
        return;

    cmd = msg[4];

    len = strlen(msg);

    strncpy(data, &msg[5], len - 6);
    data[len - 6] = '\0';

    switch (cmd)
    {
        case 'T':
            tempSPoint = atoi(data);

            I2C_eeprom_ByteWrite(I2C_EEPROM_SA1, 0x20, tempSPoint);
            delay_ms(10);
            break;

        case 'M':
            if (strlen(data) == 10)
            {
                strncpy(mobile_number, data, 10);
                mobile_number[10] = '\0';

                i2c_eeprom_pagewrite(I2C_EEPROM_SA1, 0x40, mobile_number, 11);
                delay_ms(10);
            }
            break;

        case 'I':
            UART0_Str("AT+CMGF=1\r");
            delay_ms(1000);

            UART0_Str("AT+CMGS=\"");
            UART0_Str(mobile_number);
            UART0_Str("\"\r");
            delay_ms(1000);

            UART0_Str("Temp=");
            UART0_Int(temp_integer);

            UART0_Str(" Humidity=");
            UART0_Int(humidity_integer);

            UART0_Tx(26);
            delay_ms(3000);
            break;
    }
}


// ================= HANDLE RECEIVED SMS =================
void handle_uart_sms()
{
    if (msg_ready)
    {
        msg_ready = 0;

        extract_sender(rx_buf);
        extract_sms(rx_buf);

        if (strlen(sms_data) == 0)
        {
            rx_index = 0;
            delete_sms();
            return;
        }

        if (!is_authorized())
        {
            send_alert();
            rx_index = 0;
            delete_sms();
            return;
        }

        if (validate_syntax(sms_data))
        {
            process_message(sms_data);
        }

        rx_index = 0;
        delete_sms();
    }
}
