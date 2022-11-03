#include "PKB/Facades/QueryFacade.h"

#include "catch.hpp"

struct InitAffectsStorage {
public:
    static QueryFacade createSimple();
};

QueryFacade InitAffectsStorage::createSimple() {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    AffectsTable *affects = storage->getTable<AffectsTable>();
    AffectsTTable *affectsT = storage->getTable<AffectsTTable>();
    NextTable *next = storage->getTable<NextTable>();
    NextTTable *nextT = storage->getTable<NextTTable>();
    UsesSTable *usesS = storage->getTable<UsesSTable>();
    ModifiesSTable *modS = storage->getTable<ModifiesSTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();

    Relationship<int, int> nextRs =
        Relationship(RelationshipReference::NEXT, 1, 2);
    Relationship<int, int> nextTRs =
        Relationship(RelationshipReference::NEXT_T, 1, 2);
    Relationship<int, std::string> usesS2 =
        Relationship(RelationshipReference::USES, 2, std::string("x"));
    Relationship<int, std::string> modS1 =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("x"));
    Relationship<int, std::string> modS2 =
        Relationship(RelationshipReference::MODIFIES, 2, std::string("x"));
    Statement s1 = Statement(1, StatementType::ASSIGN);
    Statement s2 = Statement(2, StatementType::ASSIGN);

    next->store(&nextRs);
    nextT->store(&nextTRs);
    usesS->store(&usesS2);
    modS->store(&modS1);
    modS->store(&modS2);
    statements->store(&s1);
    statements->store(&s2);
    affects->initAffects(storage->getStorageView());
    affectsT->initAffectsT(storage->getStorageView());

    return facade;
}

TEST_CASE("getAllStatements returns all statements correctly") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    // returned number of statements is equal to number stored
    REQUIRE(facade.getAllStatementsByType(StatementType::STMT).size() == 2);
    // first statement is test1
    REQUIRE(*facade.getAllStatementsByType(StatementType::STMT).at(0) == test1);
    // second statement is test2
    REQUIRE(*facade.getAllStatementsByType(StatementType::STMT).at(1) == test2);
}

TEST_CASE("getAllStatementByType returns all While statements correctly") {
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

    // returned number of statements is equal to number stored
    REQUIRE(facade.getAllStatementsByType(StatementType::WHILE).size() == 2);
    // first statement is test1
    REQUIRE(*facade.getAllStatementsByType(StatementType::WHILE).at(0) ==
            test3);
    // second statement is test2
    REQUIRE(*facade.getAllStatementsByType(StatementType::WHILE).at(1) ==
            test5);
}

TEST_CASE("getAllEntities(Variables) returns all variables correctly") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    VariablesTable *variables = storage->getTable<VariablesTable>();
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    // returned number of variables is equal to number stored
    REQUIRE(facade.getAllEntities(Designation::VAR).size() == 2);
}

TEST_CASE("getAllEntities(Constants) returns all constants correctly") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ConstantsTable *constants = storage->getTable<ConstantsTable>();
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    constants->store(&test1);
    constants->store(&test2);

    // returned number of variables is equal to number stored
    REQUIRE(facade.getAllEntities(Designation::CONST).size() == 2);
}

TEST_CASE("getAllEntities(Procedures) returns all constants correctly") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    Procedure test1 = Procedure("test1", 1);
    Procedure test2 = Procedure("test2", 2);

    procedures->store(&test1);
    procedures->store(&test2);

    // returned number of procedures is equal to number stored
    REQUIRE(facade.getAllEntities(Designation::PROC).size() == 2);
}

TEST_CASE("getAllEntities(Non-Entities) returns nothing") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    Statement test1 = Statement(1, StatementType::ASSIGN);
    statements->store(&test1);

    // returned number of procedures is equal to number stored
    REQUIRE(facade.getAllEntities(Designation::STMT).size() == 0);
}

TEST_CASE("StmtToStmt: Validate returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    statements->store(&line1);
    statements->store(&line2);
    follows->store(&rs);

    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");
    Reference wildcardRef = Reference("_");

    // Queries where relationships do exist
    REQUIRE(facade.validate(RelationshipReference::FOLLOWS, leftRef, rightRef));
    REQUIRE(
        facade.validate(RelationshipReference::FOLLOWS, leftRef, wildcardRef));
    REQUIRE(
        facade.validate(RelationshipReference::FOLLOWS, wildcardRef, rightRef));
    REQUIRE(facade.validate(RelationshipReference::FOLLOWS, wildcardRef,
                            wildcardRef));

    // Queries where relationships do not exist
    REQUIRE(
        !facade.validate(RelationshipReference::FOLLOWS, rightRef, rightRef));
    REQUIRE(!facade.validate(RelationshipReference::FOLLOWS, leftRef, leftRef));
    REQUIRE(!facade.validate(RelationshipReference::FOLLOWS, rightRef,
                             wildcardRef));
    REQUIRE(
        !facade.validate(RelationshipReference::FOLLOWS, wildcardRef, leftRef));
}

