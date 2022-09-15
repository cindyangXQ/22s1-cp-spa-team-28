#include "QPS/Evaluator/ClauseTable.h"
#include "commons/Synonym.h"
#include "QPS/Evaluator/Tuple.h"
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
	REQUIRE(tableJoin.header[0].name == syn2.name);
	REQUIRE(tableJoin.header[1].name == syn1.name);
	REQUIRE(tableJoin.header[2].name == syn3.name);
}

TEST_CASE("ClauseTable can join two tables with common headers") {
	Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
	Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
	Synonym syn3 = Synonym(EntityName::STMT, "s");
	std::vector<Synonym> header1{ syn1, syn2 };
	std::vector<Synonym> header2{ syn3, syn2 };
	ClauseTable table1 = ClauseTable(header1);
	ClauseTable table2 = ClauseTable(header2);
	table1.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "foo"), Value(ValueType::STMT_NUM, "2") }));
	table1.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "bar"), Value(ValueType::STMT_NUM, "3") }));
	table1.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "baz"), Value(ValueType::STMT_NUM, "4") }));
	table2.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "yey"), Value(ValueType::STMT_NUM, "3") }));
	table2.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "lol"), Value(ValueType::STMT_NUM, "4") }));
	table2.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "wtf"), Value(ValueType::STMT_NUM, "5") }));
	ClauseTable result = ClauseTable::joinTables(table1, table2);
	REQUIRE(result.size() == 2);
	REQUIRE(result.header.size() == 3);
	REQUIRE(result.header[0].name == syn2.name);
	REQUIRE(result.header[1].name == syn1.name);
	REQUIRE(result.header[2].name == syn3.name);
}

TEST_CASE("ClauseTable can join two tables with no common headers") {
	Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
	Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
	Synonym syn3 = Synonym(EntityName::STMT, "s");
	Synonym syn4 = Synonym(EntityName::STMT, "s1");
	std::vector<Synonym> header1{ syn1, syn2 };
	std::vector<Synonym> header2{ syn3, syn4 };
	ClauseTable table1 = ClauseTable(header1);
	ClauseTable table2 = ClauseTable(header2);
	table1.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "foo"), Value(ValueType::STMT_NUM, "2") }));
	table1.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "bar"), Value(ValueType::STMT_NUM, "3") }));
	table1.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "baz"), Value(ValueType::STMT_NUM, "4") }));
	table2.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "yey"), Value(ValueType::STMT_NUM, "3") }));
	table2.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "lol"), Value(ValueType::STMT_NUM, "4") }));
	table2.insert(Tuple(std::vector{ Value(ValueType::VAR_NAME, "wtf"), Value(ValueType::STMT_NUM, "5") }));
	ClauseTable result = ClauseTable::joinTables(table1, table2);
	REQUIRE(result.size() == 9);
	REQUIRE(result.header.size() == 4);
	REQUIRE(result.header[0].name == syn1.name);
	REQUIRE(result.header[1].name == syn2.name);
	REQUIRE(result.header[2].name == syn3.name);
	REQUIRE(result.header[3].name == syn4.name);
}
