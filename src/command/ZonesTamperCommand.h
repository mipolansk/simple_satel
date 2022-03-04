#ifndef ZONES_TAMPER_COMMAND
#define ZONES_TAMPER_COMMAND

#include "Command.h"

namespace SSatel {
	class ZonesTamperCommand: public Command {
	public:
		ZonesTamperCommand() : Command(0) {
			setCommand(CMD_ZONE_TAMPER);
		}
		ZonesTamperCommand(const ZonesTamperCommand &toCopy) :
				Command(toCopy) {
		}
	};

	class ZonesTamperAnswer: public Answer {
	public:
		ZonesTamperAnswer() :
				Answer(ANS_ZONE_TAMPER_DATA_LENGTH) {
		}
		ZonesTamperAnswer(const ZonesTamperAnswer &toCopy) :
				Answer(toCopy) {
		}

		bool isZoneTampered(uint8_t zoneNo) {
			return isActive(zoneNo);
		}
	};
}

#endif /* ZONES_TAMPER_COMMAND */
