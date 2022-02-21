#include "SimpleSatelFrame.h"

namespace SSatel {

	Frame::Frame(uint8_t dataLength) :
			head1(0xFE), head2(0xFE), cmd(0), crcHigh(0), crcLow(0), //
			foot1(0xFE), foot2(0x0D), dataLength(dataLength) {
		if (dataLength > 0) {
			data = new uint8_t[dataLength];
		} else {
			data = NULL;
		}
	}

	Frame::Frame(const Frame &toCopy) :
			Frame(toCopy.dataLength) {
		cmd = toCopy.cmd;
		for (int i = 0; i < dataLength; i++) {
			data[i] = toCopy.data[i];
		}
	}

	Frame::Frame(byte *bytes, uint8_t length) {
		if (length < 7) {
			head1 = 0xFE;
			head2 = 0xFE;
			cmd = 0;
			data = NULL;
			crcHigh = 0;
			crcLow = 0;
			foot1 = 0xFE;
			foot2 = 0x0D;
			dataLength = 0;
		} else {
			dataLength = length - 7;
			uint8_t idx = 0;
			head1 = bytes[idx++];
			head2 = bytes[idx++];
			cmd = bytes[idx++];
			if (dataLength > 0) {
				data = new byte[dataLength];
				for (int i = 0; i < dataLength; i++) {
					data[i] = bytes[idx++];
				}
			} else {
				data = NULL;
			}
			crcHigh = bytes[idx++];
			crcLow = bytes[idx++];
			foot1 = bytes[idx++];
			foot2 = bytes[idx++];
		}
	}

	Frame::~Frame() {
		if (data != NULL) {
			delete[] data;
			data = NULL;
		}
	}

	void Frame::setCommand(uint8_t command) {
		cmd = command;
	}

	void Frame::toBytes(byte *bytes) {
		calculateCrc();

		uint8_t idx = 0;
		bytes[idx++] = head1;
		bytes[idx++] = head2;
		bytes[idx++] = cmd;
		if (data != NULL) {
			for (int i = 0; i < dataLength; i++) {
				bytes[idx++] = data[i];
			}
		}
		bytes[idx++] = crcHigh;
		bytes[idx++] = crcLow;
		bytes[idx++] = foot1;
		bytes[idx++] = foot2;
	}

	bool Frame::fromBytes(byte *bytes, uint8_t length) {
		if (length != dataLength + 7) {
			// Incorrect frame for given bytes;
			return false;
		}
		uint8_t idx = 0, crcHighGot = 0, crcLowGot = 0;

		head1 = bytes[idx++];
		head2 = bytes[idx++];
		cmd = bytes[idx++];
		if (data != NULL) {
			for (uint8_t i = 0; i < dataLength; i++) {
				data[i] = bytes[idx++];
			}
		}
		crcHighGot = bytes[idx++];
		crcLowGot = bytes[idx++];
		foot1 = bytes[idx++];
		foot2 = bytes[idx++];

		calculateCrc();
		return crcHighGot == crcHigh && crcLowGot == crcLow;
	}

	void Frame::calculateCrc() {
		uint16_t crc = calculateCrc(0x147A, cmd);

		if (data != NULL) {
			for (int i = 0; i < dataLength; i++) {
				crc = calculateCrc(crc, data[i]);
				if (data[i] == 0xFE) {
					// Next bytes should be skipped
					i++;
				}
			}
		}

		crcHigh = crc >> 8;
		crcLow = crc & 0xFF;
	}

	uint16_t Frame::calculateCrc(uint16_t crc, uint8_t data) {
		uint16_t result = rotateLeft(crc);
		result = result ^ 0xFFFF;
		result = result + ((result >> 8) & 0xFF) + data;

		return result;
	}

	uint16_t Frame::rotateLeft(uint16_t number) {
		if ((number & 1 << 15) == 0) {
			return number << 1;
		} else {
			return (number << 1) | 1;
		}

	}

}
