/*
 * TestsRunner.cpp
 *
 *  Created on: 19 lut 2022
 *      Author: michal
 */

#include "gtest/gtest.h"

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

