#include "PKB/QueryFacade.h"

#include "catch.hpp"

TEST_CASE("getAllStatements returns all statements correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	Statement test1 = Statement(1, StatementType::ASSIGN);
	Statement test2 = Statement(2, StatementType::ASSIGN);

	statements->store(&test1);
	statements->store(&test2);

	// returned number of statements is equal to number stored
	REQUIRE(facade.getAllStatements().size() == 2);
	// first statement is test1
	REQUIRE(*facade.getAllStatements().at(0) == test1);
	// second statement is test2
	REQUIRE(*facade.getAllStatements().at(1) == test2);
}


TEST_CASE("getAllStatementByType returns all While statements correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	Statement test1 = Statement(1, StatementType::ASSIGN);
	Statement test2 = Statement(2, StatementType::ASSIGN);
	Statement test3 = Statement(3, StatementType::WHILE);
	Statement test4 = Statement(3, StatementType::IF);
	Statement test5 = Statement(3, StatementType::WHILE);

	statements->store(&test1);
	statements->store(&test2);
	statements->store(&test3);
	statements->store(&test4);
	statements->store(&test5);

	// returned number of statements is equal to number stored
	REQUIRE(facade.getAllStatementsByType(StatementType::WHILE).size() == 2);
	// first statement is test1
	REQUIRE(*facade.getAllStatementsByType(StatementType::WHILE).at(0) == test3);
	// second statement is test2
	REQUIRE(*facade.getAllStatementsByType(StatementType::WHILE).at(1) == test5);
}

TEST_CASE("getAllVariables returns all variables correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	// returned number of variables is equal to number stored
	REQUIRE(facade.getAllVariables().size() == 2);
}

TEST_CASE("getAllConstants returns all constants correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ConstantsTable* constants = (ConstantsTable*)storage.getTable(TableName::CONSTANTS);
	Constant test1 = Constant("test1");
	Constant test2 = Constant("test2");

	constants->store(&test1);
	constants->store(&test2);

	// returned number of variables is equal to number stored
	REQUIRE(facade.getAllConstants().size() == 2);
}

TEST_CASE("getAllProcedures returns all constants correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
	Procedure test1 = Procedure("test1");
	Procedure test2 = Procedure("test2");

    procedures->store(&test1);
    procedures->store(&test2);

	// returned number of procedures is equal to number stored
	REQUIRE(facade.getAllProcedures().size() == 2);
}

TEST_CASE("getStatementByLineNo retrieves statement correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	Statement test = Statement(1, StatementType::ASSIGN);

	statements->store(&test);

	REQUIRE(*facade.getStatementByLineNo(test.getLineNumber()) == test);
}

TEST_CASE("getProcedureByName retrieves procedure correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
	Procedure test = Procedure("test");

	procedures->store(&test);

	REQUIRE(*facade.getProcedureByName(test.getName()) == test);
}

TEST_CASE("getConstantByName retrieves procedure correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ConstantsTable* constants = (ConstantsTable*)storage.getTable(TableName::CONSTANTS);
	Constant test = Constant("test");

	constants->store(&test);

	REQUIRE(*facade.getConstantByName(test.getName()) == test);
}

TEST_CASE("getVariableByName retrieves procedure correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test = Variable("test");

	variables->store(&test);

	REQUIRE(*facade.getVariableByName(test.getName()) == test);
}

TEST_CASE("StmtToStmt: Validate returns correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	FollowsTable* follows = (FollowsTable*)storage.getTable(TableName::FOLLOWS);
	
	Statement line1 = Statement(1, StatementType::ASSIGN);
	Statement line2 = Statement(2, StatementType::ASSIGN);
	Relationship<int, int> rs = Relationship(RelationshipReference::FOLLOWS, 1, 2);
	statements->store(&line1);
	statements->store(&line2);
	follows->store(&rs);

	Reference leftRef = Reference("1");
	Reference rightRef = Reference("2");
	Reference wildcardRef = Reference("_");

	// Queries where relationships do exist
	REQUIRE(facade.validate(RelationshipReference::FOLLOWS, leftRef, rightRef));
	REQUIRE(facade.validate(RelationshipReference::FOLLOWS, leftRef, wildcardRef));
	REQUIRE(facade.validate(RelationshipReference::FOLLOWS, wildcardRef, rightRef));
	REQUIRE(facade.validate(RelationshipReference::FOLLOWS, wildcardRef, wildcardRef));

	// Queries where relationships do not exist
	REQUIRE(!facade.validate(RelationshipReference::FOLLOWS, rightRef, rightRef));
	REQUIRE(!facade.validate(RelationshipReference::FOLLOWS, leftRef, leftRef));
	REQUIRE(!facade.validate(RelationshipReference::FOLLOWS, rightRef, wildcardRef));
	REQUIRE(!facade.validate(RelationshipReference::FOLLOWS, wildcardRef, leftRef));
}

