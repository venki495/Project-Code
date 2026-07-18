void InitUART0 (void); /* Initialize Serial Interface       */ 
void UART0_Tx(char ch);  
char UART0_Rx(void);
void UART0_Str(char *s);
void UART0_Int(int);
void UART0_Float(float f);
void handle_uart_sms(void);   
