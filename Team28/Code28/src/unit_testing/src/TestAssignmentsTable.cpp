#include "catch.hpp"

#include "PKB/Tables/AssignmentsTable/AssignmentsTable.h"

TEST_CASE("AssignmentsTable can initialise, store and retrieve correctly") {
    AssignmentsTable assignmentsTable;

    IntStringPair test = IntStringPair(1, "x1");
    // Line 1: x1 = 1 + 2
    std::string expr = "((1)+(2))";
    Assignment assignment = Assignment(1, std::string("x1"), expr);
    assignmentsTable.store(&assignment);

    // Stored correct values
    std::vector<Value> expectedResult = {Value(ValueType::STMT_NUM, "1")};
    std::vector<Value> output = assignmentsTable.containsVarAndExpr("x1", expr);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AssignmentsTable getAllAsString works correctly") {
    AssignmentsTable assignmentsTable;

    std::string expr1 = "((1)+(2))";
    Assignment assignment1 = Assignment(1, std::string("x1"), expr1);
    assignmentsTable.store(&assignment1);

    std::string expr2 = "((1)+(3))";
    Assignment assignment2 = Assignment(2, std::string("x2"), expr2);
    assignmentsTable.store(&assignment2);

    std::vector<std::string> expectedResult = {"1, x1 = ((1)+(2))",
                                               "2, x2 = ((1)+(3))"};
    std::vector<std::string> output = assignmentsTable.getAllAsString();
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}