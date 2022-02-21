/*
 * SimpleSatelLibTest.cpp
 *
 *  Created on: 19 lut 2022
 *      Author: michal
 */

#include "gtest/gtest.h"
#include "../src/SimpleSatelLib.h"

namespace SSatel {
	namespace Tests {

		//
		// Basic outputs state test
		//

		int outputStateCommandPosition = 0;
		uint8_t outputStateCommandBytes[] = { 0xFE, 0xFE, 0x17, 0xD7, 0xF9,
				0xFE, 0x0D };
		int outputStateAnswerLengthPosition = 0;
		uint8_t outputStateAnswerLength[] = { 23, 0 };
		int outputStateAnswerPosition = 0;
		uint8_t outputStateAnswerBytes[] = { 0xFE, 0xFE, 0x17, 0x0C, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x05, 0x69, 0xFE, 0x0D };

		uint8_t outputStateWriter(byte *bytes, uint8_t length) {
			EXPECT_EQ(7, length);
			for (int i = 0; i < length; i++) {
				EXPECT_EQ(bytes[i], outputStateCommandBytes[i])
						<< "Comparing bytes at " << i;
			}
			return length;
		}
		byte outputStateReader() {
			return outputStateAnswerBytes[outputStateAnswerPosition++];
		}
		uint8_t outputStateChecker() {
			return outputStateAnswerLength[outputStateAnswerLengthPosition++];
		}

		TEST(SimpleSatelLib, ShouldSendOutputsState) {
			SimpleSatelLibClass lib;
			lib.setWriter(&outputStateWriter);
			lib.setReader(&outputStateReader);
			lib.setChecker(&outputStateChecker);

			Result<SSatel::OutputsStateAnswer> result = lib.readOutputsState();

			EXPECT_EQ(result.isSuccess(), true);
			if (result.isSuccess()) {
				byte resultBytes[23];
				result.getData().toBytes(resultBytes);

				for (int i = 0; i < 23; i++) {
					EXPECT_EQ(resultBytes[i], outputStateAnswerBytes[i])
							<< "Comparing bytes at " << i;

					if (i == 4) {
						i++;
					}
				}
			}
		}

		//
		// Outputs state test - leading bytes
		//

		int outputsStateLeadingCommandPosition = 0;
		uint8_t outputsStateLeadingCommandBytes[] = { 0xFE, 0xFE, 0x17, 0xD7, 0xF9,
				0xFE, 0x0D };
		int outputsStateLeadingAnswerLengthPosition = 0;
		uint8_t outputsStateLeadingAnswerLength[] = { 25, 0 };
		int outputsStateLeadingAnswerPosition = 0;
		uint8_t outputsStateLeadingAnswerBytes[] = { 0xFE, 0xFE, 0xFE, 0xFE, 0x17, 0x0C, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x05, 0x69, 0xFE, 0x0D };

		uint8_t outputsStateLeadingWriter(byte *bytes, uint8_t length) {
			EXPECT_EQ(7, length);
			for (int i = 0; i < length; i++) {
				EXPECT_EQ(bytes[i], outputsStateLeadingCommandBytes[i])
						<< "Comparing bytes at " << i;
			}
			return length;
		}
		byte outputsStateLeadingReader() {
			return outputsStateLeadingAnswerBytes[outputsStateLeadingAnswerPosition++];
		}
		uint8_t outputsStateLeadingChecker() {
			return outputsStateLeadingAnswerLength[outputsStateLeadingAnswerLengthPosition++];
		}

		TEST(SimpleSatelLib, ShouldSendOutputsStateAndReadAnswerWithReadingBytes) {
			SimpleSatelLibClass lib;
			lib.setWriter(&outputsStateLeadingWriter);
			lib.setReader(&outputsStateLeadingReader);
			lib.setChecker(&outputsStateLeadingChecker);

			Result<SSatel::OutputsStateAnswer> result = lib.readOutputsState();

			EXPECT_EQ(result.isSuccess(), true);
			if (result.isSuccess()) {
				byte resultBytes[23];
				result.getData().toBytes(resultBytes);

				for (int i = 0; i < 23; i++) {
					EXPECT_EQ(resultBytes[i], outputsStateLeadingAnswerBytes[i+2])
							<< "Comparing bytes at " << i;

					if (i == 4) {
						i++;
					}
				}
			}
		}
	}
}
