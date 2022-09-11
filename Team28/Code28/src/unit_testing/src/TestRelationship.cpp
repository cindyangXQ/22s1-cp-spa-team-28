#include "catch.hpp"

#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"
#include "commons/Relationship/Relationship.h"
#include "commons/Relationship/Follows.h"
#include "commons/Relationship/Modifies.h"

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
    Modifies modifiesValid = Modifies(RelationshipReference::MODIFIES, entRef, entRef);
    Modifies modifiesInvalid = Modifies(RelationshipReference::MODIFIES, stmtRef, stmtRef);
    Follows followsValid = Follows(RelationshipReference::FOLLOWS, stmtRef, stmtRef);
    Follows followsInvalid = Follows(RelationshipReference::FOLLOWS, entRef, entRef);

    REQUIRE(modifiesValid.isValid() == true);
    REQUIRE(modifiesInvalid.isValid() == false);
    REQUIRE(followsValid.isValid() == true);
    REQUIRE(followsInvalid.isValid() == false);
}
