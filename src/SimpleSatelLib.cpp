#include "SimpleSatelLib.h"

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

CmdReadingState initialState();
CmdReadingState nextReadingState(CmdReadingState currentState, byte readByte);

Result<SSatel::OutputsStateAnswer> SimpleSatelLibClass::readOutputsState() {
	if (writeData == NULL || readData == NULL || isDataAvailable == NULL) {
		Result<SSatel::OutputsStateAnswer> result;
		return result;
	}
	// send command
	byte bytes[CMD_OUTPUT_STATE_LENGTH];
	SSatel::OutputsStateCommand command;
	command.toBytes(bytes);
	writeData(bytes, CMD_OUTPUT_STATE_LENGTH);

	// await answer
	int waitTime = 0;
	while (!isDataAvailable()) {
		delay(10);
		waitTime += 10;

		if (waitTime >= MAX_ANSWER_AWAIT) {
			Result<SSatel::OutputsStateAnswer> result;
			return result;
		}
	}
	// read answer
	int readCount = 0;
	byte readBytes[READING_BUFFER_SIZE];
	if (!readAnswer(readBytes, readCount)) {
		Result<SSatel::OutputsStateAnswer> result;
		return result;
	}

	SSatel::OutputsStateAnswer answer;
	if (!answer.fromBytes(readBytes, readCount)) {
		Result<SSatel::OutputsStateAnswer> result;
		return result;
	}
	Result<SSatel::OutputsStateAnswer> result(answer);
	return result;
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

SimpleSatelLibClass SimpleSatel;
