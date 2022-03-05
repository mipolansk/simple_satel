# SimpleSatel Library for Arduino IDE

SimpleSatel is a library for [Arduino IDE](https://www.arduino.cc/en/software) that makes the integration of Satel INT-RS Protocol easier. 
The library is on the very beginning phase of development. The basic structure is almoast given, but the implementation of many commands is not done yet.

## Commands
The list of already implemented commands:

### (0x0) Zones violation
Command to get the violation of zones.

### (0x1) Zones tamper
Command to get the tamper of zones.

### (0x17) Outputs state
Command to get the state of outputs.

### (0x88) Outputs on
Command to set ouputs on.

## Requirements
As the library is on very beginning phase it is currently tested only with **ESP8266**/**ESP32** and **Integra64 connected with ETHM-1 Plus** module. It may happen, that it will not work properly with other hardware.

### Importing to Arduino IDE
If you download the repository as a zip file, you can import it as a library. For that please go *Sketch > Include Library > Add .ZIP Library*. Find the download zip file and that's it! Detailed instruction you can find on [Arduino IDE - Installing Libraries](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries).

## Examples
Take a look on [examples](examples/) directory to check how to use the library.
