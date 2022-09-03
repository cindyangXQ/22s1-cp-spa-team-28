#include "PKB/PopulateFacade.h"

#include "catch.hpp"

TEST_CASE("storeStatement stores Statement objects correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Statement test1 = Statement(1, StatementType::ASSIGN);
	Statement test2 = Statement(2, StatementType::ASSIGN);

	facade.storeStatement(&test1);
	facade.storeStatement(&test2);

	StatementsTable* statements = (StatementsTable*) storage.getTable(TableName::STATEMENTS);

	// returned number of statements is equal to number stored
	REQUIRE(statements->getTableSize() == 2);
	// first statement is test1
	REQUIRE(*statements->retrieve(1) == test1);
	// second statement is test2
	REQUIRE(*statements->retrieve(2) == test2);
}

TEST_CASE("storeVariable stores Variable objects correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Variable test1 = Variable("a");
	Variable test2 = Variable("b");
	Variable test3 = Variable("");

	facade.storeVariable(&test1);
	facade.storeVariable(&test2);
	facade.storeVariable(&test3);

	VariablesTable* variables = (VariablesTable*) storage.getTable(TableName::VARIABLES);

	// returned number of variables is equal to number stored
	REQUIRE(variables->getTableSize() == 3);
	// first variable is test1
	REQUIRE(*variables->retrieve("a") == test1);
	// second variable is test2
	REQUIRE(*variables->retrieve("b") == test2);
	// third variable is test3
	REQUIRE(*variables->retrieve("") == test3);
}

TEST_CASE("storeConstant stores Constant objects correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Constant test1 = Constant("a");
	Constant test2 = Constant("b");
	Constant test3 = Constant("");

	facade.storeConstant(&test1);
	facade.storeConstant(&test2);
	facade.storeConstant(&test3);

	ConstantsTable* constants = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);

	// returned number of constants is equal to number stored
	REQUIRE(constants->getTableSize() == 3);
	// first constant is test1
	REQUIRE(*constants->retrieve("a") == test1);
	// second constant is test2
	REQUIRE(*constants->retrieve("b") == test2);
	// third constant is test3
	REQUIRE(*constants->retrieve("") == test3);
}

TEST_CASE("storeProcedure stores Procedure objects correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Procedure test1 = Procedure("a");
	Procedure test2 = Procedure("b");
	Procedure test3 = Procedure("");

	facade.storeProcedure(&test1);
	facade.storeProcedure(&test2);
	facade.storeProcedure(&test3);

	ProceduresTable* procedures = (ProceduresTable*) storage.getTable(TableName::PROCEDURES);

	// returned number of procedures is equal to number stored
	REQUIRE(procedures->getTableSize() == 3);
	// first procedure is test1
	REQUIRE(*procedures->retrieve("a") == test1);
	// second procedure is test2
	REQUIRE(*procedures->retrieve("b") == test2);
	// third procedure is test3
	REQUIRE(*procedures->retrieve("") == test3);
}

TEST_CASE("storeProcedure does not affect other tables") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Procedure test = Procedure("a");

	facade.storeProcedure(&test);

	ProceduresTable* procedures = (ProceduresTable*) storage.getTable(TableName::PROCEDURES);
	VariablesTable* variables = (VariablesTable*) storage.getTable(TableName::VARIABLES);
	ConstantsTable* constants = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);
	StatementsTable* statements = (StatementsTable*) storage.getTable(TableName::STATEMENTS);	

	// returned number of procedures is equal to number stored
	REQUIRE(procedures->getTableSize() == 1);
	// returned number of other tables is equal to 0
	REQUIRE(variables->getTableSize() == 0);
	REQUIRE(constants->getTableSize() == 0);
	REQUIRE(statements->getTableSize() == 0);

}