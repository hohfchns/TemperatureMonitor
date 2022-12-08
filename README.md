# Temperature Monitor Project

## Hardware Requirements

The program is based on the following hardware:

* STM Nucleo L476
* DHT11 Temperature/Humidity Sensor
* LED light
* SD Card + SD Card Reader
* Buzzer

## Features

The software has the following features as a requirement:

**Internal**

* Timed data updates
* Saving to files
* Temperature Threshold (Warning, Critical)
* LED activation
* Sound playing (via buzzer)
* Logging including recording time, date, severity
* Store settings in FLASH
* Storing on SD Card
* Usage of FreeRTOS as a base operating system

**User Exposed**

* Disabling sound
* Setting time and date
* Setting thresholds
* Printing and Clearing logs
* SD Card write format readable by Windows OS

## Use Cases

### Power Up
This use-case is occurs when the system is turned on

| ID          | Description                                                                               |
| ----------- | ----------------------------------------------------------------------------------------- |
| PU1         | The system shall load the settings saved in FLASH                                         |
| PU2         | The system shall verify that the settings are within defined boundaries                   |
| PU3         | In the case that the settings are invalid, the system will restore to the defined default |

### User Command

This use-case occurs when the user sends a command via the CLI

| ID          | Description                                                                                                         |
| ----------- | ------------------------------------------------------------------------------------------------------------------- |
| UC1         | The system shall check the command name against a map of predefined commands                                        |
| UC2         | The system shall verify that the command exists, else output a message saying the input is invalid                  |
| UC3         | The system shall send the command and arguments to the correct function                                             |
| UC4         | The system shall evaluate the arguments according to the convention as defined in [User Interface](#user-interface) |
| UC5         | The system shall execute the command function according the to defined requirements in [Commands](#commands)        |

### Temperature Update

This use-case occurs when the temperature is updated due to a timed update or user input

| ID          | Description                                                                                                         |
| ----------- | ------------------------------------------------------------------------------------------------------------------- |
| TU1         | The system shall run the DHT11 update function |
| TU2         | The system shall verify that the result is correct |
| TU3         | In the case that the result is invalid, the system will run the update function again |
| TU4         | The system shall save the result in RAM |

### Temperature Exceeds Warning Threshold

This use-case occurs when the temperature exceeds the warning threshold

| ID          | Description                                                                                                         |
| ----------- | ------------------------------------------------------------------------------------------------------------------- |
| TW1         | The system shall turn on the LED |
| TW2         | The system shall write the temperature to the log, along with time and date |

### Temperature Exceeds Critical Threshold

This use-case occurs when the temperature exceeds the warning threshold

| ID          | Description                                                                                                         |
| ----------- | ------------------------------------------------------------------------------------------------------------------- |
| TC1         | The system shall blink the LED |
| TC2         | The system shall turn on the buzzer to a predefined tune |
| TC3         | The system shall write the temperature to the log, along with time and date |

### Temperature Decreases Below Threshold

This use-case occurs when the temperature exceeds the warning threshold

| ID          | Description                                                                                                         |
| ----------- | ------------------------------------------------------------------------------------------------------------------- |
| BT1         | The system shall turn off the LED |
| BT2         | The system shall turn off the buzzer|
| BT3         | The system shall write the temperature to the log, along with time and date |

## User Interface

The user shall interface with the program via a CLI based on UART

The user can send commands with arguments

A space character will always seperate arguments

Tags can be ordered however the user desires as long as they follow the convention of `--tag argument`

``` bash
# Argument format
cmd [arguments] [-tags] arg [--full-tags] arg ...
```

Examples:

``` bash
get-temp # Outputs the temperature according to the last update
save-log /home/sahar/system.log # Saves the log to a file at "/home/sahar/system.log"
set-threshold --warning 50 --critical 70 # Sets the system thresholds
set-timedate -d 16 -y 1970 -m 11 # Sets the system date to 16/11/1970
```

## Commands

``` bash
get-temp # Outputs the temperature according to the last update
save-log filepath # Saves the log to 'filepath', quits if the directory doesn't exist
set-threshold [-w, -c] [--warning, --critical] # Sets the system thresholds. Warning cannot be set higher than Critical
set-timedate [-d, -y, -m, -H, -M, -S] [--day, --year, --month, --hour, --minute, --second] # Sets the system date and time
disable-component [led, buzzer] # Disables the desired component
enable-component  [led, buzzer] # Enables the desired component
```

