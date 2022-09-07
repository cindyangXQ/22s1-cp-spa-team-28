#include "catch.hpp"

#include "commons/Relationship.h"

TEST_CASE("Relationship can be initialised correctly") {
	Relationship<int, int> test = Relationship(RelationshipReference::Parent, 1, 1);

	// attributes of Relationship are correctly initialised
	REQUIRE(test.getName() == RelationshipReference::Parent);
	REQUIRE(test.getLeft() == 1);
	REQUIRE(test.getRight() == 1);
}
