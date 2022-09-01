#include "PKB/Storage.h"

#include "catch.hpp"

#include <iostream>

TEST_CASE("Storage is initialised correctly") {
	Storage storage;
	ConstantsTable* constants = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);
	VariablesTable* variables = (VariablesTable*) storage.getTable(TableName::VARIABLES);
	ProceduresTable* procedures = (ProceduresTable*) storage.getTable(TableName::PROCEDURES);
	StatementsTable* statements = (StatementsTable*) storage.getTable(TableName::STATEMENTS);

	/*
	* Check that all tables are instantiated.
	*/
	REQUIRE(constants != nullptr);
	REQUIRE(variables != nullptr);
	REQUIRE(procedures != nullptr);
	REQUIRE(statements != nullptr);
}

TEST_CASE("Storage stores and retrieves Constants correctly") {
	Storage storage;
	ConstantsTable* constants = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);
	Constant test = Constant("Test");
	
	// ConstantsTable is initialised correctly
	REQUIRE(constants->getTableSize() == 0);

	constants->store(&test);

	// Constant stored to ConstantsTable correctly
	REQUIRE(constants->getTableSize() == 1);
	// Constant retrieved from ConstantsTable correctly
	REQUIRE(*constants->retrieve(test.getName()) == test);
}

TEST_CASE("Storage stores and retrieves Statements correctly") {
	Storage storage;
	StatementsTable* statements = (StatementsTable*) storage.getTable(TableName::STATEMENTS);
	Statement test = Statement(1, StatementType::ASSIGN);

	// StatementsTable is initialised correctly
	REQUIRE(statements->getTableSize() == 0);

	statements->store(&test);

	// Statement stored to StatementsTable correctly
	REQUIRE(statements->getTableSize() == 1);
	// Statement retrieved from StatementsTable correctly
	REQUIRE(*statements->retrieve(test.getIndex()) == test);
}

TEST_CASE("Storage stores and retrieves Variables correctly") {
	Storage storage;
	VariablesTable* variables = (VariablesTable*) storage.getTable(TableName::VARIABLES);
	Variable test = Variable("test");

	// StatementsTable is initialised correctly
	REQUIRE(variables->getTableSize() == 0);

	variables->store(&test);

	// Variable stored to VariablesTable correctly
	REQUIRE(variables->getTableSize() == 1);
	// Variable retrieved from VariablesTable correctly
	REQUIRE(*variables->retrieve(test.getName()) == test);
}

TEST_CASE("Storage stores and retrieves Procedures correctly") {
	Storage storage;
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::VARIABLES);
	Procedure test = Procedure("test");

	// ProceduresTable is initialised correctly
	REQUIRE(procedures->getTableSize() == 0);

	procedures->store(&test);

	// Procedure stored to ProceduresTable correctly
	REQUIRE(procedures->getTableSize() == 1);
	// Procedure retrieved from ProceduresTable correctly
	REQUIRE(*procedures->retrieve(test.getName()) == test);
}
