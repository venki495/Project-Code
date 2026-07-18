#include <LPC21xx.h>
#include "i2c_defines.h"

// Initialize I2C peripheral
void Init_I2C(void){
    PINSEL0 |= ((SCL_PIN) | (SDA_PIN));   // Configure SCL and SDA pins
    I2SCLL = I2C_DIVIDER;                 // Set SCL low time
    I2SCLH = I2C_DIVIDER;                 // Set SCL high time
    I2CONSET = 1 << I2EN_BIT;            // Enable I2C module
}

// Generate I2C start condition
void I2C_Start(void){
    I2CONSET = 1 << STA_BIT;             // Send START condition
    while(((I2CONSET >> SI_BIT) & 1) == 0); // Wait for START completion
    I2CONCLR = 1 << STAC_BIT;           // Clear START flag
}

// Generate I2C repeated start condition
void I2C_Restart(void){
    I2CONSET = 1 << STA_BIT;            // Send repeated START
    I2CONCLR = 1 << SIC_BIT;            // Clear interrupt flag
    while(((I2CONSET >> SI_BIT) & 1) == 0); // Wait for completion
    I2CONCLR = 1 << STAC_BIT;           // Clear START flag
}

// Generate I2C stop condition
void I2C_Stop(void){
    I2CONSET = 1 << STO_BIT;            // Send STOP condition
    I2CONCLR = 1 << SIC_BIT;            // Clear interrupt flag
}

// Write one byte on I2C bus
void I2C_Write(unsigned char data){
    I2DAT = data;                       // Load data into I2C data register
    I2CONCLR = 1 << SIC_BIT;            // Clear interrupt flag
    while(((I2CONSET >> SI_BIT) & 1) == 0); // Wait for transmission
}

// Read one byte and send NACK
unsigned char I2C_nack(void){
    I2CONCLR = 1 << SIC_BIT;            // Clear interrupt flag
    while(((I2CONSET >> SI_BIT) & 1) == 0); // Wait for data reception
    return I2DAT;                       // Return received data
}

// Read one byte and send ACK
unsigned char I2C_mack(void){
    I2CONSET = 1 << AA_BIT;             // Send ACK
    I2CONCLR = 1 << SIC_BIT;            // Clear interrupt flag
    while(((I2CONSET >> SI_BIT) & 1) == 0); // Wait for data reception
    I2CONCLR = 1 << AAC_BIT;            // Clear ACK bit
    return I2DAT;                       // Return received data
}


