#include <vector>

#include "PKB/Facades/PopulateFacade.h"

#include "catch.hpp"

TEST_CASE("storeStatement stores empty vector<Statement*> correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    std::vector<Statement *> statements = {};

    facade.storeStatements(&statements);

    StatementsTable *statementsTable =
        storage.getTable<StatementsTable>(TableName::STATEMENTS);

    // returned number of statements is equal to 0
    REQUIRE(statementsTable->getTableSize() == 0);
}

TEST_CASE("storeStatement stores Statement objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);
    std::vector<Statement *> statements = {&test1, &test2};

    facade.storeStatements(&statements);

    StatementsTable *statementsTable =
        storage.getTable<StatementsTable>(TableName::STATEMENTS);

    // returned number of statements is equal to number stored
    REQUIRE(statementsTable->getTableSize() == 2);
    // first statement is test1
    REQUIRE(*statementsTable->retrieve(1) == test1);
    // second statement is test2
    REQUIRE(*statementsTable->retrieve(2) == test2);
}

TEST_CASE("storeAssignments store Assignment objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Assignment test1 = Assignment(1, "x1", "(1)");
    Assignment test2 = Assignment(2, "x1", "(1)");
    Assignment test3 = Assignment(3, "x2", "((x1)*(x1))");
    std::vector<Assignment *> assignments = {&test1, &test2, &test3};

    facade.storeAssignments(&assignments);

    AssignmentsTable *assignmentsTable =
        storage.getTable<AssignmentsTable>(TableName::ASSIGNMENTS);

    // returned number of assignments is equal to number stored
    REQUIRE(assignmentsTable->getTableSize() == 3);

    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // items are stored correctly
    expectedResult = {Value(ValueType::STMT_NUM, "1"),
                      Value(ValueType::STMT_NUM, "2")};
    output = assignmentsTable->containsVarAndExpr("x1", "(1)");
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    expectedResult = {Value(ValueType::STMT_NUM, "3")};
    output = assignmentsTable->containsVarAndExpr("x2", "((x1)*(x1))");
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("storeVariable stores Variable objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Variable test1 = Variable("a");
    Variable test2 = Variable("b");
    Variable test3 = Variable("");
    std::vector<Variable *> variables = {&test1, &test2, &test3};

    facade.storeVariables(&variables);

    VariablesTable *variablesTable =
        storage.getTable<VariablesTable>(TableName::VARIABLES);

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
    std::vector<Constant *> constants = {&test1, &test2, &test3};

    facade.storeConstants(&constants);

    ConstantsTable *constantsTable =
        storage.getTable<ConstantsTable>(TableName::CONSTANTS);

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
    std::vector<Procedure *> procedures = {&test1, &test2, &test3};

    facade.storeProcedures(&procedures);

    ProceduresTable *proceduresTable =
        storage.getTable<ProceduresTable>(TableName::PROCEDURES);

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
    std::vector<Procedure *> procedures = {&test};

    facade.storeProcedures(&procedures);

    ProceduresTable *proceduresTable =
        storage.getTable<ProceduresTable>(TableName::PROCEDURES);
    VariablesTable *variablesTable =
        storage.getTable<VariablesTable>(TableName::VARIABLES);
    ConstantsTable *constantsTable =
        storage.getTable<ConstantsTable>(TableName::CONSTANTS);
    StatementsTable *statementsTable =
        storage.getTable<StatementsTable>(TableName::STATEMENTS);

    // returned number of procedures is equal to number stored
    REQUIRE(proceduresTable->getTableSize() == 1);
    // returned number of other tables is equal to 0
    REQUIRE(variablesTable->getTableSize() == 0);
    REQUIRE(constantsTable->getTableSize() == 0);
    REQUIRE(statementsTable->getTableSize() == 0);
}

TEST_CASE("storeFollows stores Relationship<int, int> objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::FOLLOWS, 2, 3);
    Relationship<int, int> test3 =
        Relationship(RelationshipReference::FOLLOWS, 3, 4);
    std::vector<Relationship<int, int> *> follows = {&test1, &test2, &test3};

    facade.storeFollows(&follows);

    FollowsTable *followsTable =
        storage.getTable<FollowsTable>(TableName::FOLLOWS);

    // Relationship is stored correctly
    REQUIRE(followsTable->getLeftMap().size() == 3);
    REQUIRE(followsTable->getRightMap().size() == 3);
}

TEST_CASE("storeFollowsT stores Relationship<int, int> objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS_T, 1, 2);
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::FOLLOWS_T, 2, 17);
    Relationship<int, int> test3 =
        Relationship(RelationshipReference::FOLLOWS_T, 5, 23);
    std::vector<Relationship<int, int> *> followsT = {&test1, &test2, &test3};

    facade.storeFollowsT(&followsT);

    FollowsTTable *followsTTable =
        storage.getTable<FollowsTTable>(TableName::FOLLOWS_T);

    // Relationship is stored correctly
    REQUIRE(followsTTable->getLeftMap().size() == 3);
    REQUIRE(followsTTable->getRightMap().size() == 3);
}

