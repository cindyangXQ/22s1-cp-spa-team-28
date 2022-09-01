#include "QPS/QueryParser.h"
#include "QPS/SolvableQuery.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    SolvableQuery solvableQ = QueryParser::parse("Assign a; Constant c; Variable v; Select v such that Modifies(1, v) pattern a(v, _)");
    solvableQ.evaluate();
}