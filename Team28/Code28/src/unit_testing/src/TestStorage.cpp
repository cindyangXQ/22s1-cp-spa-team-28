#include "PKB/Storage.h"

#include "catch.hpp"

TEST_CASE("Storage is initialised correctly") {
	Storage<TableValue> storage;
	NamesTable<ConstantName, Constant>* constants = (NamesTable<ConstantName, Constant>*) storage.getTable(TableName::CONSTANTS);
	Constant test = Constant("Test");

	constants->store(&test);

	REQUIRE(constants->retrieve(test.getName()) == test);
}