TEST_CASE("storeParent stores Relationship<int, int> objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::PARENT, 1, 2);
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::PARENT, 2, 3);
    Relationship<int, int> test3 =
        Relationship(RelationshipReference::PARENT, 3, 4);
    std::vector<Relationship<int, int> *> parents = {&test1, &test2, &test3};

    facade.storeParent(&parents);

    ParentTable *parentTable = storage.getTable<ParentTable>(TableName::PARENT);

    // Relationship is stored correctly
    REQUIRE(parentTable->getLeftMap().size() == 3);
    REQUIRE(parentTable->getRightMap().size() == 3);
}

TEST_CASE("storeParentT stores Relationship<int, int> objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::PARENT_T, 1, 2);
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::PARENT_T, 2, 17);
    Relationship<int, int> test3 =
        Relationship(RelationshipReference::PARENT_T, 5, 23);
    std::vector<Relationship<int, int> *> parentsT = {&test1, &test2, &test3};

    facade.storeParentT(&parentsT);

    ParentTTable *parentTTable =
        storage.getTable<ParentTTable>(TableName::PARENT_T);

    // Relationship is stored correctly
    REQUIRE(parentTTable->getLeftMap().size() == 3);
    REQUIRE(parentTTable->getRightMap().size() == 3);
}

TEST_CASE(
    "storeModifiesS stores Relationship<int, std::string> objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("a"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::MODIFIES, 2, std::string("b"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::MODIFIES, 3, std::string("c"));
    std::vector<Relationship<int, std::string> *> modifiesS = {&test1, &test2,
                                                               &test3};

    facade.storeModifiesS(&modifiesS);

    ModifiesSTable *modifiesSTable =
        storage.getTable<ModifiesSTable>(TableName::MODIFIES_S);

    // Relationship is stored correctly
    REQUIRE(modifiesSTable->getLeftMap().size() == 3);
    REQUIRE(modifiesSTable->getRightMap().size() == 3);
}

TEST_CASE("storeModifiesP stores Relationship<std::string, std::string> "
          "objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<std::string, std::string> test1 = Relationship(
        RelationshipReference::MODIFIES, std::string("x"), std::string("a"));
    Relationship<std::string, std::string> test2 = Relationship(
        RelationshipReference::MODIFIES, std::string("y"), std::string("b"));
    Relationship<std::string, std::string> test3 = Relationship(
        RelationshipReference::MODIFIES, std::string("z"), std::string("c"));
    std::vector<Relationship<std::string, std::string> *> modifiesP = {
        &test1, &test2, &test3};

    facade.storeModifiesP(&modifiesP);

    ModifiesPTable *modifiesPTable =
        storage.getTable<ModifiesPTable>(
            TableName::MODIFIES_P);

    // Relationship is stored correctly
    REQUIRE(modifiesPTable->getLeftMap().size() == 3);
    REQUIRE(modifiesPTable->getRightMap().size() == 3);
}

TEST_CASE(
    "storeUsesS stores Relationship<int, std::string> objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("a"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("b"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("c"));
    std::vector<Relationship<int, std::string> *> usesS = {&test1, &test2,
                                                           &test3};

    facade.storeUsesS(&usesS);

    UsesSTable *usesSTable = storage.getTable<UsesSTable>(TableName::USES_S);

    // Relationship is stored correctly
    REQUIRE(usesSTable->getLeftMap().size() == 3);
    REQUIRE(usesSTable->getRightMap().size() == 3);
}

TEST_CASE("storeUsesP stores Relationship<std::string, std::string> objects "
          "correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<std::string, std::string> test1 = Relationship(
        RelationshipReference::USES, std::string("x"), std::string("a"));
    Relationship<std::string, std::string> test2 = Relationship(
        RelationshipReference::USES, std::string("y"), std::string("b"));
    Relationship<std::string, std::string> test3 = Relationship(
        RelationshipReference::USES, std::string("z"), std::string("c"));
    std::vector<Relationship<std::string, std::string> *> usesP = {
        &test1, &test2, &test3};

    facade.storeUsesP(&usesP);

    UsesPTable *usesPTable = storage.getTable<UsesPTable>(TableName::USES_P);

    // Relationship is stored correctly
    REQUIRE(usesPTable->getLeftMap().size() == 3);
    REQUIRE(usesPTable->getRightMap().size() == 3);
}

