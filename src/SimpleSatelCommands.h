#ifndef SIMPLE_SATEL_COMMANDS
#define SIMPLE_SATEL_COMMANDS

#include "SimpleSatelFrame.h"

#define CMD_OUTPUT_STATE 0x17

#define CMD_OUTPUT_STATE_LENGTH 7

namespace SSatel {
	class OutputsStateCommand: public Frame {
	public:
		OutputsStateCommand() :
				Frame(0) {
			setCommand(CMD_OUTPUT_STATE);
		}
	};

	class OutputsStateAnswer: public Frame {
	public:
		OutputsStateAnswer() :
				Frame(16) {
		}
		OutputsStateAnswer(const OutputsStateAnswer &toCopy) :
				Frame(toCopy) {
		}
		bool isOutputActive(uint8_t outputNo);
	};
}

#endif /* SIMPLE_SATEL_COMMANDS */
