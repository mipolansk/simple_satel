#include "SimpleSatelLib.h"

#include "command/Command.h"

#define log_db(count, bytes) { \
		char* output = (char*)malloc((sizeof(char) * count * 3) + 2); \
		for (int i = 0; i < count; i++) { \
			char buf[4] = { 0 }; \
			sprintf(buf, "%02x ", bytes[i]); \
			strcpy(output + i * 3, buf); \
		} \
		output[count * 3 - 2] = 10; \
		output[count * 3 - 1] = 0; \
		log_d("%s", output); \
		delete output; \
	}
#define log_eb(count, bytes) { \
		char* output = (char*)malloc((sizeof(char) * count * 3) + 2); \
		for (int i = 0; i < count; i++) { \
			char buf[4] = { 0 }; \
			sprintf(buf, "%02x ", bytes[i]); \
			strcpy(output + i * 3, buf); \
		} \
		output[count * 3 - 2] = 10; \
		output[count * 3 - 1] = 0; \
		log_e("%s", output); \
		delete output; \
	}

#define READING_BUFFER_SIZE 64

enum CmdReadingState {
	AWAITING_FIRST_HEADER,
	AWAITING_SECOND_HEADER,
	AWAITING_CMD,
	AWAITING_DATA_AND_CRC,
	AWAITING_SECOND_FOOTER,
	READY
};

CmdReadingState initialState() {
	return AWAITING_FIRST_HEADER;
}

CmdReadingState nextReadingState(CmdReadingState currentState, byte readByte) {
	if (currentState == AWAITING_FIRST_HEADER) {
		if (readByte == 0xFE) {
			return AWAITING_SECOND_HEADER;
		} else {
			return AWAITING_FIRST_HEADER;
		}
	} else if (currentState == AWAITING_SECOND_HEADER) {
		if (readByte == 0xFE) {
			return AWAITING_SECOND_HEADER;
		} else {
			return AWAITING_CMD;
		}
	} else if (currentState == AWAITING_CMD) {
		if (readByte == 0xFE) {
			return AWAITING_CMD;
		} else {
			return AWAITING_DATA_AND_CRC;
		}
	} else if (currentState == AWAITING_DATA_AND_CRC) {
		if (readByte == 0xFE) {
			return AWAITING_SECOND_FOOTER;
		} else {
			return AWAITING_DATA_AND_CRC;
		}
	} else if (currentState == AWAITING_SECOND_FOOTER && readByte == 0x0D) {
		return READY;
	} else {
		return AWAITING_DATA_AND_CRC;
	}
}

bool SimpleSatelLibClass::readAnswer(byte* bytes, int& readCount) {
	// await answer
	int waitTime = 0;
	while (!client.available()) {
		delay(10);
		waitTime += 10;

		if (waitTime >= MAX_ANSWER_AWAIT) {
			return false;
		}
	}

	// read answer
	readCount = readBytes(bytes);
	return readCount > 0;
}

int SimpleSatelLibClass::readBytes(byte* bytes) {
	uint8_t readCount = 0;

	uint8_t frameStartPosition = -1;
	CmdReadingState state = initialState();
	while (client.available() && readCount < READING_BUFFER_SIZE) {
		if (state == READY) {
			// Got one full response. Stopping further processing
			break;
		}

		delay(10);
		uint8_t toRead = client.available();
		for (uint8_t i = 0; i < toRead; i++) {
			bytes[i] = client.read();
			CmdReadingState previousState = state;
			state = nextReadingState(state, bytes[i]);
			readCount++;
			if (state == READY) {
				// Got one full response. Stopping further processing
				break;
			} else if (previousState == AWAITING_CMD
				&& state == AWAITING_DATA_AND_CRC) {
				// Got first byte not equal 0xFE
				frameStartPosition = i - 3;
			}
		}
	}

	if (frameStartPosition == -1) {
		// no frame found
		return 0;
	} else if (frameStartPosition != 0) {
		for (int i = 0; i < readCount - frameStartPosition; i++) {
			bytes[i] = bytes[i + frameStartPosition];
		}
		readCount -= frameStartPosition;
	}

	return state == READY ? readCount : 0;
}

void SimpleSatelLibClass::sendCommand(SSatel::Command command) {
	uint8_t length = command.getLength();
	byte* bytes = new byte[length];
	command.toBytes(bytes);

	// Debug printing...
	log_d("Sending command (%d):", length);
	log_db(length, bytes);

	client.write(bytes, length);
	delete bytes;
}

bool SimpleSatelLibClass::processCommand(SSatel::Command command,
	SSatel::Frame& answer) {
	// send command
	sendCommand(command);

	// read answer
	int readCount = 0;
	byte readBytes[READING_BUFFER_SIZE];
	if (!readAnswer(readBytes, readCount)) {
		log_e("No answer - timeout...");
		return false;
	}

	// parse answer
	if (!answer.fromBytes(readBytes, readCount)) {
		log_e("Could not parse answer:");
		log_eb(readCount, readBytes);
		return false;
	}

	return true;
}

bool SimpleSatelLibClass::connected() {
	return client.connected();
}

bool SimpleSatelLibClass::connect(const char* host, int port) {
	int retry = 0;
	while (!client.connect(host, port)) {
		if (retry >= timeout) {
			return false;
		}
		delay(1000);
	}

	delay(250);

	return true;
}

Result<SSatel::ZonesViolationAnswer> SimpleSatelLibClass::readZonesViolation() {
	SSatel::ZonesViolationCommand command;
	SSatel::ZonesViolationAnswer answer;
	if (processCommand(command, answer)) {
		Result<SSatel::ZonesViolationAnswer> result(answer);
		return result;
	}

	Result<SSatel::ZonesViolationAnswer> result;
	return result;
}

Result<SSatel::ZonesTamperAnswer> SimpleSatelLibClass::readZonesTamper() {
	SSatel::ZonesTamperCommand command;
	SSatel::ZonesTamperAnswer answer;
	if (processCommand(command, answer)) {
		Result<SSatel::ZonesTamperAnswer> result(answer);
		return result;
	}

	Result<SSatel::ZonesTamperAnswer> result;
	return result;
}

Result<SSatel::OutputsStateAnswer> SimpleSatelLibClass::readOutputsState() {
	SSatel::OutputsStateCommand command;
	SSatel::OutputsStateAnswer answer;
	if (processCommand(command, answer)) {
		Result<SSatel::OutputsStateAnswer> result(answer);
		return result;
	}

	Result<SSatel::OutputsStateAnswer> result;
	return result;
}

Result<SSatel::CommandResultAnswer> SimpleSatelLibClass::setOutputsOn(uint8_t outputsCount, uint8_t* outputsNumbers) {
	SSatel::OutputsOnCommand command;
	command.setUserCode(8, userCode);
	for (int i = 0; i < outputsCount; i++) {
		command.setActive(outputsNumbers[i]);
	}

	SSatel::CommandResultAnswer answer;
	if (processCommand(command, answer)) {
		Result<SSatel::CommandResultAnswer> result(answer);
		return result;
	}

	Result<SSatel::CommandResultAnswer> result;
	return result;
}

SimpleSatelLibClass SimpleSatel;