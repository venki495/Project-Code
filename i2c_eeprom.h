#include "types.h"

void I2C_eeprom_ByteWrite(u8,u16,u8);
u8 I2C_eeprom_randomRead(u8,u16);
void i2c_eeprom_pagewrite(u8,u16,s8 *,u8); 
void i2c_eeprom_seqread(u8,u16,s8 *,u8); 