TEST_CASE("storeCalls stores Relationship<std::string, std::string> objects "
          "correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<std::string, std::string> test1 = Relationship(
        RelationshipReference::CALLS, std::string("x"), std::string("a"));
    Relationship<std::string, std::string> test2 = Relationship(
        RelationshipReference::CALLS, std::string("y"), std::string("b"));
    Relationship<std::string, std::string> test3 = Relationship(
        RelationshipReference::CALLS, std::string("z"), std::string("c"));
    std::vector<Relationship<std::string, std::string> *> calls = {
        &test1, &test2, &test3};

    facade.storeCalls(&calls);

    CallsTable *callsTable = storage.getTable<CallsTable>(TableName::CALLS);

    // Relationship is stored correctly
    REQUIRE(callsTable->getLeftMap().size() == 3);
    REQUIRE(callsTable->getRightMap().size() == 3);
}

TEST_CASE("storeCallsT stores Relationship<std::string, std::string> objects "
          "correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<std::string, std::string> test1 = Relationship(
        RelationshipReference::CALLS_T, std::string("x"), std::string("a"));
    Relationship<std::string, std::string> test2 = Relationship(
        RelationshipReference::CALLS_T, std::string("y"), std::string("b"));
    Relationship<std::string, std::string> test3 = Relationship(
        RelationshipReference::CALLS_T, std::string("z"), std::string("c"));
    std::vector<Relationship<std::string, std::string> *> calls = {
        &test1, &test2, &test3};

    facade.storeCallsT(&calls);

    CallsTTable *callsTable =
        storage.getTable<CallsTTable>(TableName::CALLS_T);

    // Relationship is stored correctly
    REQUIRE(callsTable->getLeftMap().size() == 3);
    REQUIRE(callsTable->getRightMap().size() == 3);
}

TEST_CASE("storeBranchIn stores Relationship<int, int> objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::EMPTY, 1, 2);
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::EMPTY, 2, 3);
    Relationship<int, int> test3 =
        Relationship(RelationshipReference::EMPTY, 3, 4);
    std::vector<Relationship<int, int> *> branchIns = {&test1, &test2, &test3};

    facade.storeBranchIn(&branchIns);

    BranchInTable *branchinTable =
        storage.getTable<BranchInTable>(TableName::BRANCH_IN);

    // Relationship is stored correctly
    REQUIRE(branchinTable->getLeftMap().size() == 3);
    REQUIRE(branchinTable->getRightMap().size() == 3);
}

TEST_CASE(
    "storeBranchOut Follows stores Relationship<int, int> objects correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::EMPTY, 1, 2);
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::EMPTY, 2, 3);
    Relationship<int, int> test3 =
        Relationship(RelationshipReference::EMPTY, 3, 4);
    std::vector<Relationship<int, int> *> branchIns = {&test1, &test2, &test3};

    facade.storeBranchOut(&branchIns);

    BranchOutTable *branchoutTable =
        storage.getTable<BranchOutTable>(TableName::BRANCH_OUT);

    // Relationship is stored correctly
    REQUIRE(branchoutTable->getLeftMap().size() == 3);
    REQUIRE(branchoutTable->getRightMap().size() == 3);
}

TEST_CASE("storeIfControlVar stores Relationship<int, std::string> objects "
          "correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("a"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("b"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("c"));
    std::vector<Relationship<int, std::string> *> usesS = {&test1, &test2,
                                                           &test3};

    facade.storeIfControlVar(&usesS);

    IfControlVarTable *ifsTable =
        storage.getTable<IfControlVarTable>(TableName::I_CONTROL);

    // Relationship is stored correctly
    REQUIRE(ifsTable->getLeftMap().size() == 3);
    REQUIRE(ifsTable->getRightMap().size() == 3);

    // While table is not affected
    WhileControlVarTable *whileTable =
        storage.getTable<WhileControlVarTable>(TableName::W_CONTROL);

    REQUIRE(whileTable->getLeftMap().size() == 0);
    REQUIRE(whileTable->getRightMap().size() == 0);
}

TEST_CASE("storeWhileControlVar stores Relationship<int, std::string> objects "
          "correctly") {
    Storage storage;
    PopulateFacade facade = PopulateFacade(&storage);
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("a"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("b"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("c"));
    std::vector<Relationship<int, std::string> *> usesS = {&test1, &test2,
                                                           &test3};

    facade.storeWhileControlVar(&usesS);

    WhileControlVarTable *whileTable =
        storage.getTable<WhileControlVarTable>(TableName::W_CONTROL);

    // Relationship is stored correctly
    REQUIRE(whileTable->getLeftMap().size() == 3);
    REQUIRE(whileTable->getRightMap().size() == 3);

    // If table is not affected
    IfControlVarTable *ifsTable =
        storage.getTable<IfControlVarTable>(TableName::I_CONTROL);

    REQUIRE(ifsTable->getLeftMap().size() == 0);
    REQUIRE(ifsTable->getRightMap().size() == 0);
}
