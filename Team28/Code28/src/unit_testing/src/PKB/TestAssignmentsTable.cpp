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

TEST_CASE("AssignmentsTable: validate works correctly") {
    AssignmentsTable assignmentsTable;

    std::string expr1 = "((1)+(2))";
    Assignment assignment1 = Assignment(1, std::string("x1"), expr1);
    assignmentsTable.store(&assignment1);

    std::string expr2 = "((1)+(3))";
    Assignment assignment2 = Assignment(2, std::string("x2"), expr2);
    assignmentsTable.store(&assignment2);

    // Positive testing
    AssignExpression assignExpr1Exact = AssignExpression(expr1, true);
    AssignExpression assignExpr1Partial = AssignExpression("(1)", false);
    AssignExpression assignExpr2Exact = AssignExpression(expr2, true);
    AssignExpression assignExpr2Partial = AssignExpression("(3)", false);
    AssignExpression wildcard = AssignExpression("_", false);
    REQUIRE(assignmentsTable.validate(1, "x1", assignExpr1Exact));
    REQUIRE(assignmentsTable.validate(1, "x1", assignExpr1Partial));
    REQUIRE(assignmentsTable.validate(2, "x2", assignExpr2Exact));
    REQUIRE(assignmentsTable.validate(2, "x2", assignExpr2Partial));
    REQUIRE(assignmentsTable.validate(1, "x1", wildcard));
    REQUIRE(assignmentsTable.validate(2, "x2", wildcard));

    // Wrong stmtNo
    REQUIRE(!assignmentsTable.validate(2, "x1", assignExpr1Exact));
    REQUIRE(!assignmentsTable.validate(1, "x2", assignExpr2Partial));

    // Wrong var
    REQUIRE(!assignmentsTable.validate(1, "nomatchbigsad", assignExpr1Exact));
    REQUIRE(!assignmentsTable.validate(1, "nomatchbigsad", assignExpr2Partial));

    // Wrong expr
    AssignExpression wrongExact = AssignExpression("(999)", true);
    AssignExpression wrongPartial = AssignExpression("(999)", false);
    REQUIRE(!assignmentsTable.validate(1, "x1", wrongExact));
    REQUIRE(!assignmentsTable.validate(2, "x2", wrongPartial));
}

TEST_CASE("AssignmentsTable: getVar works correctly") {
    AssignmentsTable assignmentsTable;

    std::string expr1 = "((1)+(2))";
    Assignment assignment1 = Assignment(1, std::string("x1"), expr1);
    assignmentsTable.store(&assignment1);

    std::string expr2 = "((1)+(3))";
    Assignment assignment2 = Assignment(2, std::string("x2"), expr2);
    assignmentsTable.store(&assignment2);

    // Positive testing
    AssignExpression assignExpr1Exact = AssignExpression(expr1, true);
    AssignExpression assignExpr1Partial = AssignExpression("(1)", false);
    AssignExpression assignExpr2Exact = AssignExpression(expr2, true);
    AssignExpression assignExpr2Partial = AssignExpression("(3)", false);
    AssignExpression wildcard = AssignExpression("_", false);

    std::vector<Value> expectedResult = {Value(ValueType::VAR_NAME, "x1")};
    std::vector<Value> output = assignmentsTable.getVar(1, assignExpr1Exact);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = assignmentsTable.getVar(1, assignExpr1Partial);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = assignmentsTable.getVar(1, wildcard);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {Value(ValueType::VAR_NAME, "x2")};
    output = assignmentsTable.getVar(2, assignExpr2Exact);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = assignmentsTable.getVar(2, assignExpr2Partial);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = assignmentsTable.getVar(2, wildcard);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Wrong stmtNo
    expectedResult = {};
    output = assignmentsTable.getVar(2, assignExpr1Exact);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = assignmentsTable.getVar(1, assignExpr2Partial);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Wrong expr
    AssignExpression wrongExact = AssignExpression("(999)", true);
    AssignExpression wrongPartial = AssignExpression("(999)", false);
    output = assignmentsTable.getVar(1, wrongExact);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = assignmentsTable.getVar(2, wrongPartial);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AssignmentsTable: getTableSize works correctly") {
    AssignmentsTable assignmentsTable;

    std::string expr1 = "((1)+(2))";
    Assignment assignment1 = Assignment(1, std::string("x1"), expr1);
    assignmentsTable.store(&assignment1);

    std::string expr2 = "((1)+(3))";
    Assignment assignment2 = Assignment(2, std::string("x2"), expr2);
    assignmentsTable.store(&assignment2);

    REQUIRE(assignmentsTable.getTableSize() == 2);
}
