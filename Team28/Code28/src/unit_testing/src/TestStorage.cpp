#include "PKB/Storage.h"

#include "catch.hpp"

#include <iostream>

TEST_CASE("Storage is initialised correctly") {
	Storage storage;
	ConstantsTable* constants = (ConstantsTable*) storage.getTable(TableName::CONSTANTS);
	VariablesTable* variables = (VariablesTable*) storage.getTable(TableName::VARIABLES);
	ProceduresTable* procedures = (ProceduresTable*) storage.getTable(TableName::PROCEDURES);
	StatementsTable* statements = (StatementsTable*) storage.getTable(TableName::STATEMENTS);

	// Check that all tables are instantiated.
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
	REQUIRE(*statements->retrieve(test.getLineNumber()) == test);
}

TEST_CASE("Storage stores and retrieves Assignments correctly") {
	Storage storage;
	AssignmentsTable* assignments = (AssignmentsTable*) storage.getTable(TableName::ASSIGNMENTS);
	Assignment test = Assignment(1, "x1", "(1)");

	assignments->store(&test);

	// Assignment stored to AssignnmentsTable correctly
	REQUIRE(assignments->retrieveFromVariable("x1").size() == 1);
	// Assignment retrieved from Assignments correctly
	REQUIRE(assignments->retrieveFromVariable("x1").count(IntStringPair(1, "(1)")) == 1);
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

TEST_CASE("Storage stores and retrieves Parent correctly") {
	Storage storage;
	ParentTable* parents = (ParentTable*)storage.getTable(TableName::PARENT);
	Relationship<int, int> test = Relationship(RelationshipReference::PARENT, 1, 2);

	parents->store(&test);

	// Relationship stored to ParentTable correctly
	REQUIRE(parents->retrieveLeft(1).size() == 1);
	REQUIRE(parents->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves ParentT correctly") {
	Storage storage;
	ParentTTable* parentsT = (ParentTTable*)storage.getTable(TableName::PARENT_T);
	Relationship<int, int> test = Relationship(RelationshipReference::PARENT_T, 1, 2);

	parentsT->store(&test);

	// Relationship stored to ParentTTable correctly
	REQUIRE(parentsT->retrieveLeft(1).size() == 1);
	REQUIRE(parentsT->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves Follows correctly") {
	Storage storage;
	FollowsTable* follows = (FollowsTable*)storage.getTable(TableName::FOLLOWS);
	Relationship<int, int> test = Relationship(RelationshipReference::FOLLOWS, 1, 2);

	follows->store(&test);

	// Relationship stored to FollowsTable correctly
	REQUIRE(follows->retrieveLeft(1).size() == 1);
	REQUIRE(follows->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves FollowsT correctly") {
	Storage storage;
	FollowsTTable* followsT = (FollowsTTable*)storage.getTable(TableName::FOLLOWS_T);
	Relationship<int, int> test = Relationship(RelationshipReference::FOLLOWS_T, 1, 2);

	followsT->store(&test);

	// Relationship stored to FollowsTTable correctly
	REQUIRE(followsT->retrieveLeft(1).size() == 1);
	REQUIRE(followsT->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves ModifiesS correctly") {
	Storage storage;
	ModifiesSTable* modifiesS = (ModifiesSTable*)storage.getTable(TableName::MODIFIES_S);
	Relationship<int, std::string> test = Relationship(RelationshipReference::MODIFIES, 1, std::string("v"));

	modifiesS->store(&test);

	// Relationship stored to ModifiesS correctly
	REQUIRE(modifiesS->retrieveLeft(1).size() == 1);
	REQUIRE(modifiesS->retrieveRight("v").size() == 1);
}

TEST_CASE("Storage stores and retrieves ModifiesP correctly") {
	Storage storage;
	ModifiesPTable* modifiesP = (ModifiesPTable*)storage.getTable(TableName::MODIFIES_P);
	Relationship<std::string, std::string> test = Relationship(
		RelationshipReference::MODIFIES, std::string("main"), std::string("v")
	);

	modifiesP->store(&test);

	// Relationship stored to ModifiesS correctly
	REQUIRE(modifiesP->retrieveLeft("main").size() == 1);
	REQUIRE(modifiesP->retrieveRight("v").size() == 1);
}

TEST_CASE("Storage stores and retrieves UsesS correctly") {
	Storage storage;
	UsesSTable* usesS = (UsesSTable*)storage.getTable(TableName::USES_S);
	Relationship<int, std::string> test = Relationship(RelationshipReference::USES, 1, std::string("v"));

	usesS->store(&test);

	// Relationship stored to ModifiesS correctly
	REQUIRE(usesS->retrieveLeft(1).size() == 1);
	REQUIRE(usesS->retrieveRight("v").size() == 1);
}

TEST_CASE("Storage stores and retrieves UsesP correctly") {
	Storage storage;
	UsesPTable* usesP = (UsesPTable*)storage.getTable(TableName::USES_P);
	Relationship<std::string, std::string> test = Relationship(
		RelationshipReference::USES, std::string("main"), std::string("v")
	);

	usesP->store(&test);

	// Relationship stored to ModifiesS correctly
	REQUIRE(usesP->retrieveLeft("main").size() == 1);
	REQUIRE(usesP->retrieveRight("v").size() == 1);
}