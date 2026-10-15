# PROJECT OVERVIEW
The Secure GSM-Based Thermal Monitoring and Set-Point Control System is an embedded project based on the
LPC2148 ARM7 microcontroller. It reads temperature and humidity from a DHT11 sensor, displays information on an
LCD, and compares temperature readings against a configurable threshold. When the threshold is exceeded, the
firmware can send an SMS notification through the M660A GSM module, provided the module is registered and
communication is working. The system also uses an AT24C256 EEPROM for persistent settings, a keypad for local input, an RTC for date/time where supported, and UART/I2C/GPIO interfaces for peripheral communication. Password-protected SMS commands
may be used to restrict remote configuration to authorized users.

<img width="506" height="146" alt="Screenshot 2026-10-15 124752" src="https://github.com/user-attachments/assets/103ced3a-f554-4b7d-a049-cfd437c10f57" />



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


# HARDWRAE REQUIREMENTS

<img width="603" height="299" alt="Screenshot 2026-10-15 114507" src="https://github.com/user-attachments/assets/b9d39d9b-7ec4-4c70-9d5a-8a8f2206cc99" />


# SOFTWARE REQUIREMENTS

<img width="605" height="220" alt="Screenshot 2026-10-15 114628" src="https://github.com/user-attachments/assets/57f48d90-5623-439d-b174-455491e82bd1" />

Power and logic caution: Check each module's datasheet for supply voltage and logic-level requirements. The LPC2148 uses 3.3 V I/O.GSM modules can require a supply capable of handling high current peaks.


# PIN CONNECTIONS 

<img width="530" height="317" alt="Screenshot 2026-10-15 115803" src="https://github.com/user-attachments/assets/78ebdc26-b3ee-4cf0-bba9-05dfbfe18246" />

Pin verification checklist:

▸ Confirm UART0 peripheral and pin multiplexing in PINSEL0 registers.

▸ Confirm I2C SDA/SCL pins and EEPROM address from the code and datasheet.

▸ Confirm LCD wiring mode and all GPIO assignments.

▸ Confirm keypad row/column order and active levels.

▸ Confirm external interrupt pin and ISR configuration.

▸ Verify common ground and voltage compatibility across modules.

# WORKING PRINCIPLE

 <img width="536" height="339" alt="Screenshot 2026-10-15 120105" src="https://github.com/user-attachments/assets/7b718599-9698-4083-a18d-a78d05c78e7c" />

 # OUTPUT

<img width="1139" height="734" alt="n" src="https://github.com/user-attachments/assets/65b5a9be-e98e-4fed-956b-756b66dfa905" />

 
 # 1.LCD Reading

<img width="205" height="69" alt="Screenshot 2026-10-15 120315" src="https://github.com/user-attachments/assets/a0207b36-d64a-4516-8e7a-e74adcc4ba00" />


 # 2.Keypad Menu
 
 when the interrupt occured first
 
<img width="858" height="367" alt="v" src="https://github.com/user-attachments/assets/dc860e6c-28ca-4597-a813-19c0688d101c" />

selecting the change set point

<img width="694" height="276" alt="&#39;" src="https://github.com/user-attachments/assets/0c825911-85bc-429a-9a45-efc13de17290" />



 # 3.SMS Alert

 <img width="406" height="120" alt="L" src="https://github.com/user-attachments/assets/620ef0e5-9247-4949-a241-602d0a4739ca" />


 # 4.Authorized command status

<img width="350" height="19" alt="Screenshot 2026-10-15 124052" src="https://github.com/user-attachments/assets/83a14256-c37e-4735-9075-5e935752460a" />

EXAMPLES:

COMMAND 1:

<img width="148" height="111" alt="Q" src="https://github.com/user-attachments/assets/832dc609-519f-4409-a6a8-b48ad9f411f3" />

OUTPUT:

<img width="313" height="105" alt="K" src="https://github.com/user-attachments/assets/7a851c97-37de-451c-bbf7-8dd473f4d017" />

COMMAND 2:

<img width="182" height="85" alt="-" src="https://github.com/user-attachments/assets/41082f43-2248-4d97-a703-121429e53445" />

OUTPUT:

<img width="406" height="120" alt="L" src="https://github.com/user-attachments/assets/f1458ce6-9709-4898-aa63-6e042f0954a6" />

COMMAND 3:

<img width="305" height="81" alt="P" src="https://github.com/user-attachments/assets/e4f2daec-cfbb-44fa-8363-76e8deddfe81" />

OUTPUT:

<img width="406" height="120" alt="L" src="https://github.com/user-attachments/assets/feb042a1-e8e5-4a74-81a9-016a7c626d74" />

 # TEST CASES AND RESULTS
 <img width="578" height="382" alt="Screenshot 2026-10-15 121059" src="https://github.com/user-attachments/assets/fbe4a416-b024-4897-a96a-056e73512344" />
 
 # Evaluation Criteria
 
 ▸ Sensor readings update correctly and invalid readings are handled.
 
 ▸ Threshold comparison matches the implemented firmware logic.
 
 ▸ Saved EEPROM values persist after a power cycle.
 
 ▸ Invalid credentials do not change protected settings.
 
 ▸ SMS delivery is verified independently from the UART command response.
 
 ▸ Communication faults do not leave the system in an unsafe state.

 # APPLICATIONS

 <img width="577" height="206" alt="Screenshot 2026-10-15 121645" src="https://github.com/user-attachments/assets/84be18d4-e4b5-4bb5-981d-fc0cd04c4916" />

# FUTURE ENHANCEMENTS

<img width="578" height="335" alt="Screenshot 2026-10-15 122109" src="https://github.com/user-attachments/assets/41b32d31-b299-4bf1-b171-5f59731df915" />

# CONCLUSION

The Secure GSM-Based Thermal Monitoring and Set-Point Control System demonstrates how an LPC2148 ARM7
microcontroller can combine sensor acquisition, local display, persistent configuration, and GSM-based remote alerts.
It offers hands-on experience with Embedded C, UART, I2C, GPIO, interrupt handling, and peripheral integration. Final
claims about exact pin mappings, output screenshots, and test results should be updated to match the actual hardware
and firmware.

 



