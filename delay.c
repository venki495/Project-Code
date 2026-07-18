#include"delay.h"
#include"types.h"

void delay_us(u32 tdly)
{
    tdly *= 12;          // Convert microseconds to loop count (for processor clock)
    while(tdly--);       // Busy-wait loop
}

void delay_ms(u32 tdly)
{
    tdly *= 12000;       // Convert milliseconds to loop count
    while(tdly--);       // Busy-wait loop
}

void delay_s(u32 tdly)
{
    tdly *= 12000000;    // Convert seconds to loop count
    while(tdly--);       // Busy-wait loop
}

