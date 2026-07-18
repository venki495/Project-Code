#include <LPC21xx.h>
#include <string.h>
#include <stdlib.h>

#include "delay.h"
#include "i2c_eeprom.h"
#include "i2c_eeprom_defines.h"

#define MAX_BUF 200

// ================= GLOBALS =================

volatile char rx_buf[MAX_BUF];
volatile unsigned int rx_index = 0;
volatile unsigned char msg_ready = 0;
volatile unsigned char sms_started = 0;

extern char pass[5];
extern char mobile_number[15];
char new_number[15];

extern unsigned char tempSPoint;
extern unsigned char humiditySPoint;

extern unsigned char temp_integer;
extern unsigned char humidity_integer;

char sender_number[15];
char sms_data[100];

// ================= UART0 ISR =================

void UART0_ISR(void) __irq
{
    unsigned char ch;
    unsigned int int_id;

    static char detect_buf[6];
    static int detect_index =  0;

    int_id = (U0IIR >> 1) & 0x07;

    if (int_id == 0x02 || int_id == 0x06)
    {
        ch = U0RBR;

        // Detect "+CMT:"
        if (!sms_started)
        {
            if (detect_index < 5)
            {
                detect_buf[detect_index++] = ch;
            }
            else
            {
                memmove(detect_buf, detect_buf + 1, 4);
                detect_buf[4] = ch;
            }

            detect_buf[5] = '\0';

            if (strstr(detect_buf, "+CMT:"))
            {
                sms_started = 1;

                rx_index = 0;

                strcpy((char *)rx_buf, "+CMT:");

                rx_index = 5;
            }
        }
        else
        {										  
            if (rx_index < MAX_BUF - 1)
            {
                rx_buf[rx_index++] = ch;
                rx_buf[rx_index] = '\0';

                if (ch == '$')
                {
                    msg_ready = 1;
                    sms_started = 0;
                }
            }
            else
            {
                rx_index = 0;
                sms_started = 0;
            }
        }
    }

    VICVectAddr = 0;
}

// ================= UART INIT =================

void InitUART0(void)
{
    PINSEL0 |= 0x00000005;

    U0LCR = 0x83;

    // 9600 baud @ 15MHz PCLK
    U0DLL = 97;
    U0DLM = 0;

    U0LCR = 0x03;

    // UART0 Interrupt Enable
    VICIntSelect &= ~(1 << 6);

    VICVectAddr1 = (unsigned long)UART0_ISR;
    VICVectCntl1 = (1 << 5) | 6;

    VICIntEnable |= (1 << 6);

    U0IER = 0x01;
}

// ================= UART TX =================

void UART0_Tx(char ch)
{
    U0THR = ch;

    while (!(U0LSR & 0x20));
}

void UART0_Str(char *s)
{
    while (*s)
    {
        UART0_Tx(*s++);
    }
}

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
    {
        UART0_Tx(buf[i]);
    }
}

// ================= CLEAR RX BUFFER =================

void clear_rx_buffer(void)
{
    rx_index = 0;
    msg_ready = 0;

    memset((char *)rx_buf, 0, sizeof(rx_buf));
    memset(sender_number, 0, sizeof(sender_number));
    memset(sms_data, 0, sizeof(sms_data));
}

// ================= EXTRACT SENDER =================

void extract_sender(volatile char *msg)
{
    char *start;
    char *end;
    int len;

    sender_number[0] = '\0';

    start = strstr((char *)msg, "+CMT:");

    if (start)
    {
        start = strchr(start, '\"');

        if (start)
        {
            start++;

            end = strchr(start, '\"');

            if (end)
            {
                len = end - start;

                if (len < sizeof(sender_number))
                {
                    strncpy(sender_number, start, len);
                    sender_number[len] = '\0';
                }
            }
        }
    }
}

// ================= EXTRACT SMS =================

void extract_sms(volatile char *msg)
{
    char *p;

    sms_data[0] = '\0';

    p = strstr((char *)msg, "\r\n");

    if (p)
    {
        p += 2;

        strncpy(sms_data, p, sizeof(sms_data) - 1);

        sms_data[sizeof(sms_data) - 1] = '\0';

        // Remove CR/LF
        sms_data[strcspn(sms_data, "\r\n")] = '\0';
    }
}

// ================= AUTHORIZATION =================

