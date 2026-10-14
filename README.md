# Project-Code
Secure GSM-Based Thermal Monitoring and Set-Point Control System using LPC2148 (ARM7). Monitors temperature and humidity with a DHT11 sensor, stores secure settings in EEPROM via I2C, displays data on an LCD, maintains time using an RTC, and sends SMS alerts through a GSM module when temperature and humidity exceeds the configured threshold.


# Objectives

Monitor temperature and relative humidity in real time.

Compare sensor readings with predefined set-points.

Display sensor readings and system status on an LCD.

Generate fault indications using an LED or buzzer.

Send SMS alerts to an authorized mobile number.

Store set-points, passwords, and mobile numbers in EEPROM.

Support secure remote configuration using SMS commands.

Allow local set-point and password changes through a keypad.

Use UART interrupts for GSM communication.

Include timestamps in fault notifications using the RTC.