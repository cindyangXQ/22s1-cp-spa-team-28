#include "catch.hpp"

#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"
#include "commons/Relationship.h"

TEST_CASE("Relationship can be initialised correctly") {
	Relationship<int, int> test = Relationship(RelationshipReference::PARENT, 1, 1);

	// attributes of Relationship are correctly initialised
	REQUIRE(test.getRelationshipReference() == RelationshipReference::PARENT);
	REQUIRE(test.getLeft() == 1);
	REQUIRE(test.getRight() == 1);
}

TEST_CASE("Relationship can be parsed correctly") {
    SolvableQuery solvableQ = QueryParser::parse("assign a; constant c; variable v; Select a such that Modifies(1, v) pattern a(v, \"x\")");

    REQUIRE(solvableQ.suchThatCl.relationship == RelationshipReference::MODIFIES);
    REQUIRE(solvableQ.suchThatCl.refRight.isSynonym == true);
    REQUIRE(solvableQ.suchThatCl.refRight.syn.entity == EntityName::VARIABLE);
}

TEST_CASE("Relationship can be validated") {
    Reference stmtRef = Reference("1");
    Reference entRef = Reference("a");
}
