# SimpleSatel Library for Arduino IDE

SimpleSatel is a library for [Arduino IDE](https://www.arduino.cc/en/software) that makes the integration of Satel INT-RS Protocol easier. 
The library is on the very beginning phase of development. The basic structure is almoast given, but the implementation of many commands is not done yet.

## Commands
The list of already implemented commands:

### (0x0) Zones violation
Command to get the violation of zones.

### (0x17) Outputs State
Command to get the state of outputs.

## Requirements
As the library is on very beginning phase it is currently tested only with **ESP8266** and **Integra64 connected with ETHM-1 Plus** module. It may happen, that it will not work properly with other hardware.

## Examples
Take a look on [examples](examples/) directory to check how to use the library.