TEST_CASE(
    "StmtToStmt: SolveRight queries for Follows(1, 2) return correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
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
    output = facade.solveRight(RelationshipReference::FOLLOWS, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Follows, 2, Assign) returns {}
    leftRef = Reference("2");
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::FOLLOWS, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Follows, 1, Print) returns {}
    leftRef = Reference("1");
    rightEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::FOLLOWS, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Follows, _, Assign) returns {'2'}
    leftRef = Reference("_");
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = facade.solveRight(RelationshipReference::FOLLOWS, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE(
    "StmtToStmt: SolveLeft queries for Follows(1, 2) return correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
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
    output = facade.solveLeft(RelationshipReference::FOLLOWS, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Follows, 1, Assign) for Follows(1, 2) returns {}
    rightRef = Reference("1");
    leftEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::FOLLOWS, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Follows, 2, Print) for Follows(1, 2) returns {}
    rightRef = Reference("2");
    leftEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::FOLLOWS, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Follows, _, Assign) for Follows(1, 2) returns {'1'}
    rightRef = Reference("_");
    leftEntityName = EntityName::ASSIGN;
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = facade.solveLeft(RelationshipReference::FOLLOWS, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE(
    "StmtToStmt: SolveBoth queries for Follows(1, 2) return correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    statements->store(&line1);
    statements->store(&line2);
    follows->store(&rs);
    Value value1 = Value(ValueType::STMT_NUM, "1");
    Value value2 = Value(ValueType::STMT_NUM, "2");

    EntityName leftEntityName;
    EntityName rightEntityName;
    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // SolveBoth(Follows, Stmt, Stmt) for Follows(1, 2) returns {('1', '2')}
    leftEntityName = EntityName::STMT;
    rightEntityName = EntityName::STMT;
    expectedResult = {std::make_pair(value1, value2)};
    output = facade.solveBoth(RelationshipReference::FOLLOWS, leftEntityName,
                              rightEntityName);
    REQUIRE(expectedResult.size() == output.size());
    REQUIRE(expectedResult[0].first == output[0].first);
    REQUIRE(expectedResult[0].second == output[0].second);

    // SolveBoth(Follows, Stmt, Print) for Follows(1, 2) returns {}
    leftEntityName = EntityName::STMT;
    rightEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::FOLLOWS, leftEntityName,
                              rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveBoth(Follows, Call, Stmt) for Follows(1, 2) returns {}
    leftEntityName = EntityName::CALL;
    rightEntityName = EntityName::STMT;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::FOLLOWS, leftEntityName,
                              rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("StmtToStmt: No duplicate results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTTable *follows = storage->getTable<FollowsTTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Statement line3 = Statement(3, StatementType::ASSIGN);
    Relationship<int, int> rs1 =
        Relationship(RelationshipReference::FOLLOWS_T, 1, 2);
    Relationship<int, int> rs2 =
        Relationship(RelationshipReference::FOLLOWS_T, 2, 3);
    Relationship<int, int> rs3 =
        Relationship(RelationshipReference::FOLLOWS_T, 1, 3);
    statements->store(&line1);
    statements->store(&line2);
    statements->store(&line3);
    follows->store(&rs1);
    follows->store(&rs2);
    follows->store(&rs3);
    Value value1 = Value(ValueType::STMT_NUM, "1");
    Value value2 = Value(ValueType::STMT_NUM, "2");
    Value value3 = Value(ValueType::STMT_NUM, "3");

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveRight(Follows*, _, Statement) returns {'2', '3'} not {'2', '3', '3'}
    Reference leftRef = Reference("_");
    EntityName rightEntityName = EntityName::STMT;
    expectedResult = {value2, value3};
    output = facade.solveRight(RelationshipReference::FOLLOWS_T, leftRef,
                               rightEntityName);
    std::sort(output.begin(), output.end());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Follows*, _, Statement) returns {'1'} not {'1', '1'}
    Reference rightRef = Reference("_");
    EntityName leftEntityName = EntityName::STMT;
    expectedResult = {value1, value2};
    output = facade.solveLeft(RelationshipReference::FOLLOWS_T, rightRef,
                              leftEntityName);
    std::sort(output.begin(), output.end());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("StmtToVar: Validate returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Variable var1 = Variable("a");
    Relationship<int, std::string> rs =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("a"));
    statements->store(&line1);
    variables->store(&var1);
    modifiesS->store(&rs);

    Reference leftRef = Reference("1");
    Reference rightRef = Reference("a");
    Reference wildcardRef = Reference("_");

    // Queries where relationships do exist
    REQUIRE(
        facade.validate(RelationshipReference::MODIFIES, leftRef, rightRef));
    REQUIRE(
        facade.validate(RelationshipReference::MODIFIES, leftRef, wildcardRef));
    REQUIRE(facade.validate(RelationshipReference::MODIFIES, wildcardRef,
                            rightRef));
    REQUIRE(facade.validate(RelationshipReference::MODIFIES, wildcardRef,
                            wildcardRef));

    // Queries where relationships do not exist
    REQUIRE(!facade.validate(RelationshipReference::MODIFIES, Reference("2"),
                             rightRef));
    REQUIRE(!facade.validate(RelationshipReference::MODIFIES, leftRef,
                             Reference("x")));
    REQUIRE(!facade.validate(RelationshipReference::MODIFIES, Reference("2"),
                             wildcardRef));
    REQUIRE(!facade.validate(RelationshipReference::MODIFIES, wildcardRef,
                             Reference("x")));
}

TEST_CASE("StmtToVar: SolveRight queries for Modifies(1, 'a') return correct "
          "results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Variable var1 = Variable("a");
    Relationship<int, std::string> rs =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("a"));
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
    output = facade.solveRight(RelationshipReference::MODIFIES, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Modifies, 2, Variable) for Modifies(1, 'a') returns {}
    leftRef = Reference("2");
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::MODIFIES, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Modifies, 1, Procedure) for Modifies(1, 'a') returns {}
    leftRef = Reference("1");
    rightEntityName = EntityName::PROCEDURE;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::MODIFIES, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Modifies, _, Variable) for Modifies(1, 'a') returns {'a'}
    leftRef = Reference("_");
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {Value(ValueType::VAR_NAME, "a")};
    output = facade.solveRight(RelationshipReference::MODIFIES, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("StmtToVar: SolveLeft queries for Modifies(1, 'a') return correct "
          "results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Variable var1 = Variable("a");
    Relationship<int, std::string> rs =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("a"));
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
    output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Modifies, x, Assign) for Modifies(1, 'a') returns {}
    rightRef = Reference("x");
    leftEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Modifies, a, Print) for Modifies(1, 'a') returns {}
    rightRef = Reference("a");
    leftEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Modifies, _, Assign) for Modifies(1, 'a') returns {'1'}
    rightRef = Reference("_");
    leftEntityName = EntityName::ASSIGN;
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("StmtToVar: SolveBoth queries for Modifies(1, 'a') return correct "
          "results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Variable var1 = Variable("a");
    Relationship<int, std::string> rs =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("a"));
    statements->store(&line1);
    variables->store(&var1);
    modifiesS->store(&rs);
    Value value1 = Value(ValueType::STMT_NUM, "1");
    Value value2 = Value(ValueType::VAR_NAME, "a");

    EntityName leftEntityName;
    EntityName rightEntityName;
    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // SolveBoth(Modifies, Stmt, Var) for Modifies(1, 'a') returns {('1', 'a')}
    leftEntityName = EntityName::STMT;
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {std::make_pair(value1, value2)};
    output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName,
                              rightEntityName);
    REQUIRE(expectedResult.size() == output.size());
    REQUIRE(expectedResult[0].first == output[0].first);
    REQUIRE(expectedResult[0].second == output[0].second);

    // SolveBoth(Modifies, Stmt, Print) for Modifies(1, 'a') returns {}
    leftEntityName = EntityName::STMT;
    rightEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName,
                              rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveBoth(Modifies, Call, Var) for Modifies(1, 'a') returns {}
    leftEntityName = EntityName::CALL;
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName,
                              rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("ProcToVar: Validate returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesPTable *modifiesP = storage->getTable<ModifiesPTable>();

    Procedure proc1 = Procedure("Main", 1);
    Variable var1 = Variable("a");
    Relationship<std::string, std::string> rs = Relationship(
        RelationshipReference::MODIFIES, std::string("Main"), std::string("a"));
    procedures->store(&proc1);
    variables->store(&var1);
    modifiesP->store(&rs);

    Reference leftRef = Reference("Main");
    Reference rightRef = Reference("a");
    Reference wildcardRef = Reference("_");

    // Queries where relationships do exist
    REQUIRE(
        facade.validate(RelationshipReference::MODIFIES, leftRef, rightRef));
    REQUIRE(
        facade.validate(RelationshipReference::MODIFIES, leftRef, wildcardRef));
    REQUIRE(facade.validate(RelationshipReference::MODIFIES, wildcardRef,
                            rightRef));
    REQUIRE(facade.validate(RelationshipReference::MODIFIES, wildcardRef,
                            wildcardRef));

    // Queries where relationships do not exist
    REQUIRE(!facade.validate(RelationshipReference::MODIFIES, Reference("Foo"),
                             rightRef));
    REQUIRE(!facade.validate(RelationshipReference::MODIFIES, leftRef,
                             Reference("x")));
    REQUIRE(!facade.validate(RelationshipReference::MODIFIES, Reference("Foo"),
                             wildcardRef));
    REQUIRE(!facade.validate(RelationshipReference::MODIFIES, wildcardRef,
                             Reference("x")));
}