TEST_CASE("StmtToStmt: SolveRight queries for Follows(1, 2) return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	FollowsTable* follows = (FollowsTable*)storage.getTable(TableName::FOLLOWS);
	
	Statement line1 = Statement(1, StatementType::ASSIGN);
	Statement line2 = Statement(2, StatementType::ASSIGN);
	Relationship<int, int> rs = Relationship(RelationshipReference::FOLLOWS, 1, 2);
	statements->store(&line1);
	statements->store(&line2);
	follows->store(&rs);

	Reference leftRef;
	EntityName rightEntityName;
	std::vector<Value> expectedResult;
	std::vector<Value> output;
	
	// SolveRight(Follows, 1, Assign) returns {'2'}
	leftRef = Reference("1");
	rightEntityName = EntityName::ASSIGN;
	expectedResult = {Value(ValueType::STMT_NUM, "2")};
	output = facade.solveRight(RelationshipReference::FOLLOWS, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Follows, 2, Assign) returns {}
	leftRef = Reference("2");
	rightEntityName = EntityName::ASSIGN;
	expectedResult = {};
	output = facade.solveRight(RelationshipReference::FOLLOWS, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Follows, 1, Print) returns {}
	leftRef = Reference("1");
	rightEntityName = EntityName::PRINT;
	expectedResult = {};
	output = facade.solveRight(RelationshipReference::FOLLOWS, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Follows, _, Assign) returns {'2'}
	leftRef = Reference("_");
	rightEntityName = EntityName::ASSIGN;
	expectedResult = {Value(ValueType::STMT_NUM, "2")};
	output = facade.solveRight(RelationshipReference::FOLLOWS, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));
}

