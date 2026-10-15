# DESCRIPTION 
Secure GSM-Based Thermal Monitoring and Set-Point Control System using LPC2148 (ARM7). Monitors temperature and humidity with a DHT11 sensor, stores secure settings in EEPROM via I2C, displays data on an LCD, maintains time using an RTC, and sends SMS alerts through a GSM module when temperature and humidity exceeds the configured threshold.


# OBJECTIVES 
▸ Monitor temperature and relative humidity in real time.

▸ Compare sensor readings with predefined set-points.

▸ Display sensor readings and system status on an LCD.

▸ Generate fault indications using an LED or buzzer.

▸ Send SMS alerts to an authorized mobile number.

▸ Store set-points, passwords, and mobile numbers in EEPROM.

▸ Support secure remote configuration using SMS commands.

▸ Allow local set-point and password changes through a keypad.

▸ Use UART interrupts for GSM communication.

▸ Include timestamps in fault notifications using the RTC.


# FEATURES 

▸ Real-Time Monitoring: Continuously reads temperature and humidity from the DHT11 sensor.

▸ Set-Point Comparison: Detects temperature values exceeding the configured limit.

▸ SMS Alerts: Notifies authorized users when abnormal conditions occur.

▸ Password-Protected Commands: Validates SMS commands before performing configuration changes.

▸ Remote Set-Point Modification: Changes the temperature limit through SMS.

▸ Mobile Number Management: Updates the registered alert recipient through an authenticated command.

▸ Remote Sensor Information: Sends current sensor readings through SMS.

▸ Non-Volatile Storage: Preserves configuration data in AT24C256 EEPROM.

▸ Local Configuration: Uses a keypad and LCD to modify settings.

▸ Failed Login Protection: Provides fault indication for incorrect passwords and temporary blocking after three consecutive incorrect local attempts.

▸ Interrupt-Based Communication: Uses UART interrupts for GSM data reception.

▸ Timestamped Alerts: Uses the RTC to associate fault notifications with the time of occurrence.


# HARDWRAE REQUIREMENTS
• LPC2148

• LCD

• AT24C256

• DTH11

• 4X4 MATRIX KEYPAD

• SWITCH

• LED’S

• GSM MODULE (M660A)

• DB-9 CABLE/USB-UART CONVERTER

# SOFTWARE REQUIREMENTS:
• KEIL C Compiler

• PROGRAMMING IN EMBEDDED C

• Flash Magic