TEST_CASE("ProcToVar: SolveRight queries for Modifies('Main', 'a') return "
          "correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesPTable *modifiesP = storage->getTable<ModifiesPTable>();

    Procedure proc1 = Procedure("Main", 1);
    Variable var1 = Variable("a");
    Relationship<std::string, std::string> rs = Relationship(
        RelationshipReference::MODIFIES, std::string("Main"), std::string("a"));
    procedures->store(&proc1);
    variables->store(&var1);
    modifiesP->store(&rs);

    Reference leftRef;
    EntityName rightEntityName;
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveRight(Modifies, Main, Variable) for Modifies('Main', 'a') returns
    // {'a'}
    leftRef = Reference("Main");
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {Value(ValueType::VAR_NAME, "a")};
    output = facade.solveRight(RelationshipReference::MODIFIES, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Modifies, Foo, Variable) for Modifies('Main', 'a') returns {}
    leftRef = Reference("Foo");
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::MODIFIES, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Modifies, Main, Procedure) for Modifies('Main', 'a') returns
    // {}
    leftRef = Reference("Main");
    rightEntityName = EntityName::PROCEDURE;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::MODIFIES, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Modifies, _, Variable) for Modifies('Main', 'a') returns {'a'}
    leftRef = Reference("_");
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {Value(ValueType::VAR_NAME, "a")};
    output = facade.solveRight(RelationshipReference::MODIFIES, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("ProcToVar: SolveLeft queries for Modifies('Main', 'a') return "
          "correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesPTable *modifiesP = storage->getTable<ModifiesPTable>();

    Procedure proc1 = Procedure("Main", 1);
    Variable var1 = Variable("a");
    Relationship<std::string, std::string> rs = Relationship(
        RelationshipReference::MODIFIES, std::string("Main"), std::string("a"));
    procedures->store(&proc1);
    variables->store(&var1);
    modifiesP->store(&rs);

    Reference rightRef;
    EntityName leftEntityName;
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveLeft(Modifies, a, Procedure) for Modifies('Main', 'a') returns
    // {'Main'}
    rightRef = Reference("a");
    leftEntityName = EntityName::PROCEDURE;
    expectedResult = {Value(ValueType::VAR_NAME, "Main")};
    output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Modifies, x, Procedure) for Modifies('Main', 'a') returns {}
    rightRef = Reference("x");
    leftEntityName = EntityName::PROCEDURE;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Modifies, a, Statement) for Modifies('Main', 'a') returns {}
    rightRef = Reference("a");
    leftEntityName = EntityName::STMT;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Modifies, _, Procedure) for Modifies('Main', 'a') returns
    // {'Main'}
    rightRef = Reference("_");
    leftEntityName = EntityName::PROCEDURE;
    expectedResult = {Value(ValueType::VAR_NAME, "Main")};
    output = facade.solveLeft(RelationshipReference::MODIFIES, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("ProcToVar: SolveBoth queries for Modifies('Main', 'a') return "
          "correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesPTable *modifiesP = storage->getTable<ModifiesPTable>();

    Procedure proc1 = Procedure("Main", 1);
    Variable var1 = Variable("a");
    Relationship<std::string, std::string> rs = Relationship(
        RelationshipReference::MODIFIES, std::string("Main"), std::string("a"));
    procedures->store(&proc1);
    variables->store(&var1);
    modifiesP->store(&rs);
    Value value1 = Value(ValueType::VAR_NAME, "Main");
    Value value2 = Value(ValueType::VAR_NAME, "a");

    EntityName leftEntityName;
    EntityName rightEntityName;
    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // SolveBoth(Modifies, Proc, Var) for Modifies('Main', 'a') returns {('1',
    // '2')}
    leftEntityName = EntityName::PROCEDURE;
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {std::make_pair(value1, value2)};
    output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName,
                              rightEntityName);
    REQUIRE(expectedResult.size() == output.size());
    REQUIRE(expectedResult[0].first == output[0].first);
    REQUIRE(expectedResult[0].second == output[0].second);

    // SolveBoth(Modifies, Proc, Print) for Modifies('Main', 'a') returns {}
    leftEntityName = EntityName::PROCEDURE;
    rightEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName,
                              rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveBoth(Modifies, Call, Var) for Modifies('Main', 'a') returns {}
    leftEntityName = EntityName::CALL;
    rightEntityName = EntityName::VARIABLE;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::MODIFIES, leftEntityName,
                              rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("ProcToProc: Validate returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    CallsTable *calls = storage->getTable<CallsTable>();

    Procedure proc1 = Procedure("proc1", 1);
    Procedure proc2 = Procedure("proc2", 2);
    Relationship<std::string, std::string> rs =
        Relationship(RelationshipReference::CALLS, std::string("proc1"),
                     std::string("proc2"));
    procedures->store(&proc1);
    procedures->store(&proc2);
    calls->store(&rs);

    Reference leftRef = Reference("proc1");
    Reference rightRef = Reference("proc2");
    Reference wildcardRef = Reference("_");
    Reference wrongRef = Reference("Foo");

    // Queries where relationships do exist
    REQUIRE(facade.validate(RelationshipReference::CALLS, leftRef, rightRef));
    REQUIRE(
        facade.validate(RelationshipReference::CALLS, leftRef, wildcardRef));
    REQUIRE(
        facade.validate(RelationshipReference::CALLS, wildcardRef, rightRef));
    REQUIRE(facade.validate(RelationshipReference::CALLS, wildcardRef,
                            wildcardRef));

    // Queries where relationships do not exist
    REQUIRE(!facade.validate(RelationshipReference::CALLS, wrongRef, rightRef));
    REQUIRE(!facade.validate(RelationshipReference::CALLS, leftRef, wrongRef));
    REQUIRE(
        !facade.validate(RelationshipReference::CALLS, wrongRef, wildcardRef));
    REQUIRE(
        !facade.validate(RelationshipReference::CALLS, wildcardRef, wrongRef));
}

TEST_CASE("ProcToProc: SolveRight queries for Calls('proc1', 'proc2') return "
          "correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    CallsTable *calls = storage->getTable<CallsTable>();

    Procedure proc1 = Procedure("proc1", 1);
    Procedure proc2 = Procedure("proc2", 2);
    Relationship<std::string, std::string> rs =
        Relationship(RelationshipReference::CALLS, std::string("proc1"),
                     std::string("proc2"));
    procedures->store(&proc1);
    procedures->store(&proc2);
    calls->store(&rs);

    Reference leftRef;
    EntityName rightEntityName;
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveRight(Calls, Proc1, Proc2) for Calls('proc1', 'proc2') returns
    // {'proc2'}
    leftRef = Reference("proc1");
    rightEntityName = EntityName::PROCEDURE;
    expectedResult = {Value(ValueType::VAR_NAME, "proc2")};
    output = facade.solveRight(RelationshipReference::CALLS, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Calls, _, Proc2) for Calls('proc1', 'proc2') returns {'proc2'}
    leftRef = Reference("_");
    rightEntityName = EntityName::PROCEDURE;
    output = facade.solveRight(RelationshipReference::CALLS, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Calls, Foo, Proc2) for Calls('proc1', 'proc2') returns {}
    leftRef = Reference("Foo");
    rightEntityName = EntityName::PROCEDURE;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::CALLS, leftRef,
                               rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("ProcToProc: SolveLeft queries for Calls('proc1', 'proc2') return "
          "correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    CallsTable *calls = storage->getTable<CallsTable>();

    Procedure proc1 = Procedure("proc1", 1);
    Procedure proc2 = Procedure("proc2", 2);
    Relationship<std::string, std::string> rs =
        Relationship(RelationshipReference::CALLS, std::string("proc1"),
                     std::string("proc2"));
    procedures->store(&proc1);
    procedures->store(&proc2);
    calls->store(&rs);

    Reference rightRef;
    EntityName leftEntityName;
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveLeft(Calls, Proc1, Proc2) for Calls('proc1', 'proc2') returns
    // {'proc1'}
    rightRef = Reference("proc2");
    leftEntityName = EntityName::PROCEDURE;
    expectedResult = {Value(ValueType::VAR_NAME, "proc1")};
    output = facade.solveLeft(RelationshipReference::CALLS, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Calls, _, Proc2) for Calls('proc1', 'proc2') returns
    // {'proc1'}
    rightRef = Reference("_");
    leftEntityName = EntityName::PROCEDURE;
    output = facade.solveLeft(RelationshipReference::CALLS, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Calls, Foo, Proc2) for Calls('Foo', 'proc2') returns {}
    rightRef = Reference("Foo");
    leftEntityName = EntityName::PROCEDURE;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::CALLS, rightRef,
                              leftEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("ProcToVar: SolveBoth queries for Calls('proc1', 'proc2') return "
          "correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    CallsTable *calls = storage->getTable<CallsTable>();

    Procedure proc1 = Procedure("proc1", 1);
    Procedure proc2 = Procedure("proc2", 2);
    Relationship<std::string, std::string> rs =
        Relationship(RelationshipReference::CALLS, std::string("proc1"),
                     std::string("proc2"));
    procedures->store(&proc1);
    procedures->store(&proc2);
    calls->store(&rs);

    Value value1 = Value(ValueType::VAR_NAME, "proc1");
    Value value2 = Value(ValueType::VAR_NAME, "proc2");

    EntityName leftEntityName;
    EntityName rightEntityName;
    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // SolveBoth(Calls, Proc1, Proc2) for Calls('proc1', 'proc2') returns
    // {('proc1', 'proc2')}
    leftEntityName = EntityName::PROCEDURE;
    rightEntityName = EntityName::PROCEDURE;
    expectedResult = {std::make_pair(value1, value2)};
    output = facade.solveBoth(RelationshipReference::CALLS, leftEntityName,
                              rightEntityName);
    REQUIRE(expectedResult.size() == output.size());
    REQUIRE(expectedResult[0].first == output[0].first);
    REQUIRE(expectedResult[0].second == output[0].second);

    // SolveBoth(Calls, Proc1, Print) for Calls('proc1', 'a') returns {}
    leftEntityName = EntityName::PROCEDURE;
    rightEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::CALLS, leftEntityName,
                              rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveBoth(Calls, Call, Proc2) for Calls('a', 'proc2') returns {}
    leftEntityName = EntityName::CALL;
    rightEntityName = EntityName::PROCEDURE;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::CALLS, leftEntityName,
                              rightEntityName);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("Affects: Validate for Affects(1,2) works correctly") {
    QueryFacade facade = InitAffectsStorage::createSimple();

    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");
    Reference wildcardRef = Reference("_");

    REQUIRE(facade.validate(RelationshipReference::AFFECTS, leftRef, rightRef));
    REQUIRE(
        facade.validate(RelationshipReference::AFFECTS, wildcardRef, rightRef));
    REQUIRE(
        facade.validate(RelationshipReference::AFFECTS, leftRef, wildcardRef));
    REQUIRE(facade.validate(RelationshipReference::AFFECTS, wildcardRef,
                            wildcardRef));
}

TEST_CASE("Affects: solveRight for Affects(1,2) works correctly") {
    QueryFacade facade = InitAffectsStorage::createSimple();

    Reference leftRef;
    EntityName rightEntityName;
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveRight(Affects, 1, Assign) for Affects(1,2) returns
    // {'2'}
    leftRef = Reference("1");
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = facade.solveRight(RelationshipReference::AFFECTS, leftRef,
                               rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Affects, _, Assign) for Affects(1,2) returns {'2'}
    leftRef = Reference("_");
    rightEntityName = EntityName::ASSIGN;
    output = facade.solveRight(RelationshipReference::AFFECTS, leftRef,
                               rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Affects, 3, Assign) for Affects(1,2) returns {}
    leftRef = Reference("3");
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::AFFECTS, leftRef,
                               rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("Affects: solveLeft for Affects(1,2) works correctly") {
    QueryFacade facade = InitAffectsStorage::createSimple();

    Reference rightRef;
    EntityName leftEntityName;
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveLeft(Affects, Assign, 2) for Affects(1,2) returns
    // {'1'}
    rightRef = Reference("2");
    leftEntityName = EntityName::ASSIGN;
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = facade.solveLeft(RelationshipReference::AFFECTS, rightRef,
                              leftEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Affects, Assign, _) for Affects(1,2) returns {'1'}
    rightRef = Reference("_");
    leftEntityName = EntityName::ASSIGN;
    output = facade.solveLeft(RelationshipReference::AFFECTS, rightRef,
                              leftEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Affects, Assign, 3) for Affects(1,2) returns {}
    rightRef = Reference("3");
    leftEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::AFFECTS, rightRef,
                              leftEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("Affects: solveBoth for Affects(1,2) works correctly") {
    QueryFacade facade = InitAffectsStorage::createSimple();

    Value value1 = Value(ValueType::STMT_NUM, "1");
    Value value2 = Value(ValueType::STMT_NUM, "2");

    EntityName leftEntityName;
    EntityName rightEntityName;
    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // SolveBoth(Affects, Assign, Assign) for Affects(1,2) returns
    // {('1','2')}
    leftEntityName = EntityName::ASSIGN;
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {std::make_pair(value1, value2)};
    output = facade.solveBoth(RelationshipReference::AFFECTS, leftEntityName,
                              rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveBoth(Affects, Assign, Print) for Affects(1,2) returns {}
    leftEntityName = EntityName::ASSIGN;
    rightEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::AFFECTS, leftEntityName,
                              rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveBoth(Affects, Procedure, Assign) for Affects(1,2) returns {}
    leftEntityName = EntityName::PROCEDURE;
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::AFFECTS, leftEntityName,
                              rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AffectsT: Validate for Affects*(1,2) works correctly") {
    QueryFacade facade = InitAffectsStorage::createSimple();

    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");
    Reference wildcardRef = Reference("_");

    REQUIRE(
        facade.validate(RelationshipReference::AFFECTS_T, leftRef, rightRef));
    REQUIRE(facade.validate(RelationshipReference::AFFECTS_T, wildcardRef,
                            rightRef));
    REQUIRE(facade.validate(RelationshipReference::AFFECTS_T, leftRef,
                            wildcardRef));
    REQUIRE(facade.validate(RelationshipReference::AFFECTS_T, wildcardRef,
                            wildcardRef));
}

TEST_CASE("AffectsT: solveRight for Affects*(1,2) works correctly") {
    QueryFacade facade = InitAffectsStorage::createSimple();

    Reference leftRef;
    EntityName rightEntityName;
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveRight(Affects*, 1, Assign) for Affects*(1,2) returns
    // {'2'}
    leftRef = Reference("1");
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = facade.solveRight(RelationshipReference::AFFECTS_T, leftRef,
                               rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Affects*, _, Assign) for Affects*(1,2) returns {'2'}
    leftRef = Reference("_");
    rightEntityName = EntityName::ASSIGN;
    output = facade.solveRight(RelationshipReference::AFFECTS_T, leftRef,
                               rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveRight(Affects*, 3, Assign) for Affects*(1,2) returns {}
    leftRef = Reference("3");
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveRight(RelationshipReference::AFFECTS_T, leftRef,
                               rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AffectsT: solveLeft for Affects*(1,2) works correctly") {
    QueryFacade facade = InitAffectsStorage::createSimple();

    Reference rightRef;
    EntityName leftEntityName;
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // SolveLeft(Affects*, Assign, 2) for Affects*(1,2) returns
    // {'1'}
    rightRef = Reference("2");
    leftEntityName = EntityName::ASSIGN;
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = facade.solveLeft(RelationshipReference::AFFECTS_T, rightRef,
                              leftEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Affects*, Assign, _) for Affects*(1,2) returns {'1'}
    rightRef = Reference("_");
    leftEntityName = EntityName::ASSIGN;
    output = facade.solveLeft(RelationshipReference::AFFECTS_T, rightRef,
                              leftEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveLeft(Affects*, Assign, 3) for Affects*(1,2) returns {}
    rightRef = Reference("3");
    leftEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveLeft(RelationshipReference::AFFECTS_T, rightRef,
                              leftEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AffectsT: solveBoth for Affects*(1,2) works correctly") {
    QueryFacade facade = InitAffectsStorage::createSimple();

    Value value1 = Value(ValueType::STMT_NUM, "1");
    Value value2 = Value(ValueType::STMT_NUM, "2");

    EntityName leftEntityName;
    EntityName rightEntityName;
    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // SolveBoth(Affects*, Assign, Assign) for Affects*(1,2) returns
    // {('1','2')}
    leftEntityName = EntityName::ASSIGN;
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {std::make_pair(value1, value2)};
    output = facade.solveBoth(RelationshipReference::AFFECTS_T, leftEntityName,
                              rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveBoth(Affects*, Assign, Print) for Affects*(1,2) returns {}
    leftEntityName = EntityName::ASSIGN;
    rightEntityName = EntityName::PRINT;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::AFFECTS_T, leftEntityName,
                              rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // SolveBoth(Affects*, Procedure, Assign) for Affects*(1,2) returns {}
    leftEntityName = EntityName::PROCEDURE;
    rightEntityName = EntityName::ASSIGN;
    expectedResult = {};
    output = facade.solveBoth(RelationshipReference::AFFECTS_T, leftEntityName,
                              rightEntityName);
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("getAssign/getAssignExact returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    AssignmentsTable *assignments = storage->getTable<AssignmentsTable>();

    Assignment test1 = Assignment(1, "x1", "(1)");
    Assignment test2 = Assignment(2, "x1", "((1)+(10))");
    Assignment test3 = Assignment(3, "x2", "((x1)*(x1))");
    assignments->store(&test1);
    assignments->store(&test2);
    assignments->store(&test3);

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // getAssign('_', '_', false) returns {'1', '2', '3'}
    expectedResult = {Value(ValueType::STMT_NUM, "1"),
                      Value(ValueType::STMT_NUM, "2"),
                      Value(ValueType::STMT_NUM, "3")};
    output = facade.getAssign("_", AssignExpression("_", false));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssign('_', '1', false) returns {'1', '2'}
    expectedResult = {Value(ValueType::STMT_NUM, "1"),
                      Value(ValueType::STMT_NUM, "2")};
    output = facade.getAssign("_", AssignExpression("1", false));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssignExact('_', '(1)', true) returns {'1'}
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = facade.getAssign("_", AssignExpression("(1)", true));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssign('x1', '_', false) returns {'1', '2'}
    expectedResult = {Value(ValueType::STMT_NUM, "1"),
                      Value(ValueType::STMT_NUM, "2")};
    output = facade.getAssign("x1", AssignExpression("_", false));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssign('x1', '10', false) returns {'1', '2', '3'}
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = facade.getAssign("x1", AssignExpression("10", false));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssignExact('x1', '(10)', true) returns {}
    expectedResult = {};
    output = facade.getAssign("x1", AssignExpression("(10)", true));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("getAssignAndVar/getAssignAndVarExact returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    AssignmentsTable *assignments = storage->getTable<AssignmentsTable>();

    Assignment test1 = Assignment(1, "x1", "(1)");
    Assignment test2 = Assignment(2, "x1", "((1)+(10))");
    Assignment test3 = Assignment(3, "x2", "((x1)*(x1))");
    assignments->store(&test1);
    assignments->store(&test2);
    assignments->store(&test3);

    Value varX1 = Value(ValueType::VAR_NAME, "x1");
    Value varX2 = Value(ValueType::VAR_NAME, "x2");
    Value stmt1 = Value(ValueType::STMT_NUM, "1");
    Value stmt2 = Value(ValueType::STMT_NUM, "2");
    Value stmt3 = Value(ValueType::STMT_NUM, "3");

    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // getAssignAndVar('_', false) returns
    // {('1', 'x1'), ('2', 'x1'), ('3', 'x2')}
    expectedResult = {std::make_pair(stmt1, varX1),
                      std::make_pair(stmt2, varX1),
                      std::make_pair(stmt3, varX2)};
    output = facade.getAssignAndVar(AssignExpression("_", false));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssignAndVar('(1)', false) returns {('1', 'x1'), ('2', 'x1')}
    expectedResult = {std::make_pair(stmt1, varX1),
                      std::make_pair(stmt2, varX1)};
    output = facade.getAssignAndVar(AssignExpression("(1)", false));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssignAndVarExact('(1)', true) returns {('1', 'x1')}
    expectedResult = {std::make_pair(stmt1, varX1)};
    output = facade.getAssignAndVar(AssignExpression("(1)", true));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssignAndVar('(x1)', false) returns {('3', 'x2')}
    expectedResult = {std::make_pair(stmt3, varX2)};
    output = facade.getAssignAndVar(AssignExpression("(x1)", false));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getAssignAndVarExact('(x1)', true) returns {}
    expectedResult = {};
    output = facade.getAssignAndVar(AssignExpression("(x1)", false));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("GetWhile returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    WhileControlVarTable *whiles = storage->getTable<WhileControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("z"));
    whiles->store(&test1);
    whiles->store(&test2);
    whiles->store(&test3);

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // getCond(Designation::WHILE_C, "x") returns {"1"}
    output = facade.getCond(Designation::WHILE_C, "x");
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getCond(Designation::WHILE_C, "_") returns {"1", "2", "3"}
    output = facade.getCond(Designation::WHILE_C, "_");
    expectedResult = {Value(ValueType::STMT_NUM, "1"),
                      Value(ValueType::STMT_NUM, "2"),
                      Value(ValueType::STMT_NUM, "3")};
    std::sort(output.begin(), output.end());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getCond(Designation::WHILE_C, "invalid") returns {}
    output = facade.getCond(Designation::WHILE_C, "invalid");
    expectedResult = {};
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("GetWhileAndVar returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    WhileControlVarTable *whiles = storage->getTable<WhileControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("z"));
    whiles->store(&test1);
    whiles->store(&test2);
    whiles->store(&test3);

    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    Value varX = Value(ValueType::VAR_NAME, "x");
    Value varY = Value(ValueType::VAR_NAME, "y");
    Value varZ = Value(ValueType::VAR_NAME, "z");
    Value stmt1 = Value(ValueType::STMT_NUM, "1");
    Value stmt2 = Value(ValueType::STMT_NUM, "2");
    Value stmt3 = Value(ValueType::STMT_NUM, "3");

    // getWhileAndVar returns {("1","x"), ("2","y"), ("3","z"),}
    output = facade.getCondAndVar(Designation::WHILE_C);
    expectedResult = {std::make_pair(stmt1, varX), std::make_pair(stmt2, varY),
                      std::make_pair(stmt3, varZ)};
    std::sort(output.begin(), output.end(), value_pair_sort());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("GetIf returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    IfControlVarTable *ifs = storage->getTable<IfControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("z"));
    ifs->store(&test1);
    ifs->store(&test2);
    ifs->store(&test3);

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // getCond(Designation::IF_C, "x") returns {"1"}
    output = facade.getCond(Designation::IF_C, "x");
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getCond(Designation::IF_C, "_") returns {"1", "2", "3"}
    output = facade.getCond(Designation::IF_C, "_");
    expectedResult = {Value(ValueType::STMT_NUM, "1"),
                      Value(ValueType::STMT_NUM, "2"),
                      Value(ValueType::STMT_NUM, "3")};
    std::sort(output.begin(), output.end());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // getCond(Designation::IF_C, "invalid") returns {}
    output = facade.getCond(Designation::IF_C, "invalid");
    expectedResult = {};
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("GetIfAndVar returns correct results") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    IfControlVarTable *ifs = storage->getTable<IfControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("z"));
    ifs->store(&test1);
    ifs->store(&test2);
    ifs->store(&test3);

    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    Value varX = Value(ValueType::VAR_NAME, "x");
    Value varY = Value(ValueType::VAR_NAME, "y");
    Value varZ = Value(ValueType::VAR_NAME, "z");
    Value stmt1 = Value(ValueType::STMT_NUM, "1");
    Value stmt2 = Value(ValueType::STMT_NUM, "2");
    Value stmt3 = Value(ValueType::STMT_NUM, "3");

    // getIfAndVar returns {("1","x"), ("2","y"), ("3","z"),}
    output = facade.getCondAndVar(Designation::IF_C);
    expectedResult = {std::make_pair(stmt1, varX), std::make_pair(stmt2, varY),
                      std::make_pair(stmt3, varZ)};
    std::sort(output.begin(), output.end(), value_pair_sort());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("GetCond(invalidDesignation, validValue) returns nothing") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    IfControlVarTable *ifs = storage->getTable<IfControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("z"));
    ifs->store(&test1);
    ifs->store(&test2);
    ifs->store(&test3);

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // getCond(Designation::STMT, "x") returns {}}
    output = facade.getCond(Designation::STMT, "x");
    expectedResult = {};
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("GetCondAndVar(invalidDesignation) returns nothing") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    IfControlVarTable *ifs = storage->getTable<IfControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("z"));
    ifs->store(&test1);
    ifs->store(&test2);
    ifs->store(&test3);

    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // getCondAndVar(invalid) returns {}
    output = facade.getCondAndVar(Designation::STMT);
    expectedResult = {};
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("getAttribute returns correct results") {
    // procedure main {
    //     a = b;
    //     print a;
    //     read x;
    //     call bar;
    // } ... assumes valid procedure bar exists
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

    REQUIRE_THROWS(facade.getSecondaryAttribute(1),
                   "StmtNum does not refer to attributable statement");
    REQUIRE(facade.getSecondaryAttribute(2) == "a");
    REQUIRE(facade.getSecondaryAttribute(3) == "x");
    REQUIRE(facade.getSecondaryAttribute(4) == "bar");
}

TEST_CASE("solveOneAttribute returns correct results -- Relationships") {
    // procedure main {
    //     a = b + 1;
    //     print a;
    //     read x;
    //     call bar;
    //     if (a) ...
    //     while (a) ...
    // } ... assumes valid procedure bar exists
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    UsesSTable *usesS = storage->getTable<UsesSTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();
    CallProcTable *callP = storage->getTable<CallProcTable>();

    // Relationships
    Relationship<int, std::string> rs1 =
        Relationship(RelationshipReference::USES, 1, std::string("b"));
    Relationship<int, std::string> rs2 =
        Relationship(RelationshipReference::USES, 2, std::string("a"));
    Relationship<int, std::string> rs3 =
        Relationship(RelationshipReference::MODIFIES, 3, std::string("x"));
    Relationship<int, std::string> rs4 =
        Relationship(RelationshipReference::USES, 4, std::string("bar"));
    Relationship<int, std::string> rs5 =
        Relationship(RelationshipReference::USES, 5, std::string("a"));
    Relationship<int, std::string> rs6 =
        Relationship(RelationshipReference::USES, 6, std::string("a"));
    usesS->store(&rs1);
    usesS->store(&rs2);
    modifiesS->store(&rs3);
    callP->store(&rs4);
    usesS->store(&rs5);
    usesS->store(&rs6);

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // Call.procName
    expectedResult = {Value(ValueType::STMT_NUM, "4")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::CALL, ""), EntityAttribute::PROC_NAME),
        Value(ValueType::VAR_NAME, "bar"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::CALL, ""), EntityAttribute::PROC_NAME),
        Value(ValueType::VAR_NAME, "init"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Read.varName
    expectedResult = {Value(ValueType::STMT_NUM, "3")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::READ, ""), EntityAttribute::VAR_NAME),
        Value(ValueType::VAR_NAME, "x"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::READ, ""), EntityAttribute::VAR_NAME),
        Value(ValueType::VAR_NAME, "a"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Print.varName
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::PRINT, ""), EntityAttribute::VAR_NAME),
        Value(ValueType::VAR_NAME, "a"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::PRINT, ""), EntityAttribute::VAR_NAME),
        Value(ValueType::VAR_NAME, "A"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("solveOneAttribute returns correct results -- Entities") {
    // procedure main {
    //     a = b + 1;
    //     print a;
    //     read x;
    //     call bar;
    //     if (a) ...
    //     while (a) ...
    // } ... assumes valid procedure bar exists
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ConstantsTable *constants = storage->getTable<ConstantsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();

    // Constants, Variables and Procedures
    constants->store(new Constant("1"));
    variables->store(new Variable("a"));
    variables->store(new Variable("b"));
    variables->store(new Variable("x"));
    procedures->store(new Procedure("main", 1));
    procedures->store(new Procedure("bar", 7));

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // Constants
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::CONSTANT, "1"), EntityAttribute::VALUE),
        Value(ValueType::STMT_NUM, "1"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::CONSTANT, "2"), EntityAttribute::VALUE),
        Value(ValueType::STMT_NUM, "2"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Variables
    expectedResult = {Value(ValueType::VAR_NAME, "a")};
    output =
        facade.solveOneAttribute(Reference(Synonym(EntityName::VARIABLE, "a"),
                                           EntityAttribute::VAR_NAME),
                                 Value(ValueType::VAR_NAME, "a"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::VARIABLE, "main"),
                  EntityAttribute::VAR_NAME),
        Value(ValueType::VAR_NAME, "main"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Procedures
    expectedResult = {Value(ValueType::VAR_NAME, "main")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::PROCEDURE, "main"),
                  EntityAttribute::PROC_NAME),
        Value(ValueType::VAR_NAME, "main"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::PROCEDURE, "init"),
                  EntityAttribute::PROC_NAME),
        Value(ValueType::VAR_NAME, "init"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("solveOneAttribute returns correct results -- Statements") {
    // procedure main {
    //     a = b + 1;
    //     print a;
    //     read x;
    //     call bar;
    //     if (a) ...
    //     while (a) ...
    // } ... assumes valid procedure bar exists
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();

    // Statements
    Statement stmt1 = Statement(1, StatementType::ASSIGN);
    Statement stmt2 = Statement(2, StatementType::PRINT);
    Statement stmt3 = Statement(3, StatementType::READ);
    Statement stmt4 = Statement(4, StatementType::CALL);
    Statement stmt5 = Statement(5, StatementType::IF);
    Statement stmt6 = Statement(6, StatementType::WHILE);
    statements->store(&stmt1);
    statements->store(&stmt2);
    statements->store(&stmt3);
    statements->store(&stmt4);
    statements->store(&stmt5);
    statements->store(&stmt6);

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // STATEMENT
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::STMT, "1"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "1"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::STMT, "7"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "7"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    // ASSIGN
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::ASSIGN, "1"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "1"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::ASSIGN, "6"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "6"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // PRINT
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::PRINT, "2"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "2"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::PRINT, "6"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "6"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // READ
    expectedResult = {Value(ValueType::STMT_NUM, "3")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::READ, "3"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "3"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::READ, "6"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "6"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // CALL
    expectedResult = {Value(ValueType::STMT_NUM, "4")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::CALL, "4"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "4"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::CALL, "6"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "6"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // IF
    expectedResult = {Value(ValueType::STMT_NUM, "5")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::IF, "5"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "5"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::IF, "6"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "6"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // WHILE
    expectedResult = {Value(ValueType::STMT_NUM, "6")};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::WHILE, "6"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "6"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {};
    output = facade.solveOneAttribute(
        Reference(Synonym(EntityName::WHILE, "1"), EntityAttribute::STMT_NO),
        Value(ValueType::STMT_NUM, "1"));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("solveBothAttribute returns correct results -- Relationships") {
    // procedure main {
    //     a = b + 1;
    //     print a;
    //     read a;
    //     call bar;
    //     read bar;
    // } ... assumes valid procedure bar exists
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    UsesSTable *usesS = storage->getTable<UsesSTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();
    CallProcTable *callP = storage->getTable<CallProcTable>();

    // Relationships
    Relationship<int, std::string> rs2 =
        Relationship(RelationshipReference::USES, 2, std::string("a"));
    Relationship<int, std::string> rs3 =
        Relationship(RelationshipReference::MODIFIES, 3, std::string("a"));
    Relationship<int, std::string> rs4 =
        Relationship(RelationshipReference::USES, 4, std::string("bar"));
    Relationship<int, std::string> rs5 =
        Relationship(RelationshipReference::MODIFIES, 5, std::string("bar"));
    usesS->store(&rs2);
    modifiesS->store(&rs3);
    callP->store(&rs4);
    modifiesS->store(&rs5);

    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // Call.procName match with Read.varName
    expectedResult = {
        {Value(ValueType::STMT_NUM, "4"), Value(ValueType::STMT_NUM, "5")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::CALL, ""), EntityAttribute::PROC_NAME),
        Reference(Synonym(EntityName::READ, ""), EntityAttribute::VAR_NAME));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Call.procName match with Print.varName
    expectedResult = {};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::CALL, ""), EntityAttribute::PROC_NAME),
        Reference(Synonym(EntityName::PRINT, ""), EntityAttribute::VAR_NAME));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Read.procName match with Print.varName
    expectedResult = {
        {Value(ValueType::STMT_NUM, "3"), Value(ValueType::STMT_NUM, "2")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::READ, ""), EntityAttribute::VAR_NAME),
        Reference(Synonym(EntityName::PRINT, ""), EntityAttribute::VAR_NAME));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("solveBothAttribute returns correct results -- Statements") {
    // procedure main {
    //     a = b + 1;
    //     print a;
    //     read x;
    //     call bar;
    //     if (a) ...
    //     while (a) ...
    // } ... assumes valid procedure bar exists
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();

    // Statements
    Statement stmt1 = Statement(1, StatementType::ASSIGN);
    Statement stmt2 = Statement(2, StatementType::PRINT);
    statements->store(&stmt1);
    statements->store(&stmt2);

    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // STMT with ASSIGN
    expectedResult = {
        {Value(ValueType::STMT_NUM, "1"), Value(ValueType::STMT_NUM, "1")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::STMT, ""), EntityAttribute::STMT_NO),
        Reference(Synonym(EntityName::ASSIGN, ""), EntityAttribute::STMT_NO));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // ASSIGN with ASSIGN
    expectedResult = {
        {Value(ValueType::STMT_NUM, "1"), Value(ValueType::STMT_NUM, "1")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::ASSIGN, ""), EntityAttribute::STMT_NO),
        Reference(Synonym(EntityName::ASSIGN, ""), EntityAttribute::STMT_NO));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // ASSIGN with PRINT
    expectedResult = {};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::ASSIGN, ""), EntityAttribute::STMT_NO),
        Reference(Synonym(EntityName::PRINT, ""), EntityAttribute::STMT_NO));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // STMT with PRINT
    expectedResult = {
        {Value(ValueType::STMT_NUM, "2"), Value(ValueType::STMT_NUM, "2")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::STMT, ""), EntityAttribute::STMT_NO),
        Reference(Synonym(EntityName::PRINT, ""), EntityAttribute::STMT_NO));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // STMT with STMT
    expectedResult = {
        {Value(ValueType::STMT_NUM, "1"), Value(ValueType::STMT_NUM, "1")},
        {Value(ValueType::STMT_NUM, "2"), Value(ValueType::STMT_NUM, "2")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::STMT, ""), EntityAttribute::STMT_NO),
        Reference(Synonym(EntityName::STMT, ""), EntityAttribute::STMT_NO));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("solveBothAttribute returns correct results -- Entities") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ConstantsTable *constants = storage->getTable<ConstantsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();

    // Constants, Variables and Procedures
    constants->store(new Constant("1"));
    variables->store(new Variable("a"));
    variables->store(new Variable("b"));
    variables->store(new Variable("bar"));
    procedures->store(new Procedure("main", 1));
    procedures->store(new Procedure("bar", 7));

    std::vector<std::pair<Value, Value>> expectedResult;
    std::vector<std::pair<Value, Value>> output;

    // CONST with CONST
    expectedResult = {
        {Value(ValueType::STMT_NUM, "1"), Value(ValueType::STMT_NUM, "1")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::CONSTANT, ""), EntityAttribute::VALUE),
        Reference(Synonym(EntityName::CONSTANT, ""), EntityAttribute::VALUE));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    // VAR with VAR
    expectedResult = {
        {Value(ValueType::VAR_NAME, "a"), Value(ValueType::VAR_NAME, "a")},
        {Value(ValueType::VAR_NAME, "b"), Value(ValueType::VAR_NAME, "b")},
        {Value(ValueType::VAR_NAME, "bar"), Value(ValueType::VAR_NAME, "bar")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::VARIABLE, ""), EntityAttribute::VAR_NAME),
        Reference(Synonym(EntityName::VARIABLE, ""),
                  EntityAttribute::VAR_NAME));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // PROC with PROC
    expectedResult = {
        {Value(ValueType::VAR_NAME, "bar"), Value(ValueType::VAR_NAME, "bar")},
        {Value(ValueType::VAR_NAME, "main"),
         Value(ValueType::VAR_NAME, "main")},
    };
    output =
        facade.solveBothAttribute(Reference(Synonym(EntityName::PROCEDURE, ""),
                                            EntityAttribute::PROC_NAME),
                                  Reference(Synonym(EntityName::PROCEDURE, ""),
                                            EntityAttribute::PROC_NAME));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // VAR with CONST
    expectedResult = {};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::VARIABLE, ""), EntityAttribute::VAR_NAME),
        Reference(Synonym(EntityName::CONSTANT, ""), EntityAttribute::VALUE));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // VAR with PROC
    expectedResult = {
        {Value(ValueType::VAR_NAME, "bar"), Value(ValueType::VAR_NAME, "bar")}};
    output = facade.solveBothAttribute(
        Reference(Synonym(EntityName::VARIABLE, ""), EntityAttribute::VAR_NAME),
        Reference(Synonym(EntityName::PROCEDURE, ""),
                  EntityAttribute::PROC_NAME));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // PROC with VAR
    expectedResult = {
        {Value(ValueType::VAR_NAME, "bar"), Value(ValueType::VAR_NAME, "bar")}};
    output =
        facade.solveBothAttribute(Reference(Synonym(EntityName::PROCEDURE, ""),
                                            EntityAttribute::PROC_NAME),
                                  Reference(Synonym(EntityName::VARIABLE, ""),
                                            EntityAttribute::VAR_NAME));
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("pattern-assign validate works correctly") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    AssignmentsTable *assignmentsTable = storage->getTable<AssignmentsTable>();

    std::string expr1 = "((1)+(2))";
    Assignment assignment1 = Assignment(1, std::string("x1"), expr1);
    assignmentsTable->store(&assignment1);

    std::string expr2 = "((1)+(3))";
    Assignment assignment2 = Assignment(2, std::string("x2"), expr2);
    assignmentsTable->store(&assignment2);

    // Positive testing
    AssignExpression assignExpr1Exact = AssignExpression(expr1, true);
    AssignExpression assignExpr1Partial = AssignExpression("(1)", false);
    AssignExpression assignExpr2Exact = AssignExpression(expr2, true);
    AssignExpression assignExpr2Partial = AssignExpression("(3)", false);
    AssignExpression wildcard = AssignExpression("_", false);

    REQUIRE(facade.validate(1, "x1", assignExpr1Exact));
    REQUIRE(facade.validate(1, "x1", assignExpr1Partial));
    REQUIRE(facade.validate(2, "x2", assignExpr2Exact));
    REQUIRE(facade.validate(2, "x2", assignExpr2Partial));
    REQUIRE(facade.validate(1, "x1", wildcard));
    REQUIRE(facade.validate(2, "x2", wildcard));

    // Wrong stmtNo
    REQUIRE(!facade.validate(2, "x1", assignExpr1Exact));
    REQUIRE(!facade.validate(1, "x2", assignExpr2Partial));

    // Wrong var
    REQUIRE(!facade.validate(1, "nomatchbigsad", assignExpr1Exact));
    REQUIRE(!facade.validate(1, "nomatchbigsad", assignExpr2Partial));

    // Wrong expr
    AssignExpression wrongExact = AssignExpression("(999)", true);
    AssignExpression wrongPartial = AssignExpression("(999)", false);
    REQUIRE(!facade.validate(1, "x1", wrongExact));
    REQUIRE(!facade.validate(2, "x2", wrongPartial));
}