TEST_CASE("StmtToStmt: SolveLeft queries for Follows(1, 2) return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	FollowsTable* follows = (FollowsTable*)storage.getTable(TableName::FOLLOWS);
	
	Statement line1 = Statement(1, StatementType::ASSIGN);
	Statement line2 = Statement(2, StatementType::ASSIGN);
	Relationship<int, int> rs = Relationship(RelationshipReference::FOLLOWS, 1, 2);
	statements->store(&line1);
	statements->store(&line2);
	follows->store(&rs);

	Reference rightRef;
	EntityName leftEntityName;
	std::vector<Value> expectedResult;
	std::vector<Value> output;

	// SolveLeft(Follows, 2, Assign) for Follows(1, 2) returns {'1'}
	rightRef = Reference("2");
	leftEntityName = EntityName::ASSIGN;
	expectedResult = {Value(ValueType::STMT_NUM, "1")};
	output = facade.solveLeft(RelationshipReference::FOLLOWS, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Follows, 1, Assign) for Follows(1, 2) returns {}
	rightRef = Reference("1");
	leftEntityName = EntityName::ASSIGN;
	expectedResult = {};
	output = facade.solveLeft(RelationshipReference::FOLLOWS, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Follows, 2, Print) for Follows(1, 2) returns {}
	rightRef = Reference("2");
	leftEntityName = EntityName::PRINT;
	expectedResult = {};
	output = facade.solveLeft(RelationshipReference::FOLLOWS, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Follows, _, Assign) for Follows(1, 2) returns {'1'}
	rightRef = Reference("_");
	leftEntityName = EntityName::ASSIGN;
	expectedResult = {Value(ValueType::STMT_NUM, "1")};
	output = facade.solveLeft(RelationshipReference::FOLLOWS, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

}

TEST_CASE("StmtToStmt: SolveBoth queries for Follows(1, 2) return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	FollowsTable* follows = (FollowsTable*)storage.getTable(TableName::FOLLOWS);
	
	Statement line1 = Statement(1, StatementType::ASSIGN);
	Statement line2 = Statement(2, StatementType::ASSIGN);
	Relationship<int, int> rs = Relationship(RelationshipReference::FOLLOWS, 1, 2);
	statements->store(&line1);
	statements->store(&line2);
	follows->store(&rs);
	Value value1 = Value(ValueType::STMT_NUM, "1");
	Value value2 = Value(ValueType::STMT_NUM, "2");

	EntityName leftEntityName;
	EntityName rightEntityName;
	std::vector<std::pair<Value,Value>> expectedResult;
	std::vector<std::pair<Value,Value>> output;

	// SolveBoth(Follows, Stmt, Stmt) for Follows(1, 2) returns {('1', '2')}
	leftEntityName = EntityName::STMT;
	rightEntityName = EntityName::STMT;
	expectedResult = {std::make_pair(value1, value2)};
	output = facade.solveBoth(RelationshipReference::FOLLOWS, leftEntityName, rightEntityName);
	REQUIRE(expectedResult.size() == output.size());
	REQUIRE(expectedResult[0].first == output[0].first);
	REQUIRE(expectedResult[0].second == output[0].second);

	// SolveBoth(Follows, Stmt, Print) for Follows(1, 2) returns {}
	leftEntityName = EntityName::STMT;
	rightEntityName = EntityName::PRINT;
	expectedResult = {};
	output = facade.solveBoth(RelationshipReference::FOLLOWS, leftEntityName, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveBoth(Follows, Call, Stmt) for Follows(1, 2) returns {}
	leftEntityName = EntityName::CALL;
	rightEntityName = EntityName::STMT;
	expectedResult = {};
	output = facade.solveBoth(RelationshipReference::FOLLOWS, leftEntityName, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));
}

TEST_CASE("StmtToVar: Validate returns correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	ModifiesSTable* modifiesS = (ModifiesSTable*)storage.getTable(TableName::MODIFIES_S);
	
	Statement line1 = Statement(1, StatementType::ASSIGN);
	Variable var1 = Variable("a");
	Relationship<int, std::string> rs = Relationship(
		RelationshipReference::MODIFIES, 1, std::string("a")
	);
	statements->store(&line1);
	variables->store(&var1);
	modifiesS->store(&rs);

	Reference leftRef = Reference("1");
	Reference rightRef = Reference("a");
	Reference wildcardRef = Reference("_");

	// Queries where relationships do exist
	REQUIRE(facade.validate(RelationshipReference::MODIFIES, leftRef, rightRef));
	REQUIRE(facade.validate(RelationshipReference::MODIFIES, leftRef, wildcardRef));
	REQUIRE(facade.validate(RelationshipReference::MODIFIES, wildcardRef, rightRef));
	REQUIRE(facade.validate(RelationshipReference::MODIFIES, wildcardRef, wildcardRef));

	// Queries where relationships do not exist
	REQUIRE(!facade.validate(RelationshipReference::MODIFIES, Reference("2"), rightRef));
	REQUIRE(!facade.validate(RelationshipReference::MODIFIES, leftRef, Reference("x")));
	REQUIRE(!facade.validate(RelationshipReference::MODIFIES, Reference("2"), wildcardRef));
	REQUIRE(!facade.validate(RelationshipReference::MODIFIES, wildcardRef, Reference("x")));
}

TEST_CASE("StmtToVar: SolveRight queries for Modifies(1, 'a') return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	ModifiesSTable* modifiesS = (ModifiesSTable*)storage.getTable(TableName::MODIFIES_S);
	
	Statement line1 = Statement(1, StatementType::ASSIGN);
	Variable var1 = Variable("a");
	Relationship<int, std::string> rs = Relationship(
		RelationshipReference::MODIFIES, 1, std::string("a")
	);
	statements->store(&line1);
	variables->store(&var1);
	modifiesS->store(&rs);

	Reference leftRef;
	EntityName rightEntityName;
	std::vector<Value> expectedResult;
	std::vector<Value> output;
	
	// SolveRight(Modifies, 1, Variable) for Modifies(1, 'a') returns {'a'}
	leftRef = Reference("1");
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {Value(ValueType::VAR_NAME, "a")};
	output = facade.solveRight(RelationshipReference::MODIFIES, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Modifies, 2, Variable) for Modifies(1, 'a') returns {}
	leftRef = Reference("2");
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {};
	output = facade.solveRight(RelationshipReference::MODIFIES, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Modifies, 1, Procedure) for Modifies(1, 'a') returns {}
	leftRef = Reference("1");
	rightEntityName = EntityName::PROCEDURE;
	expectedResult = {};
	output = facade.solveRight(RelationshipReference::MODIFIES, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Modifies, _, Variable) for Modifies(1, 'a') returns {'a'}
	leftRef = Reference("_");
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {Value(ValueType::VAR_NAME, "a")};
	output = facade.solveRight(RelationshipReference::MODIFIES, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));
}

