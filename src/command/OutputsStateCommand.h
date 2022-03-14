#ifndef OUTPUT_STATE_COMMAND
#define OUTPUT_STATE_COMMAND

#include "Command.h"

namespace SSatel {
	class OutputsStateCommand: public Command {
	public:
		OutputsStateCommand() :
				Command(0) {
			setCommand(CMD_OUTPUT_STATE);
		}
		OutputsStateCommand(const OutputsStateCommand &toCopy) :
				Command(toCopy) {
		}
	};

	class OutputsStateAnswer: public Answer {
	public:
		OutputsStateAnswer() :
				Answer(ANS_OUTPUT_STATE_DATA_LENGTH) {
		}
		OutputsStateAnswer(const OutputsStateAnswer &toCopy) :
				Answer(toCopy) {
		}

		inline bool isOutputActive(uint8_t outputNo) {
			return isActive(outputNo);
		}

		bool acceptsCommand(uint8_t cmd) {
			return cmd == CMD_OUTPUT_STATE;
		}
	};
}

#endif /* OUTPUT_STATE_COMMAND */
