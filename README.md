# PROJECT OVERVIEW
The Secure GSM-Based Thermal Monitoring and Set-Point Control System is an embedded project based on the
LPC2148 ARM7 microcontroller. It reads temperature and humidity from a DHT11 sensor, displays information on an
LCD, and compares temperature readings against a configurable threshold. When the threshold is exceeded, the
firmware can send an SMS notification through the M660A GSM module, provided the module is registered and
communication is working. The system also uses an AT24C256 EEPROM for persistent settings, a keypad for local input, an RTC for date/time where supported, and UART/I2C/GPIO interfaces for peripheral communication. Password-protected SMS commands
may be used to restrict remote configuration to authorized users.


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