TEST_CASE("pattern-if/while validate works correctly") {
    // TODO: SOME WEIRD BEHAVIOUR HELPPPPP
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    WhileControlVarTable *wTable = storage->getTable<WhileControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 1, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 2, std::string("z"));
    wTable->store(&test1);
    wTable->store(&test2);
    wTable->store(&test3);

    // Positive testing
    REQUIRE(facade.validate(Designation::WHILE_C, 1, "x"));
    REQUIRE(facade.validate(Designation::WHILE_C, 1, "y"));
    REQUIRE(facade.validate(Designation::WHILE_C, 2, "z"));

    // Wrong stmtNo
    REQUIRE(!facade.validate(Designation::WHILE_C, 2, "x"));
    REQUIRE(!facade.validate(Designation::WHILE_C, 1, "z"));

    // Wrong var
    REQUIRE(!facade.validate(Designation::WHILE_C, 1, "z"));
    REQUIRE(!facade.validate(Designation::WHILE_C, 2, "x"));
    REQUIRE(!facade.validate(Designation::WHILE_C, 1, "missingNo"));

    // Read from wrong table
    REQUIRE(!facade.validate(Designation::IF_C, 1, "x"));
}

TEST_CASE("pattern-if/while validate (incorrect designation) returns nothing") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    WhileControlVarTable *wTable = storage->getTable<WhileControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 1, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 2, std::string("z"));
    wTable->store(&test1);
    wTable->store(&test2);
    wTable->store(&test3);

    REQUIRE(!facade.validate(Designation::AFFECTS, 1, "x"));
    REQUIRE(!facade.validate(Designation::B_IN, 1, "x"));
    REQUIRE(!facade.validate(Designation::CALL_T, 1, "x"));
}

