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

TEST_CASE("Empty PredicateMap returns same StatementsTable") {
	StatementsTable table = StatementsTable();
	Statement statement1 = Statement(1, StatementType::ASSIGN);
	Statement statement2 = Statement(2, StatementType::PRINT);
	Statement statement3 = Statement(3, StatementType::PRINT);

	table.store(&statement1);
	table.store(&statement2);
	table.store(&statement3);
	
	std::map<StatementHeader, Statement*> m = {};
	StatementPredicateMap predicateMap = StatementPredicateMap(&m);
	StatementsTable *filteredTable = table.filter(&predicateMap);

	// test is stored and retrieved correctly
	REQUIRE(*filteredTable->retrieve(statement1.getIndex()) == statement1);
	REQUIRE(*filteredTable->retrieve(statement2.getIndex()) == statement2);
	REQUIRE(*filteredTable->retrieve(statement3.getIndex()) == statement3);
	// tableSize updated correctly
	REQUIRE(filteredTable->getTableSize() == 3);
}

TEST_CASE("Successfully filters StatementsTable using StatementType") {
	StatementsTable table = StatementsTable();
	Statement statement1 = Statement(1, StatementType::ASSIGN);
	Statement statement2 = Statement(2, StatementType::PRINT);
	Statement statement3 = Statement(3, StatementType::PRINT);

	table.store(&statement1);
	table.store(&statement2);
	table.store(&statement3);
	
	std::map<StatementHeader, Statement*> m = {{StatementHeader::STATEMENT_TYPE, &statement2}};
	StatementPredicateMap predicateMap = StatementPredicateMap(&m);
	StatementsTable *filteredTable = table.filter(&predicateMap);

	// test is stored and retrieved correctly
	REQUIRE(*filteredTable->retrieve(statement2.getIndex()) == statement2);
	REQUIRE(*filteredTable->retrieve(statement3.getIndex()) == statement3);
	// tableSize updated correctly
	REQUIRE(filteredTable->getTableSize() == 2);
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
	StatementsTable *filteredTable = table.filter(&predicateMap);


	// test is stored and retrieved correctly
	REQUIRE(*filteredTable->retrieve(statement2.getIndex()) == statement2);
	// tableSize updated correctly
	REQUIRE(filteredTable->getTableSize() == 1);
}

TEST_CASE("Successfully filters StatementsTable using StatementType and Index") {
	StatementsTable table;
	Statement statement1 = Statement(1, StatementType::ASSIGN);
	Statement statement2 = Statement(2, StatementType::PRINT);
	Statement statement3 = Statement(3, StatementType::PRINT);

	table.store(&statement1);
	table.store(&statement2);
	table.store(&statement3);
	
	std::map<StatementHeader, Statement*> m = {
		{StatementHeader::STATEMENT_TYPE, &statement1},
		{StatementHeader::INDEX, &statement2},
	};
	StatementPredicateMap predicateMap = StatementPredicateMap(&m);
	StatementsTable *filteredTable = table.filter(&predicateMap);

	// tableSize updated correctly
	REQUIRE(filteredTable->retrieve(statement2.getIndex()) == nullptr);

	REQUIRE(filteredTable->getTableSize() == 0);
}

TEST_CASE("StatementsTable can getAll statements correctly") {
	StatementsTable table;
	Statement test1 = Statement(1, StatementType::ASSIGN);
	Statement test2 = Statement(2, StatementType::ASSIGN);

	table.store(&test1);
	table.store(&test2);

	// number of elements in table is equal to number stored
	REQUIRE(table.getAll().size() == 2);
}

TEST_CASE("StatementsTable can getStatementByType correctly") {
	StatementsTable table;
	Statement assignStmt = Statement(1, StatementType::ASSIGN);
	Statement callStmt = Statement(1, StatementType::CALL);
	Statement ifStmt = Statement(1, StatementType::IF);
	Statement printStmt = Statement(1, StatementType::PRINT);
	Statement readStmt = Statement(1, StatementType::READ);
	Statement whileStmt = Statement(1, StatementType::WHILE);

	table.store(&assignStmt);
	table.store(&callStmt);
	table.store(&ifStmt);
	table.store(&printStmt);
	table.store(&readStmt);
	table.store(&whileStmt);

	// 1 statement of each type successfully retrieved
	REQUIRE(table.getStatementsByType(StatementType::ASSIGN).size() == 1);
	REQUIRE(table.getStatementsByType(StatementType::CALL).size() == 1);
	REQUIRE(table.getStatementsByType(StatementType::IF).size() == 1);
	REQUIRE(table.getStatementsByType(StatementType::PRINT).size() == 1);
	REQUIRE(table.getStatementsByType(StatementType::READ).size() == 1);
	REQUIRE(table.getStatementsByType(StatementType::WHILE).size() == 1);
}
