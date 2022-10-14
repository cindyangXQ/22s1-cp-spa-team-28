#include "catch.hpp"

#include "commons/StringUtil.h"

TEST_CASE("toString works correctly") { REQUIRE(toString(1) == "1"); }

TEST_CASE("convertToType works correctly") {
    REQUIRE(convertToType<std::string>("1") == "1");
    REQUIRE(convertToType<int>("1") == 1);
}
