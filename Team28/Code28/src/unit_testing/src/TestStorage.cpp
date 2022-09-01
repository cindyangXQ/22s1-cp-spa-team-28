#include "PKB/Storage.h"

#include "catch.hpp"

#include <iostream>
TEST_CASE("Storage is initialised correctly") {
	Storage storage;
	ConstantsTable* constants = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);
	VariablesTable* variables = (VariablesTable*) storage.getTable(TableName::VARIABLES);
	ProceduresTable* procedures = (ProceduresTable*) storage.getTable(TableName::PROCEDURES);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);

	REQUIRE(constants != nullptr);
	REQUIRE(variables != nullptr);
	REQUIRE(procedures != nullptr);
	REQUIRE(statements != nullptr);

}

TEST_CASE("Storage stores and retrieves Constants correctly") {
	Storage storage;
	ConstantsTable* constants = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);
	Constant test = Constant("Test");
	
	REQUIRE(constants->getTableSize() == 0);
	constants->store(&test);

	REQUIRE(constants->getTableSize() == 1);
	REQUIRE(*constants->retrieve(test.getName()) == test);
}

TEST_CASE("Storage stores and retrieves Statements correctly") {
	Storage storage;
	StatementsTable* statements = (StatementsTable*) storage.getTable(TableName::STATEMENTS);
	REQUIRE(statements->getTableSize() == 0);

	Statement test = Statement(1, StatementType::ASSIGN);
	statements->store(&test);

	REQUIRE(statements->getTableSize() == 1);
	REQUIRE(*statements->retrieve(test.getIndex()) == test);
}