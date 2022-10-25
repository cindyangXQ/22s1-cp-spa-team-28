#include "PKB/Facades/QueryFacade.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"

#include "catch.hpp"

TEST_CASE(
    "Query evaluator can evaluate query with only select statement clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("stmt s; Select s");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE(
    "Query evaluator can evaluate query with only select variable clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    VariablesTable *variables = storage->getTable<VariablesTable>();
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("variable v; Select v");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE(
    "Query evaluator can evaluate query with only select constant clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ConstantsTable *constants = storage->getTable<ConstantsTable>();
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    constants->store(&test1);
    constants->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("constant c; Select c");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE(
    "Query evaluator can evaluate query with only select procedure clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    Procedure test1 = Procedure("test1", 1);
    Procedure test2 = Procedure("test2", 2);

    procedures->store(&test1);
    procedures->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("procedure p; Select p");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE("Evaluator can retrieve specific statement types") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
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

    std::string input;
    std::vector<std::string> result;
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);

    input = "stmt s; Select s";
    SolvableQuery solvableQStmt = QueryParser::parse(input);
    QueryResult queryResultStmt = queryEvaluator.evaluate(&solvableQStmt);
    result = queryEvaluator.interpretQueryResult(&queryResultStmt);

    REQUIRE(result.size() == 5);

    input = "if s; Select s";
    SolvableQuery solvableQIf = QueryParser::parse(input);
    QueryResult queryResultIf = queryEvaluator.evaluate(&solvableQIf);
    result = queryEvaluator.interpretQueryResult(&queryResultIf);

    REQUIRE(result.size() == 1);

    input = "assign s; Select s";
    SolvableQuery solvableQAssign = QueryParser::parse(input);
    QueryResult queryResultAssign = queryEvaluator.evaluate(&solvableQAssign);
    result = queryEvaluator.interpretQueryResult(&queryResultAssign);

    REQUIRE(result.size() == 2);

    input = "while s; Select s";
    SolvableQuery solvableQWhile = QueryParser::parse(input);
    QueryResult queryResultWhile = queryEvaluator.evaluate(&solvableQWhile);
    result = queryEvaluator.interpretQueryResult(&queryResultWhile);

    REQUIRE(result.size() == 2);
}

TEST_CASE("Query evaluator can evaluate query with single such that clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");

    statements->store(&line1);
    statements->store(&line2);
    follows->store(&rs);

    SolvableQuery solvableQ =
        QueryParser::parse("stmt s; Select s such that Follows(1,2)");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}
TEST_CASE("Query evaluator can evaluate query with single such that clause "
          "that is false") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");

    statements->store(&line1);
    statements->store(&line2);
    follows->store(&rs);

    SolvableQuery solvableQ =
        QueryParser::parse("stmt s; Select s such that Follows(2,1)");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 0);
}

TEST_CASE("Query evaluator can evaluate query with single such that clause "
          "with synonym") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");

    statements->store(&line1);
    statements->store(&line2);
    follows->store(&rs);

    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);

    SolvableQuery solvableQRight =
        QueryParser::parse("stmt s; Select s such that Follows(1,s)");
    QueryResult queryResultRight = queryEvaluator.evaluate(&solvableQRight);
    std::vector<std::string> result_right =
        queryEvaluator.interpretQueryResult(&queryResultRight);

    REQUIRE(result_right.size() == 1);

    SolvableQuery solvableQLeft =
        QueryParser::parse("stmt s; Select s such that Follows(s,2)");
    QueryResult queryResultLeft = queryEvaluator.evaluate(&solvableQLeft);
    std::vector<std::string> resultLeft =
        queryEvaluator.interpretQueryResult(&queryResultLeft);

    REQUIRE(resultLeft.size() == 1);

    SolvableQuery solvableQWildcard =
        QueryParser::parse("stmt s; Select s such that Follows(_, s)");
    QueryResult queryResultWildcard =
        queryEvaluator.evaluate(&solvableQWildcard);
    std::vector<std::string> resultWildcard =
        queryEvaluator.interpretQueryResult(&queryResultWildcard);

    REQUIRE(resultWildcard.size() == 1);

    SolvableQuery solvableQWildcardRight =
        QueryParser::parse("stmt s; Select s such that Follows(s, _)");
    QueryResult queryResultWildcardRight =
        queryEvaluator.evaluate(&solvableQWildcardRight);
    std::vector<std::string> resultWildcardRight =
        queryEvaluator.interpretQueryResult(&queryResultWildcardRight);

    REQUIRE(resultWildcardRight.size() == 1);

    SolvableQuery solvableQZero =
        QueryParser::parse("stmt s; Select s such that Follows(s,1)");
    QueryResult queryResultZero = queryEvaluator.evaluate(&solvableQZero);
    std::vector<std::string> resultZero =
        queryEvaluator.interpretQueryResult(&queryResultZero);

    REQUIRE(resultZero.size() == 0);
}

