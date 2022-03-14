#ifndef OUTPUTS_ON_COMMAND
#define OUTPUTS_ON_COMMAND

#include "Command.h"

namespace SSatel {
	class OutputsOnCommand : public Command {
	public:
		OutputsOnCommand() :
			Command(24) {
			setCommand(CMD_OUTPUTS_ON);
		}
		OutputsOnCommand(const OutputsOnCommand& toCopy) :
			Command(toCopy) {}

		/**
		 * Code should be defined as in protocol description.
		 * ex if code is '1234' you shoud privde two bytes 0x12, 0x34 and length equal 2
		 */
		void setUserCode(uint8_t codeLength, uint8_t* bytes) {
			for (uint8_t i = 0; i < 8; i++) {
				if (i < codeLength) {
					setData(i, bytes[i]);
				} else {
					setData(i, 0xFF);
				}
			}
		}

		bool setActive(uint8_t bit) {
			if (bit > 64) {
				return false;
			}

			// +8 because first 8 bytes are for the user code
			uint8_t byteNo = ((bit - 1) / 8) + 8;
			uint8_t bitNo = (bit - 1) % 8;

			uint8_t* data = getData();
			uint8_t currentValue = data[byteNo];
			setData(byteNo, currentValue | (1 << bitNo));
			return true;
		}

		bool acceptsCommand(uint8_t cmd) {
			return cmd == CMD_OUTPUTS_ON;
		}
	};
}

#endif /* OUTPUTS_ON_COMMAND */