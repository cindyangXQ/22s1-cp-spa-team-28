#include "catch.hpp"

#include "PKB/RelationshipsTable.h"

TEST_CASE("RelationshipsTable can be initialised correctly") {
	RelationshipsTable<int, int> table;

	REQUIRE(table.getTableSize() == -1);
}