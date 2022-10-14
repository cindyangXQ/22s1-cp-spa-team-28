#include "catch.hpp"

#include "commons/StringUtil.h"

TEST_CASE("toString works correctly") { REQUIRE(toString(1) == "1"); }
