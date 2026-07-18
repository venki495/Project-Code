#include <lpc214x.h>
#include "delay.h"
#include "uart.h"
#include "gsm.h"
void gsm_init(void)
{
    UART0_Str("AT\r\n");
    delay_ms(1000);

    UART0_Str("ATE0\r\n");        // Disable echo
    delay_ms(1000);

    UART0_Str("AT+CMGF=1\r\n");   // Text mode
    delay_ms(1000);

    UART0_Str("AT+CNMI=2,2,0,0,0\r\n");   // New SMS indication
    delay_ms(1000);
	UART0_Str("AT+CMGD=1\r\n");
}
//functon to send sms
void gsm_send_sms(char *num, char *msg)
{
    UART0_Str("AT+CMGS=\"");
    UART0_Str(num);
    UART0_Str("\"\r");

    delay_ms(2000);

    UART0_Str(msg);

    UART0_Tx(0x1A);   // CTRL + Z to send SMS
    delay_ms(5000);
}
//function to read sms
void gsm_read_sms(void)
{
    UART0_Str("AT+CMGR=1\r");
    delay_ms(2000);
}
 