int is_authorized(void)
{
    int len1 = strlen(sender_number);
    int len2 = strlen(mobile_number);

    if (len1 >= 10)
    {
        return (strcmp(&sender_number[len1 - 10], mobile_number) == 0);
    }

    return 0;
}

// ================= NUMERIC CHECK =================

int is_numeric(char *s)
{
    while (*s)
    {
        if (*s < '0' || *s > '9')
        {
            return 0;
        }

        s++;
    }

    return 1;
}

// ================= VALIDATE SYNTAX =================

int validate_syntax(char *msg)
{
    int len;
    char cmd;
    char data[20];

    len = strlen(msg);

    if (len < 6)
    {
        return 0;
    }

    if (msg[len - 1] != '$')
    {
        return 0;
    }

    cmd = msg[4];

    strncpy(data, &msg[5], sizeof(data) - 1);
    data[sizeof(data) - 1] = '\0';

    // Remove $
    data[strcspn(data, "$")] = '\0';

    switch (cmd)
    {
        case 'T':

            if (!is_numeric(data))
            {
                return 0;
            }

            break;

        case 'M':

            if (strlen(data) != 10)
            {
                return 0;
            }

            if (!is_numeric(data))
            {
                return 0;
            }

            break;

        case 'I':
            break;

        default:
            return 0;
    }

    return 1;
}

// ================= DELETE SMS =================

void delete_sms(void)
{
    UART0_Str("AT+CMGD=1,4\r\n");
    delay_ms(1000);
}

// ================= SEND ALERT =================

void send_alert(void)
{
    UART0_Str("AT+CMGF=1\r\n");
    delay_ms(1000);

    UART0_Str("AT+CMGS=\"");
    UART0_Str(mobile_number);
    UART0_Str("\"\r\n");

    delay_ms(1000);

    UART0_Str("Unauthorized access attempt from");
	UART0_Str(&sender_number[2]);
	UART0_Str("\n");

    UART0_Tx(0x1A);

    delay_ms(3000);
}

// ================= PROCESS MESSAGE =================

void process_message(char *msg)
{
    char pwd[5];
    char data[20];
    char cmd;

    strncpy(pwd, msg, 4);

    pwd[4] = '\0';
 
    if (strcmp(pwd, pass) != 0)
    {
        return;
    }

    cmd = msg[4];

    strncpy(data, &msg[5], sizeof(data) - 1);

    data[sizeof(data) - 1] = '\0';

    // Remove $
    data[strcspn(data, "$")] = '\0';

    switch (cmd)
    {
        // ================= TEMPERATURE SETPOINT =================

        case 'T':

            //tempSPoint = atoi(data);

            I2C_eeprom_ByteWrite(I2C_EEPROM_SA1,
                                 0x20,
                                 atoi(data));

            delay_ms(10);
			tempSPoint=I2C_eeprom_randomRead(I2C_EEPROM_SA1,0x20);
            break;

        // ================= MOBILE NUMBER =================

        case 'M':

            strncpy(new_number, data, 10);

            new_number[10] = '\0';

            i2c_eeprom_pagewrite(I2C_EEPROM_SA1,
                                 0x40,
                                 new_number,
                                 11);

            delay_ms(10);
			i2c_eeprom_seqread(I2C_EEPROM_SA1,0x40,mobile_number,10);
            break;

        // ================= SEND STATUS =================

        case 'I':

            UART0_Str("AT+CMGF=1\r\n");
            delay_ms(1000);

            UART0_Str("AT+CMGS=\"");
            UART0_Str(mobile_number);
            UART0_Str("\"\r\n");

            delay_ms(1000);

            UART0_Str("Temp=");
            UART0_Int(temp_integer);

            UART0_Str(" Humidity=");
            UART0_Int(humidity_integer);

            UART0_Tx(0x1A);

            delay_ms(3000);

            break;
    }

    delete_sms();
}

// ================= MAIN SMS HANDLER =================

void handle_uart_sms(void)
{
    if (msg_ready)
    {
        extract_sender(rx_buf);

        extract_sms(rx_buf);

        if (strlen(sms_data) == 0)
        {
            delete_sms();
            clear_rx_buffer();
            return;
        }

        // Check authorized user
        if (!is_authorized())
        {
            send_alert();

            delete_sms();

            clear_rx_buffer();

            return;
        }

        // Validate syntax
        if (validate_syntax(sms_data))
        {
            process_message(sms_data);
        }
        else
        {
            delete_sms();
        }

        clear_rx_buffer();
    }
}
