#ifndef COMMAND_COMMAND_H_
#define COMMAND_COMMAND_H_

#include "../SimpleSatelFrame.h"

#define CMD_ZONE_VIOLATION 0x0
#define CMD_ZONE_TAMPER 0x1
#define CMD_OUTPUT_STATE 0x17
#define CMD_OUTPUTS_ON 0x88

#define ANS_ZONE_VIOLATION_DATA_LENGTH 16
#define ANS_ZONE_TAMPER_DATA_LENGTH 16
#define ANS_OUTPUT_STATE_DATA_LENGTH 16

namespace SSatel {
	class Command : public Frame {

	public:
		Command(uint8_t dataLength) :
			Frame(dataLength) {}
		Command(const Command& toCopy) :
			Frame(toCopy) {}
		inline uint8_t getLength() {
			return getDataLength() + FRAME_BASIC_LENGTH;
		}
	};

	class Answer : public Frame {
	protected:
		bool isActive(uint8_t bit) {
			if (bit > (16 << 3)) {
				return false;
			}

			uint8_t byteNo = (bit - 1) / 8;
			uint8_t bitNo = (bit - 1) % 8;

			uint8_t* data = getData();
			return (data[byteNo] & (1 << bitNo)) > 0;
		}

	public:
		Answer(uint8_t dataLength) :
			Frame(dataLength) {}
		Answer(const Answer& toCopy) :
			Frame(toCopy) {}
	};

	class CommandResultAnswer : public Frame {
	public:
		CommandResultAnswer() :
			Frame(1) {}
		CommandResultAnswer(const CommandResultAnswer& toCopy) :
			Frame(toCopy) {}

		bool isAccepted() {
			return getData(0) == 0xFF;
		}
	};
}

#endif /* COMMAND_COMMAND_H_ */
