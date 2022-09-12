#include "QPS/Evaluator/ClauseTable.h"
#include "Commons/Synonym.h"
#include "Commons/Tuple.h"
#include <vector>

#include "catch.hpp"

TEST_CASE("ClauseTable is initialised correctly") {
	std::vector<Synonym> syns{ Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a") };
	ClauseTable table = ClauseTable(syns);
	REQUIRE(table.size() == 0);
}

TEST_CASE("ClauseTable can insert tuple") {
	std::vector<Synonym> syns{ Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a") };
	ClauseTable table = ClauseTable(syns);
	REQUIRE(table.size() == 0);
	table.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "foo"), Value(ValueType::STMT_NUM, "2")}));
	REQUIRE(table.size() == 1);
	table.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "yey"), Value(ValueType::STMT_NUM, "5") }));
	REQUIRE(table.size() == 2);
}

TEST_CASE("ClauseTable can get common headers from two tables") {
	Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
	Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
	Synonym syn3 = Synonym(EntityName::STMT, "s");
	std::vector<Synonym> header1{ syn1, syn2 };
	std::vector<Synonym> header2{ syn2, syn3 };
	ClauseTable table1 = ClauseTable(header1);
	ClauseTable table2 = ClauseTable(header2);
	std::vector<Synonym> common_headers = ClauseTable::getCommonHeaders(table1, table2);
	REQUIRE(common_headers.size() == 1);
}

TEST_CASE("ClauseTable can get indices of common headers from a tables") {
	Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
	Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
	Synonym syn3 = Synonym(EntityName::STMT, "s");
	std::vector<Synonym> header1{ syn1, syn2, syn3 };
	std::vector<Synonym> common_header{ syn3, syn1 };
	ClauseTable table1 = ClauseTable(header1);
	std::vector<int> indices = table1.getIndices(common_header);
	REQUIRE(indices.size() == 2);
	REQUIRE(indices[0] == 2);
	REQUIRE(indices[1] ==0);
}

TEST_CASE("ClauseTable can create new table from two input table") {
	Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
	Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
	Synonym syn3 = Synonym(EntityName::STMT, "s");
	std::vector<Synonym> header1{ syn1, syn2 };
	std::vector<Synonym> header2{ syn2, syn3 };
	ClauseTable table1 = ClauseTable(header1);
	ClauseTable table2 = ClauseTable(header2);
	ClauseTable tableJoin = ClauseTable::ConstructTable(table1, table2);
	REQUIRE(tableJoin.header.size() == 3);
}
