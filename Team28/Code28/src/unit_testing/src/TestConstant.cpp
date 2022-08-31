#include "catch.hpp"

#include "PKB/Constant.h"

TEST_CASE("Constant is initialised correctly and inherits all Entity methods correctly") {
	Constant test = Constant("test");
	Constant testCopy = Constant("test");
	Constant notTestCopy = Constant("not test");

	// getName() gets the name correctly
	REQUIRE(test.getName() == "test");
	// Different instances of Constant are equal if ConstantName is the same
	REQUIRE(test == testCopy);
}
