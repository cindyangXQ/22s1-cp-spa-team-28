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
	REQUIRE(assignmentsTable.retrieveFromVariable("x1").size() == 1);
	REQUIRE(assignmentsTable.retrieveFromExpression(expr).size() == 1);

	// values in maps are correct
	REQUIRE(assignmentsTable.retrieveFromVariable("x1").count(IntStringPair(1, expr)) == 1);
	REQUIRE(assignmentsTable.retrieveFromExpression(expr).count(IntStringPair(1, "x1")) == 1);
}