TEST_CASE("pattern-assign getVar works correctly") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    AssignmentsTable *assignmentsTable = storage->getTable<AssignmentsTable>();

    std::string expr1 = "((1)+(2))";
    Assignment assignment1 = Assignment(1, std::string("x1"), expr1);
    assignmentsTable->store(&assignment1);

    std::string expr2 = "((1)+(3))";
    Assignment assignment2 = Assignment(2, std::string("x2"), expr2);
    assignmentsTable->store(&assignment2);

    // Positive testing
    AssignExpression assignExpr1Exact = AssignExpression(expr1, true);
    AssignExpression assignExpr1Partial = AssignExpression("(1)", false);
    AssignExpression assignExpr2Exact = AssignExpression(expr2, true);
    AssignExpression assignExpr2Partial = AssignExpression("(3)", false);
    AssignExpression wildcard = AssignExpression("_", false);

    std::vector<Value> expectedResult = {Value(ValueType::VAR_NAME, "x1")};
    std::vector<Value> output = facade.getVar(1, assignExpr1Exact);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = facade.getVar(1, assignExpr1Partial);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = facade.getVar(1, wildcard);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {Value(ValueType::VAR_NAME, "x2")};
    output = facade.getVar(2, assignExpr2Exact);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = facade.getVar(2, assignExpr2Partial);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = facade.getVar(2, wildcard);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Wrong stmtNo
    expectedResult = {};
    output = facade.getVar(2, assignExpr1Exact);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = facade.getVar(1, assignExpr2Partial);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Wrong expr
    AssignExpression wrongExact = AssignExpression("(999)", true);
    AssignExpression wrongPartial = AssignExpression("(999)", false);
    output = facade.getVar(1, wrongExact);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    output = facade.getVar(2, wrongPartial);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("pattern if-while getVar works correctly") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    WhileControlVarTable *wTable = storage->getTable<WhileControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 1, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 2, std::string("z"));
    wTable->store(&test1);
    wTable->store(&test2);
    wTable->store(&test3);

    // Positive testing
    std::vector<Value> expectedResult = {Value(ValueType::VAR_NAME, "x"),
                                         Value(ValueType::VAR_NAME, "y")};
    std::vector<Value> output = facade.getVar(Designation::WHILE_C, 1);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
    expectedResult = {Value(ValueType::VAR_NAME, "z")};
    output = facade.getVar(Designation::WHILE_C, 2);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Wrong stmtNo
    expectedResult = {};
    output = facade.getVar(Designation::WHILE_C, 93489453);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}
