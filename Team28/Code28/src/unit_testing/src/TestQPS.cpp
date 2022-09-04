#include "PKB/QueryFacade.h"
#include "QPS/QPS.h"

#include "catch.hpp"

TEST_CASE("QPS can process simple queries to select statements") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	Statement test1 = Statement(1, StatementType::ASSIGN);
	Statement test2 = Statement(2, StatementType::ASSIGN);

	statements->store(&test1);
	statements->store(&test2);

	QPS qps = QPS(&facade);

	std::string input = "Statement s; Select s;";
	std::string output = qps.processQuery(input);
	REQUIRE(output == "1, 2");
}

TEST_CASE("QPS can process simple queries to select variables") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	QPS qps = QPS(&facade);

	std::string input = "Variable v; Select v;";
	std::string output = qps.processQuery(input);
	REQUIRE(output == "test1, test2");
}

TEST_CASE("QPS can process simple queries to select constants") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ConstantsTable* constants = (ConstantsTable*)storage.getTable(TableName::CONSTANTS);
	Constant test1 = Constant("test1");
	Constant test2 = Constant("test2");

	constants->store(&test1);
	constants->store(&test2);

	QPS qps = QPS(&facade);

	std::string input = "Constant c; Select c;";
	std::string output = qps.processQuery(input);
	REQUIRE(output == "test1, test2");
}

TEST_CASE("QPS can process simple queries to select procedures") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
	Procedure test1 = Procedure("test1");
	Procedure test2 = Procedure("test2");

	procedures->store(&test1);
	procedures->store(&test2);

	QPS qps = QPS(&facade);

	std::string input = "Procedure p; Select p;";
	std::string output = qps.processQuery(input);
	REQUIRE(output == "test1, test2");
}

TEST_CASE("QPS can process simple queries to select procedures when got no procedures") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);

	QPS qps = QPS(&facade);

	std::string input = "Procedure p; Select p;";
	std::string output = qps.processQuery(input);
	REQUIRE(output == "None");
}