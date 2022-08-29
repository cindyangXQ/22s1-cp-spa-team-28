#include "PKB/StatementsTable.h"
#include "PKB/StatementPredicateMap.h"

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
	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 1);
	//test is stored in the correct bucket
	REQUIRE(table.getStatementType(test.getIndex()) == StatementType:: ASSIGN);
}

TEST_CASE("Successfully filters StatementsTable using StatementType") {
	StatementsTable table;
	Statement statement1 = Statement(1, StatementType::ASSIGN);
	Statement statement2 = Statement(2, StatementType::PRINT);
	Statement statement3 = Statement(3, StatementType::PRINT);

	table.store(&statement1);
	table.store(&statement2);
	table.store(&statement3);
	
	std::map<StatementHeader, Statement*> m = {{StatementHeader::STATEMENT_TYPE, &statement2}};
	StatementPredicateMap predicateMap = StatementPredicateMap(&m);

	// test is stored and retrieved correctly
	REQUIRE(*table.retrieve(statement2.getIndex()) == statement2);
	REQUIRE(*table.retrieve(statement3.getIndex()) == statement3);
	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 2);
}

TEST_CASE("Successfully filters StatementsTable using Index") {
	StatementsTable table;
	Statement statement1 = Statement(1, StatementType::ASSIGN);
	Statement statement2 = Statement(2, StatementType::PRINT);
	Statement statement3 = Statement(3, StatementType::PRINT);

	table.store(&statement1);
	table.store(&statement2);
	table.store(&statement3);
	
	std::map<StatementHeader, Statement*> m = {{StatementHeader::INDEX, &statement2}};
	StatementPredicateMap predicateMap = StatementPredicateMap(&m);

	// test is stored and retrieved correctly
	REQUIRE(*table.retrieve(statement2.getIndex()) == statement2);
	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 1);
}

TEST_CASE("Successfully filters StatementsTable using StatementType and Index") {
	StatementsTable table;
	Statement statement1 = Statement(1, StatementType::ASSIGN);
	Statement statement2 = Statement(2, StatementType::PRINT);
	Statement statement3 = Statement(3, StatementType::PRINT);

	table.store(&statement1);
	table.store(&statement2);
	table.store(&statement3);
	
	std::map<StatementHeader, Statement*> m = {{
		StatementHeader::INDEX, &statement2}, {StatementHeader::STATEMENT_TYPE, &statement1}};
	StatementPredicateMap predicateMap = StatementPredicateMap(&m);

	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 0);
}