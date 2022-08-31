#include "PKB/Storage.h"

#include "catch.hpp"

TEST_CASE("Storage is initialised correctly") {
	Storage<TableValue> storage;
	NamesTable<ConstantName, Constant>* constants = (NamesTable<ConstantName, Constant>*) storage.getTable(TableName::CONSTANTS);
	NamesTable<VariableName, Variable>* variables = (NamesTable<VariableName, Variable>*) storage.getTable(TableName::VARIABLES);
	NamesTable<ProcedureName, Procedure>* procedures = (NamesTable<ProcedureName, Procedure>*) storage.getTable(TableName::PROCEDURES);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);

	REQUIRE(constants != nullptr);
	REQUIRE(variables != nullptr);
	REQUIRE(procedures != nullptr);
	REQUIRE(statements != nullptr);

}

TEST_CASE("Storage stores and retrieves correctly") {
	Storage<TableValue> storage;
	NamesTable<ConstantName, Constant>* constants = (NamesTable<ConstantName, Constant>*) storage.getTable(TableName::CONSTANTS);
	Constant test = Constant("Test");

	// constants->store(&test);

	// REQUIRE(constants->getTableSize() == 1);
}
