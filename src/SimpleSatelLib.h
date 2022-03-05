#ifndef SATEL_INT_RS_LIB
#define SATEL_INT_RS_LIB

#include "command/ZonesViolationCommand.h"
#include "command/ZonesTamperCommand.h"
#include "command/OutputsStateCommand.h"
#include "command/OutputsOnCommand.h"
#include "SimpleSatelFrame.h"
#include <WiFi.h>

#define MAX_ANSWER_AWAIT 3000

template<class T>
class Result {
	bool success;
	T data;
public:
	Result(T data) :
		success(true), data(data) {}
	Result(const Result<T>& toCopy) :
		success(toCopy.success), data(toCopy.data) {}
	Result() :
		success(false) {}
	inline bool isSuccess() {
		return success;
	}
	inline T getData() {
		return data;
	}
	void setData(T data) {
		this->data = data;
		this->success = true;
	}
};

class SimpleSatelLibClass {

	WiFiClient client;
	uint8_t userCode[8];
	int timeout = 15000; // About 15 seconds.

	bool readAnswer(byte* bytes, int& readCount);
	int readBytes(byte* bytes);
	bool processCommand(SSatel::Command command, SSatel::Frame& answer);
	void sendCommand(SSatel::Command command);

public:

	bool connected();
	bool connect(const char* host, int port);
	/**
	 * Code should be defined as in protocol description.
	 * ex if code is '1234' you shoud privde two bytes 0x12, 0x34 and length equal 2
	 */
	void setUserCode(uint8_t codeLength, uint8_t* bytes) {
		for (uint8_t i = 0; i < 8; i++) {
			if (i < codeLength) {
				userCode[i] = bytes[i];
			} else {
				userCode[i] = 0xFF;
			}
		}
	}
	Result<SSatel::ZonesViolationAnswer> readZonesViolation();
	Result<SSatel::ZonesTamperAnswer> readZonesTamper();
	Result<SSatel::OutputsStateAnswer> readOutputsState();
	Result<SSatel::CommandResultAnswer> setOutputsOn(uint8_t outputsCount, uint8_t* outputsNumbers);

};

extern SimpleSatelLibClass SimpleSatel;

#endif /* SATEL_INT_RS_LIB */
