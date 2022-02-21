/*
 * debug.h
 *
 *  Created on: 20 lut 2022
 *      Author: michal
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef TEST

#include <Arduino.h>

void print(int v) { Serial.print(v); }
void print(const char* v) { Serial.print(v); }
void printH(int v) { Serial.print(v, HEX); }
void println(int v) { Serial.println(v); }
void println(const char* v) { Serial.println(v); }
void printlnH(int v) { Serial.println(v, HEX); }

#else

#include <iostream>

void print(unsigned int v) { std::cout << std::dec << v; }
void print(const char* v) { std::cout << v; }
void printH(unsigned int v) { std::cout << std::hex << v; }
void println(unsigned int v) { std::cout << std::dec << v << "\n"; }
void println(const char* v) { std::cout << v << "\n"; }
void printlnH(unsigned int v) { std::cout << std::hex << v << "\n"; }

#endif

void printBytes(byte *bytes, uint8_t length) {
	print("Printing bytes (");
	print(length);
	print(") ");
	for (int i = 0; i < length; i++) {
		printH(bytes[i]);
		print(" ");
	}
	println(".");
}

#endif /* DEBUG_H_ */
