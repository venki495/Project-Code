#include <LPC21xx.h>

#include "types.h"
#include "defines.h"
#include "menu.h"

#define EINT0_PIN      16	
#define EINT0_VIC_CHNO 14	

// EINT0 Interrupt Service Routine
void eint0_isr(void) __irq
{
    // Display menu on interrupt
    DisplayMenu();
    // Execute selected menu option
    implementMenu();
    // Clear EINT0 interrupt flag
    EXTINT |= (1<<0);
    // Indicate end of interrupt
    VICVectAddr = 0;
}

// Enable and configure External Interrupt 0
void Enable_EINT0(void)
{
    // Configure P0.16 as EINT0 pin
    PINSEL1 = ((PINSEL1&~(3<<0))|(1<<0));
    // Enable EINT0 interrupt in VIC
    VICIntEnable |= (1<<EINT0_VIC_CHNO);
    // Configure VIC vector slot for EINT0
    VICVectCntl0 = ((1<<5) | EINT0_VIC_CHNO);
    // Assign ISR address to vector slot
    VICVectAddr0 = (unsigned int)eint0_isr;
    // Set EINT0 as edge-triggered
    EXTMODE |= (1<<0);
    // Configure falling-edge trigger
    EXTPOLAR = 0X00;
}


