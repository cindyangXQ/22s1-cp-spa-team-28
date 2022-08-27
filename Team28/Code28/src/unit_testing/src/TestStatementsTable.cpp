#include "PKB/StatementsTable.h"

#include "catch.hpp"

TEST_CASE("StatementsTable is initialised correctly") {
	StatementsTable table;

	REQUIRE(table.getTableSize() == 0);
}

TEST_CASE("StatementsTable can store and retrieve a statement of type not NONE") {
	StatementsTable table;
	Statement test = Statement(1, StatementType::ASSIGN);

	table.store(&test);

	// test is stored and retrieved correctly
	REQUIRE(*table.retrieve(test.getIndex()) == test);
	// invalid index returns a nullptr for retrieve()
	REQUIRE(table.retrieve(-1) == nullptr);
}