TEST_CASE("Query evaluator can evaluate query with single such that clause "
          "with synonyms that are the same") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);

    SolvableQuery solvableQFollow =
        QueryParser::parse("stmt s; Select s such that Follows(s, s)");
    QueryResult queryResultFollow = queryEvaluator.evaluate(&solvableQFollow);
    std::vector<std::string> resultFollow =
        queryEvaluator.interpretQueryResult(&queryResultFollow);

    REQUIRE(resultFollow.size() == 0);

    SolvableQuery solvableQFollowT =
        QueryParser::parse("stmt s; Select s such that Follows*(s, s)");
    QueryResult queryResultFollowT =
        queryEvaluator.evaluate(&solvableQFollowT);
    std::vector<std::string> resultFollowT =
        queryEvaluator.interpretQueryResult(&queryResultFollowT);

    REQUIRE(resultFollowT.size() == 0);

    SolvableQuery solvableQParent =
        QueryParser::parse("stmt s; Select s such that Parent*(s, s)");
    QueryResult queryResultParent = queryEvaluator.evaluate(&solvableQParent);
    std::vector<std::string> resultParent =
        queryEvaluator.interpretQueryResult(&queryResultParent);

    REQUIRE(resultParent.size() == 0);

    SolvableQuery solvableQParentT =
        QueryParser::parse("stmt s; Select s such that Parent*(s, s)");
    QueryResult queryResultParentT =
        queryEvaluator.evaluate(&solvableQParentT);
    std::vector<std::string> resultParentT =
        queryEvaluator.interpretQueryResult(&queryResultParentT);

    REQUIRE(resultParentT.size() == 0);
}

TEST_CASE("Select alternate attribute with no clauses") { 

    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    UsesSTable *usesS = storage->getTable<UsesSTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();
    CallProcTable *callP = storage->getTable<CallProcTable>();

    // Statements
    Statement stmt1 = Statement(1, StatementType::ASSIGN);
    Statement stmt2 = Statement(2, StatementType::PRINT);
    Statement stmt3 = Statement(3, StatementType::READ);
    Statement stmt4 = Statement(4, StatementType::CALL);
    statements->store(&stmt1);
    statements->store(&stmt2);
    statements->store(&stmt3);
    statements->store(&stmt4);

    // Relationships
    Relationship<int, std::string> rs1 =
        Relationship(RelationshipReference::USES, 1, std::string("b"));
    Relationship<int, std::string> rs2 =
        Relationship(RelationshipReference::USES, 2, std::string("a"));
    Relationship<int, std::string> rs3 =
        Relationship(RelationshipReference::MODIFIES, 3, std::string("x"));
    Relationship<int, std::string> rs4 =
        Relationship(RelationshipReference::USES, 4, std::string("bar"));
    usesS->store(&rs1);
    usesS->store(&rs2);
    modifiesS->store(&rs3);
    callP->store(&rs4);

    std::vector<std::string> result;

    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    Synonym r = Synonym(EntityName::READ, "r");
    Reference rVarName = Reference(r, EntityAttribute::VAR_NAME);
    SelectClause selectClR = SelectClause({rVarName}, SelectType::SINGLE);
    QueryResult queryResultR = QueryResult(selectClR, {});
    result = queryEvaluator.interpretQueryResult(&queryResultR);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == "x");

    Synonym p = Synonym(EntityName::PRINT, "p");
    Reference pVarName = Reference(p, EntityAttribute::VAR_NAME);
    SelectClause selectClP = SelectClause({pVarName}, SelectType::SINGLE);
    QueryResult queryResultP = QueryResult(selectClP, {});
    result = queryEvaluator.interpretQueryResult(&queryResultP);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == "a");

    Synonym c = Synonym(EntityName::CALL, "c");
    Reference cProcName = Reference(c, EntityAttribute::PROC_NAME);
    SelectClause selectClC = SelectClause({cProcName}, SelectType::SINGLE);
    QueryResult queryResultC = QueryResult(selectClC, {});
    result = queryEvaluator.interpretQueryResult(&queryResultC);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == "bar");
}