TEST_CASE("StmtToVar: SolveLeft queries for Modifies(1, 'a') return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	ModifiesSTable* modifiesS = (ModifiesSTable*)storage.getTable(TableName::MODIFIES_S);
	
	Statement line1 = Statement(1, StatementType::ASSIGN);
	Variable var1 = Variable("a");
	Relationship<int, std::string> rs = Relationship(
		RelationshipReference::MODIFIES, 1, std::string("a")
	);
	statements->store(&line1);
	variables->store(&var1);
	modifiesS->store(&rs);

	Reference rightRef;
	EntityName leftEntityName;
	std::vector<Value> expectedResult;
	std::vector<Value> output;

	// SolveLeft(Modifies, a, Assign) for Modifies(1, 'a') returns {'1'}
	rightRef = Reference("a");
	leftEntityName = EntityName::ASSIGN;
	expectedResult = {Value(ValueType::STMT_NUM, "1")};
	output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Modifies, x, Assign) for Modifies(1, 'a') returns {}
	rightRef = Reference("x");
	leftEntityName = EntityName::ASSIGN;
	expectedResult = {};
	output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Modifies, a, Print) for Modifies(1, 'a') returns {}
	rightRef = Reference("a");
	leftEntityName = EntityName::PRINT;
	expectedResult = {};
	output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Modifies, _, Assign) for Modifies(1, 'a') returns {'1'}
	rightRef = Reference("_");
	leftEntityName = EntityName::ASSIGN;
	expectedResult = {Value(ValueType::STMT_NUM, "1")};
	output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

}

TEST_CASE("StmtToVar: SolveBoth queries for Modifies(1, 'a') return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	ModifiesSTable* modifiesS = (ModifiesSTable*)storage.getTable(TableName::MODIFIES_S);
	
	Statement line1 = Statement(1, StatementType::ASSIGN);
	Variable var1 = Variable("a");
	Relationship<int, std::string> rs = Relationship(
		RelationshipReference::MODIFIES, 1, std::string("a")
	);
	statements->store(&line1);
	variables->store(&var1);
	modifiesS->store(&rs);
	Value value1 = Value(ValueType::STMT_NUM, "1");
	Value value2 = Value(ValueType::VAR_NAME, "a");

	EntityName leftEntityName;
	EntityName rightEntityName;
	std::vector<std::pair<Value,Value>> expectedResult;
	std::vector<std::pair<Value,Value>> output;

	// SolveBoth(Modifies, Stmt, Var) for Modifies(1, 'a') returns {('1', 'a')}
	leftEntityName = EntityName::STMT;
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {std::make_pair(value1, value2)};
	output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName, rightEntityName);
	REQUIRE(expectedResult.size() == output.size());
	REQUIRE(expectedResult[0].first == output[0].first);
	REQUIRE(expectedResult[0].second == output[0].second);

	// SolveBoth(Modifies, Stmt, Print) for Modifies(1, 'a') returns {}
	leftEntityName = EntityName::STMT;
	rightEntityName = EntityName::PRINT;
	expectedResult = {};
	output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveBoth(Modifies, Call, Var) for Modifies(1, 'a') returns {}
	leftEntityName = EntityName::CALL;
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {};
	output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));
}

