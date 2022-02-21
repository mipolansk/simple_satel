/*
 * Testing.cpp
 *
 *  Created on: 18 lut 2022
 *      Author: michal
 */

#include "gtest/gtest.h"
#include "../src/SimpleSatelFrame.h"

namespace SSatel {
	namespace Tests {
		TEST(SimpleSatelFrame, ShouldCalculateCrcCorrectly1) {
			byte bytes[] = { 0xFE, 0xFE, 0x09, 0xD7, 0xEB, 0xFE, 0x0D };
			SSatel::Frame frame(bytes, 7);

			byte result[7];
			frame.toBytes(result);

			for (int i = 0; i < 7; i++) {
				EXPECT_EQ(result[i], bytes[i]) << "Comparing bytes at " << i;
			}
		}

		TEST(SimpleSatelFrame, ShouldCalculateCrcCorrectly2) {
			byte bytes[] = { 0xFE, 0xFE, 0x1C, 0xD7, 0xFE, 0xFE, 0x0D };
			SSatel::Frame frame(bytes, 7);

			byte result[7];
			frame.toBytes(result);

			for (int i = 0; i < 7; i++) {
				EXPECT_EQ(result[i], bytes[i]) << "Comparing bytes at " << i;
			}
		}

		TEST(SimpleSatelFrame, ShouldCalculateCrcCorrectly3) {
			byte bytes[] = { 0xFE, 0xFE, 0xE0, 0x12, 0x34, 0xFF, 0xFF, 0x8A, 0x9B, 0xFE, 0x0D };
			SSatel::Frame frame(bytes, 11);

			byte result[11];
			frame.toBytes(result);

			for (int i = 0; i < 11; i++) {
				EXPECT_EQ(result[i], bytes[i]) << "Comparing bytes at " << i;
			}
		}
	}
}
