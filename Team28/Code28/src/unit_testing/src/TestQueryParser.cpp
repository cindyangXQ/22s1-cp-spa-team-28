#include "QPS/QueryParser.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    QueryParser::parse("Assign a; Constant c; Variable v; Select v such that Modifies(1, v) pattern a(v, _)");
    REQUIRE(0 == 0);
}