TEST_CASE("ProcToVar: Validate returns correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	ModifiesPTable* modifiesP = (ModifiesPTable*)storage.getTable(TableName::MODIFIES_P);
	
	Procedure proc1 = Procedure("Main");
	Variable var1 = Variable("a");
	Relationship<std::string, std::string> rs = Relationship(
		RelationshipReference::MODIFIES, std::string("Main"), std::string("a")
	);
	procedures->store(&proc1);
	variables->store(&var1);
	modifiesP->store(&rs);

	Reference leftRef = Reference("Main");
	Reference rightRef = Reference("a");
	Reference wildcardRef = Reference("_");

	// Queries where relationships do exist
	REQUIRE(facade.validate(RelationshipReference::MODIFIES, leftRef, rightRef));
	REQUIRE(facade.validate(RelationshipReference::MODIFIES, leftRef, wildcardRef));
	REQUIRE(facade.validate(RelationshipReference::MODIFIES, wildcardRef, rightRef));
	REQUIRE(facade.validate(RelationshipReference::MODIFIES, wildcardRef, wildcardRef));

	// Queries where relationships do not exist
	REQUIRE(!facade.validate(RelationshipReference::MODIFIES, Reference("Foo"), rightRef));
	REQUIRE(!facade.validate(RelationshipReference::MODIFIES, leftRef, Reference("x")));
	REQUIRE(!facade.validate(RelationshipReference::MODIFIES, Reference("Foo"), wildcardRef));
	REQUIRE(!facade.validate(RelationshipReference::MODIFIES, wildcardRef, Reference("x")));
}

TEST_CASE("ProcToVar: SolveRight queries for Modifies('Main', 'a') return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	ModifiesPTable* modifiesP = (ModifiesPTable*)storage.getTable(TableName::MODIFIES_P);
	
	Procedure proc1 = Procedure("Main");
	Variable var1 = Variable("a");
	Relationship<std::string, std::string> rs = Relationship(
		RelationshipReference::MODIFIES, std::string("Main"), std::string("a")
	);
	procedures->store(&proc1);
	variables->store(&var1);
	modifiesP->store(&rs);

	Reference leftRef;
	EntityName rightEntityName;
	std::vector<Value> expectedResult;
	std::vector<Value> output;
	
	// SolveRight(Modifies, Main, Variable) for Modifies('Main', 'a') returns {'a'}
	leftRef = Reference("Main");
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {Value(ValueType::VAR_NAME, "a")};
	output = facade.solveRight(RelationshipReference::MODIFIES, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Modifies, Foo, Variable) for Modifies('Main', 'a') returns {}
	leftRef = Reference("Foo");
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {};
	output = facade.solveRight(RelationshipReference::MODIFIES, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Modifies, Main, Procedure) for Modifies('Main', 'a') returns {}
	leftRef = Reference("Main");
	rightEntityName = EntityName::PROCEDURE;
	expectedResult = {};
	output = facade.solveRight(RelationshipReference::MODIFIES, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveRight(Modifies, _, Variable) for Modifies('Main', 'a') returns {'a'}
	leftRef = Reference("_");
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {Value(ValueType::VAR_NAME, "a")};
	output = facade.solveRight(RelationshipReference::MODIFIES, leftRef, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));
}

