#include "PKB/QueryFacade.h"
#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "QPS/Evaluator/SuchThatEvaluator.h"
#include "QPS/Evaluator/PatternEvaluator.h"
#include "QPS/Evaluator/QueryResult.h"
#include "QPS/Evaluator/ClauseResult.h"

#include "catch.hpp"

TEST_CASE("Query evaluator can evaluate query with only select statement clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable* statements = (StatementsTable*)storage.getTable(TableName::STATEMENTS);
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("Statement s; Select s;");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result = queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}


TEST_CASE("Query evaluator can evaluate query with only select variable clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    VariablesTable* variables = (VariablesTable*)storage.getTable(TableName::VARIABLES);
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("Variable v; Select v;");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result = queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE("Query evaluator can evaluate query with only select constant clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    ConstantsTable* constants = (ConstantsTable*)storage.getTable(TableName::CONSTANTS);
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    constants->store(&test1);
    constants->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("Constant c; Select c;");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result = queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE("Query evaluator can evaluate query with only select procedure clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    ProceduresTable* procedures = (ProceduresTable*)storage.getTable(TableName::PROCEDURES);
    Procedure test1 = Procedure("test1");
    Procedure test2 = Procedure("test2");

    procedures->store(&test1);
    procedures->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("Procedure p; Select p;");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result = queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}
