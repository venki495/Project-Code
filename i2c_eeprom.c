#include <LPC21XX.h>
#include "i2c.h"
#include "delay.h"

typedef unsigned char u8;
typedef unsigned short int u16;

// Write a single byte to EEPROM
void I2C_eeprom_ByteWrite(u8 slaveAddr,u16 BuffAddr,u8 data){
    I2C_Start();                        // Generate START condition
    I2C_Write(slaveAddr<<1);            // Send slave address with write bit
    I2C_Write(BuffAddr>>8);             // Send high byte of memory address
    I2C_Write(BuffAddr);                // Send low byte of memory address
    I2C_Write(data);                    // Write data byte
    I2C_Stop();                         // Generate STOP condition
}

// Read a single byte from EEPROM
u8 I2C_eeprom_randomRead(u8 slaveAddr,u16 BuffAddr){
    u8 dat;
    I2C_Start();                        // Generate START condition
    I2C_Write(slaveAddr<<1);            // Send slave address with write bit
    I2C_Write(BuffAddr>>8);             // Send high byte of memory address
    I2C_Write(BuffAddr);                // Send low byte of memory address
    I2C_Restart();                      // Generate repeated START
    I2C_Write(slaveAddr<<1|1);          // Send slave address with read bit
    dat = I2C_nack();                   // Read last byte and send NACK
    I2C_Stop();                         // Generate STOP condition
    return dat;                         // Return received data
}

// Write multiple bytes to EEPROM
void i2c_eeprom_pagewrite(u8 slaveAddr,
                          u16 wBufStartAddr,
                          s8 *p,
                          u8 nBytes)
{
    u8 i;
    I2C_Start();                        // Generate START condition
    I2C_Write(slaveAddr<<1);            // Send slave address with write bit
    I2C_Write(wBufStartAddr>>8);        // Send high byte of start address
    I2C_Write(wBufStartAddr);           // Send low byte of start address
    for(i=0;i<nBytes;i++)
    {
        I2C_Write(p[i]);                // Write each data byte
    }
    I2C_Stop();                         // Generate STOP condition
    delay_ms(10);                       // Wait for EEPROM write cycle
}

// Read multiple bytes from EEPROM
void i2c_eeprom_seqread(u8 slaveAddr,
                        u16 rBufStartAddr,
                        s8 *p,
                        u8 nBytes)
{
    u8 i;
    I2C_Start();                        // Generate START condition
    I2C_Write(slaveAddr<<1);            // Send slave address with write bit
    I2C_Write(rBufStartAddr>>8);        // Send high byte of start address
    I2C_Write(rBufStartAddr);           // Send low byte of start address
    I2C_Restart();                      // Generate repeated START
    I2C_Write(slaveAddr<<1|1);          // Send slave address with read bit
    for(i=0;i<nBytes-1;i++)
    {
        p[i] = I2C_mack();              // Read byte and send ACK
    }
    p[i] = I2C_nack();                  // Read last byte and send NACK
    I2C_Stop();                         // Generate STOP condition
}
