#include <vector>

#include "PKB/PopulateFacade.h"

#include "catch.hpp"

TEST_CASE("storeStatement stores empty vector<Statement*> correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	std::vector<Statement*> statements = {};

	facade.storeStatements(&statements);

	StatementsTable* statementsTable = (StatementsTable*) storage.getTable(TableName::STATEMENTS);

	// returned number of statements is equal to 0
	REQUIRE(statementsTable->getTableSize() == 0);
}

TEST_CASE("storeStatement stores Statement objects correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Statement test1 = Statement(1, StatementType::ASSIGN);
	Statement test2 = Statement(2, StatementType::ASSIGN);
	std::vector<Statement*> statements = {&test1, &test2};

	facade.storeStatements(&statements);

	StatementsTable* statementsTable = (StatementsTable*) storage.getTable(TableName::STATEMENTS);

	// returned number of statements is equal to number stored
	REQUIRE(statementsTable->getTableSize() == 2);
	// first statement is test1
	REQUIRE(*statementsTable->retrieve(1) == test1);
	// second statement is test2
	REQUIRE(*statementsTable->retrieve(2) == test2);
}

TEST_CASE("storeVariable stores Variable objects correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Variable test1 = Variable("a");
	Variable test2 = Variable("b");
	Variable test3 = Variable("");
	std::vector<Variable*> variables = {&test1, &test2, &test3};

	facade.storeVariables(&variables);

	VariablesTable* variablesTable = (VariablesTable*) storage.getTable(TableName::VARIABLES);

	// returned number of variables is equal to number stored
	REQUIRE(variablesTable->getTableSize() == 3);
	// first variable is test1
	REQUIRE(*variablesTable->retrieve("a") == test1);
	// second variable is test2
	REQUIRE(*variablesTable->retrieve("b") == test2);
	// third variable is test3
	REQUIRE(*variablesTable->retrieve("") == test3);
}

TEST_CASE("storeConstant stores Constant objects correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Constant test1 = Constant("a");
	Constant test2 = Constant("b");
	Constant test3 = Constant("");
	std::vector<Constant*> constants = {&test1, &test2, &test3};

	facade.storeConstants(&constants);

	ConstantsTable* constantsTable = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);

	// returned number of constants is equal to number stored
	REQUIRE(constantsTable->getTableSize() == 3);
	// first constant is test1
	REQUIRE(*constantsTable->retrieve("a") == test1);
	// second constant is test2
	REQUIRE(*constantsTable->retrieve("b") == test2);
	// third constant is test3
	REQUIRE(*constantsTable->retrieve("") == test3);
}

TEST_CASE("storeProcedure stores Procedure objects correctly") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Procedure test1 = Procedure("a");
	Procedure test2 = Procedure("b");
	Procedure test3 = Procedure("");
	std::vector<Procedure*> procedures = {&test1, &test2, &test3};

	facade.storeProcedures(&procedures);

	ProceduresTable* proceduresTable = (ProceduresTable*) storage.getTable(TableName::PROCEDURES);

	// returned number of procedures is equal to number stored
	REQUIRE(proceduresTable->getTableSize() == 3);
	// first procedure is test1
	REQUIRE(*proceduresTable->retrieve("a") == test1);
	// second procedure is test2
	REQUIRE(*proceduresTable->retrieve("b") == test2);
	// third procedure is test3
	REQUIRE(*proceduresTable->retrieve("") == test3);
}

TEST_CASE("storeProcedure does not affect other tables") {
	Storage storage;
	PopulateFacade facade = PopulateFacade(&storage);
	Procedure test = Procedure("a");
	std::vector<Procedure*> procedures = {&test};

	facade.storeProcedures(&procedures);

	ProceduresTable* proceduresTable = (ProceduresTable*) storage.getTable(TableName::PROCEDURES);
	VariablesTable* variablesTable = (VariablesTable*) storage.getTable(TableName::VARIABLES);
	ConstantsTable* constantsTable = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);
	StatementsTable* statementsTable = (StatementsTable*) storage.getTable(TableName::STATEMENTS);	

	// returned number of procedures is equal to number stored
	REQUIRE(proceduresTable->getTableSize() == 1);
	// returned number of other tables is equal to 0
	REQUIRE(variablesTable->getTableSize() == 0);
	REQUIRE(constantsTable->getTableSize() == 0);
	REQUIRE(statementsTable->getTableSize() == 0);

}