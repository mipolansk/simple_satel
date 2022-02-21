#include "SimpleSatelCommands.h"

namespace SSatel {
	bool OutputsStateAnswer::isOutputActive(uint8_t outputNo) {
		if (outputNo > (16 << 3)) {
			return false;
		}

		uint8_t byteNo = (outputNo - 1) / 8;
		uint8_t bitNo = (outputNo - 1) % 8;

		uint8_t *data = getData();
		return (data[byteNo] & (1 << bitNo)) > 0;
	}
}
