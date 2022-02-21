#ifndef SATEL_FRAME
#define SATEL_FRAME

#ifndef TEST

#include <Arduino.h>

#else

#include <inttypes.h>
#include <cstddef>
typedef unsigned char byte;

#endif

#define FRAME_BASIC_LENGTH 7

namespace SSatel {
	class Frame {
		uint8_t head1;
		uint8_t head2;
		uint8_t cmd;
		uint8_t *data;
		uint8_t crcHigh;
		uint8_t crcLow;
		uint8_t foot1;
		uint8_t foot2;

		uint8_t dataLength;

		void calculateCrc();
		uint16_t calculateCrc(uint16_t crc, uint8_t data);
		uint16_t rotateLeft(uint16_t);

	protected:
		inline uint8_t* getData() {
			return data;
		}
		inline void setData(uint8_t position, uint8_t value) {
			data[position] = value;
		}
		inline uint8_t getDataLength() {
			return dataLength;
		}

	public:
		Frame(uint8_t dataLength);
		Frame(const Frame &toCopy);
		Frame(byte *bytes, uint8_t length);
		~Frame();
		void setCommand(uint8_t command);
		void toBytes(byte *bytes);
		bool fromBytes(byte *bytes, uint8_t length);
	};
}
;

#endif /* SATEL_FRAME */
