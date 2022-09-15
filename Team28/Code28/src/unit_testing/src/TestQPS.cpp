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

	std::string input = "stmt s; Select s";
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

	std::string input = "variable v; Select v";
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

	std::string input = "constant c; Select c";
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

	std::string input = "procedure p; Select p";
	std::string output = qps.processQuery(input);
	REQUIRE(output == "test1, test2");
}

TEST_CASE("QPS can process simple queries to select procedures when there are no procedures") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);

	QPS qps = QPS(&facade);

	std::string input = "procedure p; Select p";
	std::string output = qps.processQuery(input);
	REQUIRE(output.size() == 0);
}

TEST_CASE("QPS can process queries with select syntax error") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	QPS qps = QPS(&facade);

	std::string misspelled = "variable v; Selct v such that Modifies(1, v)";
	REQUIRE_THROWS(qps.processQuery(misspelled));

	std::string missing_synonym = "variable v; Select such that Modifies(1, v)";
	REQUIRE_THROWS(qps.processQuery(missing_synonym));
}

TEST_CASE("QPS can process queries with such that syntax error") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	QPS qps = QPS(&facade);

	std::string extra_bracket = "variable v; Select v such that Modifies((1, v)";
	REQUIRE_THROWS(qps.processQuery(extra_bracket));

	std::string missing_select = "variable v; such that Modifies((1, v)";
	REQUIRE_THROWS(qps.processQuery(missing_select));

	std::string missing_bracket = "variable v; Select v such that Modifies(1, v";
	REQUIRE_THROWS(qps.processQuery(missing_bracket));

	std::string misspelled = "variable v; Select v suxh that Modifies(1, v)";
	REQUIRE_THROWS(qps.processQuery(misspelled));

	std::string too_many_arguments = "variable v; Select v suxh that Modifies(1, v, d)";
	REQUIRE_THROWS(qps.processQuery(too_many_arguments));

	std::string too_few_arguments = "variable v; Select v suxh that Modifies(1)";
	REQUIRE_THROWS(qps.processQuery(too_few_arguments));

	std::string missing_relationship_name = "variable v; Select v suxh that (1, v)";
	REQUIRE_THROWS(qps.processQuery(missing_relationship_name));
}

TEST_CASE("QPS can process queries with pattern syntax error") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	QPS qps = QPS(&facade);

	std::string misspelled = "variable v; assign a; Select v pattrn a(v, _)";
	REQUIRE_THROWS(qps.processQuery(misspelled));

	std::string too_many_arguments = "variable v; assign a; Select v such that Modifies(1, 2) pattern a(v, _, c)";
	REQUIRE_THROWS(qps.processQuery(too_many_arguments));

	std::string too_few_arguments = "variable v; assign a; Select v such that Modifies(1, 2) pattern a(c)";
	REQUIRE_THROWS(qps.processQuery(too_few_arguments));
}

TEST_CASE("QPS can process simple queries with semantic error") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	QPS qps = QPS(&facade);

	std::string undeclared_synonym = "variable v; Select v such that Modifies(1, yey)";
	REQUIRE_THROWS(qps.processQuery(undeclared_synonym));
}

TEST_CASE("QPS evaluate select statements") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
	Statement test1 = Statement(1, StatementType::ASSIGN);
	Statement test2 = Statement(2, StatementType::ASSIGN);

	statements->store(&test1);
	statements->store(&test2);

	QPS qps = QPS(&facade);

	std::string input = "stmt s; Select s";
	std::list<std::string> results;
	qps.evaluate(input, results);
	std::list<std::string> correct_output{ "1", "2" };
	REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate select variables") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	QPS qps = QPS(&facade);

	std::string input = "variable v; Select v";
	std::list<std::string> results;
	qps.evaluate(input, results);
	std::list<std::string> correct_output{ "test1", "test2" };
	REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate select constants") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ConstantsTable* constants = (ConstantsTable*)storage.getTable(TableName::CONSTANTS);
	Constant test1 = Constant("test1");
	Constant test2 = Constant("test2");

	constants->store(&test1);
	constants->store(&test2);

	QPS qps = QPS(&facade);

	std::string input = "constant c; Select c";
	std::list<std::string> results;
	qps.evaluate(input, results);
	std::list<std::string> correct_output{ "test1", "test2" };
	REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate select procedures") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
	Procedure test1 = Procedure("test1");
	Procedure test2 = Procedure("test2");

	procedures->store(&test1);
	procedures->store(&test2);

	QPS qps = QPS(&facade);

	std::string input = "procedure p; Select p";
	std::list<std::string> results;
	qps.evaluate(input, results);
	std::list<std::string> correct_output{ "test1", "test2" };
	REQUIRE(results == correct_output);
}

TEST_CASE("QPS can evaluate select procedures when there are no procedures") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);

	QPS qps = QPS(&facade);

	std::string input = "procedure p; Select p";
	std::list<std::string> results;
	qps.evaluate(input, results);
	REQUIRE(results.size() == 0);
}

TEST_CASE("QPS evaluate syntax error") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	QPS qps = QPS(&facade);

	std::string extra_bracket = "variable v; Select v such that Modifies((1, v)";
	std::list<std::string> results;
	REQUIRE_THROWS(qps.evaluate(extra_bracket, results));
}

TEST_CASE("QPS evaluate semantic error") {
	Storage storage;
	QueryFacade facade = QueryFacade(&storage);
	VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
	Variable test1 = Variable("test1");
	Variable test2 = Variable("test2");

	variables->store(&test1);
	variables->store(&test2);

	QPS qps = QPS(&facade);

	std::string undeclared_synonym = "variable v; Select v such that Modifies(1, yey)";
	std::list<std::string> results;
	REQUIRE_THROWS(qps.evaluate(undeclared_synonym, results));
}

TEST_CASE("QPS can process queries with such that clause") {
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

	QPS qps = QPS(&facade);

	std::string input = "stmt s; Select s such that Follows(s, 2)";
	std::list<std::string> results;
	qps.evaluate(input, results);
	std::list<std::string> correct_output{ "1" };
	REQUIRE(results == correct_output);
	REQUIRE(results == correct_output);
}