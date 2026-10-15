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

# BLOCK DIAGRAM
The diagram below summarizes the main functional blocks. It is conceptual: the exact pin assignments and peripheral
choices should be confirmed from the circuit schematic and source code.
<img width="563" height="328" alt="Screenshot 2026-10-15 104908" src="https://github.com/user-attachments/assets/c4b97b36-118e-48b4-8126-d3fd387c0d61" />
<img width="613" height="257" alt="Screenshot 2026-10-15 114018" src="https://github.com/user-attachments/assets/e92d6f0c-0616-47d8-be9d-963cf9d3e6c4" />


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

<img width="603" height="299" alt="Screenshot 2026-10-15 114507" src="https://github.com/user-attachments/assets/b9d39d9b-7ec4-4c70-9d5a-8a8f2206cc99" />


# SOFTWARE REQUIREMENTS:

<img width="605" height="220" alt="Screenshot 2026-10-15 114628" src="https://github.com/user-attachments/assets/57f48d90-5623-439d-b174-455491e82bd1" />
