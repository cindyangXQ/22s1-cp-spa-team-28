#include "catch.hpp"

#include "PKB/RelationshipsTable.h"

TEST_CASE("RelationshipsTable can be initialised correctly") {
	RelationshipsTable<int, int> table;

	REQUIRE(table.getTableSize() == -1);
}

TEST_CASE("RelationshipsTable can store and retrieve correctly") {
	RelationshipsTable<int, int> table;
	Relationship<int, int> test = Relationship(RelationshipReference::Follows, 1, 2);

	table.store(&test);

	// successfully stored 1 item to left map
	REQUIRE(table.retrieveLeft(1).size() == 1);
	// successfully stored 1 item to right map
	REQUIRE(table.retrieveRight(2).size() == 1);
}

TEST_CASE("RelationshipsTable can storeLeft and retrieve correctly") {
	RelationshipsTable<int, int> table;

	table.storeRightToLeftMap(1, 2);

	// successfully stored 1 item to left map
	REQUIRE(table.retrieveLeft(1).size() == 1);
	// nothing stored to right map
	REQUIRE(table.retrieveRight(2).size() == 0);
}

TEST_CASE("RelationshipsTable can storeRight and retrieve correctly") {
	RelationshipsTable<int, int> table;

	table.storeLeftToRightMap(1, 2);

	// successfully stored 1 item to right map
	REQUIRE(table.retrieveRight(1).size() == 1);
	// nothing stored to left map
	REQUIRE(table.retrieveLeft(2).size() == 0);
}
