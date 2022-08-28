#include "PKB/NamesTable.h"
#include "PKB/NameDefinitions.h"
#include "PKB/Constant.h"

#include "catch.hpp"

TEST_CASE("NamesTable is initialised correctly") {
	NamesTable<ConstantName, Constant> table;

	REQUIRE(table.getTableSize() == 0);
}

TEST_CASE("NamesTable can store and retrieve correctly") {
	NamesTable<ConstantName, Constant> table;
	Constant test = Constant("Test");
	
	table.store(&test);

	// test is stored and retrieved correctly
	REQUIRE(*table.retrieve(test.getName()) == test);
	// invalid index returns a nullptr for retrieve()
	REQUIRE(table.retrieve((ConstantName)"not test") == nullptr);
	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 1);
}