TEST_CASE("ProcToVar: SolveLeft queries for Modifies('Main', 'a') return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	ModifiesPTable* modifiesP = (ModifiesPTable*)storage.getTable(TableName::MODIFIES_P);
	
	Procedure proc1 = Procedure("Main");
	Variable var1 = Variable("a");
	Relationship<std::string, std::string> rs = Relationship(
		RelationshipReference::MODIFIES, std::string("Main"), std::string("a")
	);
	procedures->store(&proc1);
	variables->store(&var1);
	modifiesP->store(&rs);

	Reference rightRef;
	EntityName leftEntityName;
	std::vector<Value> expectedResult;
	std::vector<Value> output;

	// SolveLeft(Modifies, a, Procedure) for Modifies('Main', 'a') returns {'Main'}
	rightRef = Reference("a");
	leftEntityName = EntityName::PROCEDURE;
	expectedResult = {Value(ValueType::VAR_NAME, "Main")};
	output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Modifies, x, Procedure) for Modifies('Main', 'a') returns {}
	rightRef = Reference("x");
	leftEntityName = EntityName::PROCEDURE;
	expectedResult = {};
	output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Modifies, a, Statement) for Modifies('Main', 'a') returns {}
	rightRef = Reference("a");
	leftEntityName = EntityName::STMT;
	expectedResult = {};
	output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveLeft(Modifies, _, Procedure) for Modifies('Main', 'a') returns {'Main'}
	rightRef = Reference("_");
	leftEntityName = EntityName::PROCEDURE;
	expectedResult = {Value(ValueType::VAR_NAME, "Main")};
	output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef, leftEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

}

TEST_CASE("ProcToVar: SolveBoth queries for Modifies('Main', 'a') return correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	ModifiesPTable* modifiesP = (ModifiesPTable*)storage.getTable(TableName::MODIFIES_P);
	
	Procedure proc1 = Procedure("Main");
	Variable var1 = Variable("a");
	Relationship<std::string, std::string> rs = Relationship(
		RelationshipReference::MODIFIES, std::string("Main"), std::string("a")
	);
	procedures->store(&proc1);
	variables->store(&var1);
	modifiesP->store(&rs);
	Value value1 = Value(ValueType::VAR_NAME, "Main");
	Value value2 = Value(ValueType::VAR_NAME, "a");

	EntityName leftEntityName;
	EntityName rightEntityName;
	std::vector<std::pair<Value,Value>> expectedResult;
	std::vector<std::pair<Value,Value>> output;

	// SolveBoth(Modifies, Proc, Var) for Modifies('Main', 'a') returns {('1', '2')}
	leftEntityName = EntityName::PROCEDURE;
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {std::make_pair(value1, value2)};
	output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName, rightEntityName);
	REQUIRE(expectedResult.size() == output.size());
	REQUIRE(expectedResult[0].first == output[0].first);
	REQUIRE(expectedResult[0].second == output[0].second);

	// SolveBoth(Modifies, Proc, Print) for Modifies('Main', 'a') returns {}
	leftEntityName = EntityName::PROCEDURE;
	rightEntityName = EntityName::PRINT;
	expectedResult = {};
	output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// SolveBoth(Modifies, Call, Var) for Modifies('Main', 'a') returns {}
	leftEntityName = EntityName::CALL;
	rightEntityName = EntityName::VARIABLE;
	expectedResult = {};
	output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName, rightEntityName);
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));
}

TEST_CASE("getAssign returns correct results") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	AssignmentsTable* assignments = (AssignmentsTable*)storage.getTable(TableName::ASSIGNMENTS);
	
	Assignment test1 = Assignment(1, "x1", "(1)");
	Assignment test2 = Assignment(2, "x1", "((1)+(10))");
	Assignment test3 = Assignment(3, "x2", "((x1)*(x1))");
	assignments->store(&test1);
	assignments->store(&test2);
	assignments->store(&test3);

	std::vector<Value> expectedResult;
	std::vector<Value> output;
	
	// getAssign('_', '_') returns {'1', '2', '3'}
	expectedResult = {
		Value(ValueType::STMT_NUM, "1"),
		Value(ValueType::STMT_NUM, "2"),
		Value(ValueType::STMT_NUM, "3")};
	output = facade.getAssign("_", "_");
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// getAssign('_', '1') returns {'1', '2'}
	expectedResult = {Value(ValueType::STMT_NUM, "1"), Value(ValueType::STMT_NUM, "2")};
	output = facade.getAssign("_", "1");
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// getAssign('x1', '_') returns {'1', '2'}
	expectedResult = {Value(ValueType::STMT_NUM, "1"), Value(ValueType::STMT_NUM, "2")};
	output = facade.getAssign("x1", "_");
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));

	// getAssign('x1', '10') returns {'1', '2', '3'}
	expectedResult = {Value(ValueType::STMT_NUM, "2")};
	output = facade.getAssign("x1", "10");
	REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(), output.begin()));
}