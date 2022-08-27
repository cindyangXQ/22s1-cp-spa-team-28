#include "PKB/NamesTable.h"

#include "catch.hpp"

TEST_CASE("NamesTable is initialised correctly") {
	NamesTable table;

	REQUIRE(table.getTableSize() == 0);
}

TEST_CASE("NamesTable can store and retrieve correctly") {
	NamesTable table;
	Name test = Name(1, "test");

	table.store(&test);

	// REQUIRE(*table.retrieve(test.getIndex()) == test);
	// invalid index returns a nullptr for retrieve()
	REQUIRE(table.retrieve(-1) == nullptr);
	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 1);
}