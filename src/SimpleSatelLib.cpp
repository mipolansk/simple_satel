#include "SimpleSatelLib.h"

#include "command/Command.h"

#ifdef TEST
void delay(int delay) {
}
#endif

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

bool SimpleSatelLibClass::readAnswer(byte *bytes, int &readCount) {
	// await answer
	int waitTime = 0;
	while (!isDataAvailable()) {
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

int SimpleSatelLibClass::readBytes(byte *bytes) {
	uint8_t readCount = 0;

	uint8_t frameStartPosition = -1;
	CmdReadingState state = initialState();
	while (isDataAvailable() && readCount < READING_BUFFER_SIZE) {
		if (state == READY) {
			// Got one full response. Stopping further processing
			break;
		}

		delay(10);
		uint8_t toRead = isDataAvailable();
		for (uint8_t i = 0; i < toRead; i++) {
			bytes[i] = readData();
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

bool SimpleSatelLibClass::processCommand(SSatel::Command command,
		SSatel::Answer &answer) {
	if (writeData == NULL || readData == NULL || isDataAvailable == NULL) {
		return false;
	}

	// send command
	byte *bytes = new byte[command.getLength()];
	command.toBytes(bytes);
	writeData(bytes, command.getLength());
	delete bytes;

	// await answer
	int waitTime = 0;
	while (!isDataAvailable()) {
		delay(10);
		waitTime += 10;

		if (waitTime >= MAX_ANSWER_AWAIT) {
			return false;
		}
	}

	// read answer
	int readCount = 0;
	byte readBytes[READING_BUFFER_SIZE];
	if (!readAnswer(readBytes, readCount)) {
		return false;
	}

	// parse answer
	if (!answer.fromBytes(readBytes, readCount)) {
		return false;
	}

	return true;
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

SimpleSatelLibClass SimpleSatel;
