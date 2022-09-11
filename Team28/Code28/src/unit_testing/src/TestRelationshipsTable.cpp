#include "catch.hpp"

#include "PKB/RelationshipsTable.h"

TEST_CASE("RelationshipsTable can be initialised correctly") {
	RelationshipsTable<int, int> table;

	REQUIRE(table.getTableSize() == -1);
}

TEST_CASE("RelationshipsTable can store and retrieve correctly") {
	RelationshipsTable<int, int> table;
	Relationship<int, int> test = Relationship(RelationshipReference::FOLLOWS, 1, 2);

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

TEST_CASE("Empty PredicateMap returns same RelationshipsTable") {
	RelationshipsTable<int, int> table;
	Relationship<int, int> test1 = Relationship(RelationshipReference::FOLLOWS, 1, 2);
	Relationship<int, int> test2 = Relationship(RelationshipReference::FOLLOWS, 2, 3);
	Relationship<int, int> test3 = Relationship(RelationshipReference::FOLLOWS, 3, 4);

	table.store(&test1);
	table.store(&test2);
	table.store(&test3);

	std::map<RelationshipHeader, Relationship<int, int>*> m = {};
	RelationshipPredicateMap predicateMap = RelationshipPredicateMap(&m);
	RelationshipsTable<int, int>* filteredTable = table.filter(&predicateMap);

	REQUIRE(filteredTable->getLeftMap().size() == 3);
	REQUIRE(filteredTable->getRightMap().size() == 3);
}

TEST_CASE("Successfully filters RelationshipsTable using CHECK_LEFT") {
	RelationshipsTable<int, int> table;
	Relationship<int, int> test1 = Relationship(RelationshipReference::FOLLOWS, 1, 2);
	Relationship<int, int> test2 = Relationship(RelationshipReference::FOLLOWS, 2, 3);
	Relationship<int, int> test3 = Relationship(RelationshipReference::FOLLOWS, 3, 4);

	table.store(&test1);
	table.store(&test2);
	table.store(&test3);

	std::map<RelationshipHeader, Relationship<int, int>*> m = {{RelationshipHeader::CHECK_LEFT, &test1}};
	RelationshipPredicateMap predicateMap = RelationshipPredicateMap(&m);
	RelationshipsTable<int, int>* filteredTable = table.filter(&predicateMap);

	REQUIRE(filteredTable->getLeftMap().size() == 0);
	REQUIRE(filteredTable->getRightMap().size() == 1);
}

TEST_CASE("Successfully filters RelationshipsTable using CHECK_RIGHT") {
	RelationshipsTable<int, int> table;
	Relationship<int, int> test1 = Relationship(RelationshipReference::FOLLOWS, 1, 2);
	Relationship<int, int> test2 = Relationship(RelationshipReference::FOLLOWS, 2, 3);
	Relationship<int, int> test3 = Relationship(RelationshipReference::FOLLOWS, 3, 4);

	table.store(&test1);
	table.store(&test2);
	table.store(&test3);

	std::map<RelationshipHeader, Relationship<int, int>*> m = {{RelationshipHeader::CHECK_RIGHT, &test1}};
	RelationshipPredicateMap predicateMap = RelationshipPredicateMap(&m);
	RelationshipsTable<int, int>* filteredTable = table.filter(&predicateMap);

	REQUIRE(filteredTable->getLeftMap().size() == 1);
	REQUIRE(filteredTable->getRightMap().size() == 0);
}

TEST_CASE("Successfully filters RelationshipsTable using both CHECK_LEFT and CHECK_RIGHT") {
	RelationshipsTable<int, int> table;
	Relationship<int, int> test1 = Relationship(RelationshipReference::FOLLOWS, 1, 2);
	Relationship<int, int> test2 = Relationship(RelationshipReference::FOLLOWS, 2, 3);
	Relationship<int, int> test3 = Relationship(RelationshipReference::FOLLOWS, 3, 4);

	table.store(&test1);
	table.store(&test2);
	table.store(&test3);

	std::map<RelationshipHeader, Relationship<int, int>*> m = {{RelationshipHeader::CHECK_LEFT, &test1}, {RelationshipHeader::CHECK_RIGHT, &test2}};
	RelationshipPredicateMap predicateMap = RelationshipPredicateMap(&m);
	RelationshipsTable<int, int>* filteredTable = table.filter(&predicateMap);

	REQUIRE(filteredTable->getLeftMap().size() == 1);
	REQUIRE(filteredTable->getRightMap().size() == 1);
}
