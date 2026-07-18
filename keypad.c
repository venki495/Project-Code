#include<LPC21xx.h>
#include"keypad.h"
#include"lcd.h"

u8 LUT[][3]={1,2,3,
			4,5,6,
             7,8,9,
			'C','=',0};

void KeyPdInit(void)
{
		IODIR1|=((1<<R0)|(1<<R1)|(1<<R2)|(1<<R3));
	    //P1.16-P1.19 are for output functionality
		IOCLR1=((1<<R0)|(1<<R1)|(1<<R2)|(1<<R3));
		//Initializing rows to 0

}
// Check if any key is pressed
u8 ColStat(void)
{
    if((((IOPIN1>>20)&0x0f)==0x0f))
        return 1;                      // No key pressed
    else
        return 0;                      // Key pressed
}

// Scan keypad and return the pressed key
u8 KeyVal(void)
{
    char row_val=0,col_val=0;
    IOCLR1=(1<<R0);                    // Activate row 0
    IOSET1=((1<<R1)|(1<<R2)|(1<<R3));
    if((((IOPIN1>>20)&0x0f)!=0x0f))
    {
        row_val=0;                     // Row 0 detected
        goto colcheck;
    }
    IOCLR1=(1<<R1);                    // Activate row 1
    IOSET1=((1<<R0)|(1<<R2)|(1<<R3));
    if((((IOPIN1>>20)&0x0f)!=0x0f))
    {
        row_val=1;                     // Row 1 detected
        goto colcheck;
    }
    IOCLR1=(1<<R2);                    // Activate row 2
    IOSET1=((1<<R0)|(1<<R1)|(1<<R3));
    if((((IOPIN1>>20)&0x0f)!=0x0f))
    {
        row_val=2;                     // Row 2 detected
        goto colcheck;
    }
    IOCLR1=(1<<R3);                    // Activate row 3
    IOSET1=((1<<R0)|(1<<R1)|(1<<R2));
    if((((IOPIN1>>20)&0x0f)!=0x0f))
        row_val=3;                     // Row 3 detected

colcheck:
    if(((IOPIN1>>C0)&1)==0)
        col_val=0;                     // Column 0 detected
    else if(((IOPIN1>>C1)&1)==0)
        col_val=1;                     // Column 1 detected
    else if(((IOPIN1>>C2)&1)==0)
        col_val=2;                     // Column 2 detected
    else
        col_val=3;                     // Column 3 detected
    IOCLR1=((1<<R0)|(1<<R1)|(1<<R2)|(1<<R3));   // Reset all rows
    return LUT[row_val][col_val];      // Return corresponding key
}

// Read a number from the keypad
u32 readnum()
{
    u32 ch=0,key=0;
    while(1)
    {
        while(ColStat());              // Wait for key press
        key=KeyVal();                  // Read key value
        while(!ColStat());             // Wait for key release
        if(key=='=')
            break;                     // End input
        else if(key=='C')
        {
            if(ch>0)
            {
                ch/=10;                // Delete last digit
                cmdLCD(0x10);          // Move cursor left
                charLCD(' ');          // Erase character
                cmdLCD(0x10);          // Move cursor back
            }
        }
        else
        {
            integerLCD(key);           // Display entered digit
            ch=(ch*10)+key;            // Store entered digit
        }
    }
    return ch;                         // Return entered number
}
