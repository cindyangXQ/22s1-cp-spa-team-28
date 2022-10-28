#include "catch.hpp"

#include "commons/Variable.h"

TEST_CASE("Variable is initialised correctly and inherits all Entity methods "
          "correctly") {
    Variable test = Variable("test");
    Variable testCopy = Variable("test");
    Variable notTestCopy = Variable("not test");

    // getName() gets the name correctly
    REQUIRE(test.getName() == "test");
    // Different instances of Variable are equal if VariableName is the same
    REQUIRE(test == testCopy);
}
