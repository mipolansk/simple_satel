#ifndef ZONES_VIOLATION_COMMAND
#define ZONES_VIOLATION_COMMAND

#include "../SimpleSatelFrame.h"

namespace SSatel {
	class ZonesViolationCommand: public Command {
	public:
		ZonesViolationCommand() :
				Command(0) {
			setCommand(CMD_ZONE_VIOLATION);
		}
		ZonesViolationCommand(const ZonesViolationCommand &toCopy) :
				Command(toCopy) {
		}
	};

	class ZonesViolationAnswer: public Answer {
	public:
		ZonesViolationAnswer() :
				Answer(ANS_ZONE_VIOLATION_DATA_LENGTH) {
		}
		ZonesViolationAnswer(const ZonesViolationAnswer &toCopy) :
				Answer(toCopy) {
		}

		bool isZoneViolated(uint8_t zoneNo) {
			return isActive(zoneNo);
		}
	};
}

#endif /* ZONES_VIOLATION_COMMAND */