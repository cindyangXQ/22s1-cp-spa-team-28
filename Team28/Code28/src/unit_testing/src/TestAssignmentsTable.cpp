#include "catch.hpp"

#include "PKB/AssignmentsTable.h"

TEST_CASE("AssignmentsTable can initialise, store and retrieve correctly") {
    AssignmentsTable assignmentsTable;

    IntStringPair test = IntStringPair(1, "x1");
    // Line 1: x1 = 1 + 2
    std::string expr = "((1)+(2))";
    Assignment assignment = Assignment(1, std::string("x1"), expr);
    assignmentsTable.store(&assignment);

    // successfully stored x1 = 1 + 2
    REQUIRE(assignmentsTable.getTableSize() == 1);

    // Stored correct values
    std::vector<Value> expectedResult = {Value(ValueType::STMT_NUM, "1")};
    std::vector<Value> output = assignmentsTable.containsVarAndExpr("x1", expr);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}
