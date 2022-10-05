#include "PKB/Storage/Storage.h"

#include "catch.hpp"

#include <iostream>

TEST_CASE("Storage stores and retrieves Constants correctly") {
    Storage storage;
    ConstantsTable *constants =
        (ConstantsTable *)storage.getTable(TableName::CONSTANTS);
    Constant test = Constant("Test");

    constants->store(&test);

    // Constant retrieved from ConstantsTable correctly
    REQUIRE(*constants->retrieve(test.getName()) == test);
}

TEST_CASE("Storage stores and retrieves Statements correctly") {
    Storage storage;
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    Statement test = Statement(1, StatementType::ASSIGN);

    statements->store(&test);

    // Statement retrieved from StatementsTable correctly
    REQUIRE(*statements->retrieve(test.getLineNumber()) == test);
}

TEST_CASE("Storage stores and retrieves Assignments correctly") {
    Storage storage;
    AssignmentsTable *assignments =
        (AssignmentsTable *)storage.getTable(TableName::ASSIGNMENTS);
    Assignment test = Assignment(1, "x1", "(1)");

    assignments->store(&test);

    // Assignment retrieved from Assignments correctly
    std::vector<Value> expectedResult = {Value(ValueType::STMT_NUM, "1")};
    std::vector<Value> output = assignments->containsVarAndExpr("x1", "(1)");
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("Storage stores and retrieves Variables correctly") {
    Storage storage;
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);
    Variable test = Variable("test");

    variables->store(&test);

    // Variable retrieved from VariablesTable correctly
    REQUIRE(*variables->retrieve(test.getName()) == test);
}

TEST_CASE("Storage stores and retrieves Procedures correctly") {
    Storage storage;
    ProceduresTable *procedures =
        (ProceduresTable *)storage.getTable(TableName::VARIABLES);
    Procedure test = Procedure("test");

    procedures->store(&test);

    // Procedure retrieved from ProceduresTable correctly
    REQUIRE(*procedures->retrieve(test.getName()) == test);
}

TEST_CASE("Storage stores and retrieves Parent correctly") {
    Storage storage;
    ParentTable *parents = (ParentTable *)storage.getTable(TableName::PARENT);
    Relationship<int, int> test =
        Relationship(RelationshipReference::PARENT, 1, 2);

    parents->store(&test);

    // Relationship stored to ParentTable correctly
    REQUIRE(parents->retrieveLeft(1).size() == 1);
    REQUIRE(parents->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves ParentT correctly") {
    Storage storage;
    ParentTTable *parentsT =
        (ParentTTable *)storage.getTable(TableName::PARENT_T);
    Relationship<int, int> test =
        Relationship(RelationshipReference::PARENT_T, 1, 2);

    parentsT->store(&test);

    // Relationship stored to ParentTTable correctly
    REQUIRE(parentsT->retrieveLeft(1).size() == 1);
    REQUIRE(parentsT->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves Follows correctly") {
    Storage storage;
    FollowsTable *follows =
        (FollowsTable *)storage.getTable(TableName::FOLLOWS);
    Relationship<int, int> test =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);

    follows->store(&test);

    // Relationship stored to FollowsTable correctly
    REQUIRE(follows->retrieveLeft(1).size() == 1);
    REQUIRE(follows->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves FollowsT correctly") {
    Storage storage;
    FollowsTTable *followsT =
        (FollowsTTable *)storage.getTable(TableName::FOLLOWS_T);
    Relationship<int, int> test =
        Relationship(RelationshipReference::FOLLOWS_T, 1, 2);

    followsT->store(&test);

    // Relationship stored to FollowsTTable correctly
    REQUIRE(followsT->retrieveLeft(1).size() == 1);
    REQUIRE(followsT->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves ModifiesS correctly") {
    Storage storage;
    ModifiesSTable *modifiesS =
        (ModifiesSTable *)storage.getTable(TableName::MODIFIES_S);
    Relationship<int, std::string> test =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("v"));

    modifiesS->store(&test);

    // Relationship stored to ModifiesS correctly
    REQUIRE(modifiesS->retrieveLeft(1).size() == 1);
    REQUIRE(modifiesS->retrieveRight("v").size() == 1);
}

TEST_CASE("Storage stores and retrieves ModifiesP correctly") {
    Storage storage;
    ModifiesPTable *modifiesP =
        (ModifiesPTable *)storage.getTable(TableName::MODIFIES_P);
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::MODIFIES, std::string("main"), std::string("v"));

    modifiesP->store(&test);

    // Relationship stored to ModifiesS correctly
    REQUIRE(modifiesP->retrieveLeft("main").size() == 1);
    REQUIRE(modifiesP->retrieveRight("v").size() == 1);
}

TEST_CASE("Storage stores and retrieves UsesS correctly") {
    Storage storage;
    UsesSTable *usesS = (UsesSTable *)storage.getTable(TableName::USES_S);
    Relationship<int, std::string> test =
        Relationship(RelationshipReference::USES, 1, std::string("v"));

    usesS->store(&test);

    // Relationship stored to ModifiesS correctly
    REQUIRE(usesS->retrieveLeft(1).size() == 1);
    REQUIRE(usesS->retrieveRight("v").size() == 1);
}

TEST_CASE("Storage stores and retrieves UsesP correctly") {
    Storage storage;
    UsesPTable *usesP = (UsesPTable *)storage.getTable(TableName::USES_P);
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::USES, std::string("main"), std::string("v"));

    usesP->store(&test);

    // Relationship stored to ModifiesS correctly
    REQUIRE(usesP->retrieveLeft("main").size() == 1);
    REQUIRE(usesP->retrieveRight("v").size() == 1);
}

TEST_CASE("Storage stores and retrieves Calls correctly") {
    Storage storage;
    CallsTable *calls = (CallsTable *)storage.getTable(TableName::CALLS);
    Relationship<std::string, std::string> test =
        Relationship(RelationshipReference::CALLS, std::string("proc1"),
                     std::string("proc2"));

    calls->store(&test);

    // Relationship stored to Calls correctly
    REQUIRE(calls->retrieveLeft("proc1").size() == 1);
    REQUIRE(calls->retrieveRight("proc2").size() == 1);
}

TEST_CASE("Storage stores and retrieves CallsT correctly") {
    Storage storage;
    CallsTTable *callsT = (CallsTTable *)storage.getTable(TableName::CALLS);
    Relationship<std::string, std::string> test =
        Relationship(RelationshipReference::CALLS_T, std::string("proc1"),
                     std::string("proc2"));

    callsT->store(&test);

    // Relationship stored to Calls correctly
    REQUIRE(callsT->retrieveLeft("proc1").size() == 1);
    REQUIRE(callsT->retrieveRight("proc2").size() == 1);
}

TEST_CASE("Storage stores and retrieves BranchIn correctly") {
    Storage storage;
    BranchInTable *branchIn =
        (BranchInTable *)storage.getTable(TableName::BRANCH_IN);
    Relationship<int, int> test =
        Relationship(RelationshipReference::EMPTY, 1, 2);

    branchIn->store(&test);

    // Relationship stored to BranchInTable correctly
    REQUIRE(branchIn->retrieveLeft(1).size() == 1);
    REQUIRE(branchIn->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves BranchOut correctly") {
    Storage storage;
    BranchOutTable *branchOut =
        (BranchOutTable *)storage.getTable(TableName::BRANCH_OUT);
    Relationship<int, int> test =
        Relationship(RelationshipReference::EMPTY, 1, 2);

    branchOut->store(&test);

    // Relationship stored to FollowsTTable correctly
    REQUIRE(branchOut->retrieveLeft(1).size() == 1);
    REQUIRE(branchOut->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves Next correctly") {
    Storage storage;
    NextTable *next = (NextTable *)storage.getTable(TableName::NEXT);
    Relationship<int, int> test =
        Relationship(RelationshipReference::NEXT, 1, 2);

    next->store(&test);

    // Relationship stored to FollowsTTable correctly
    REQUIRE(next->retrieveLeft(1).size() == 1);
    REQUIRE(next->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves NextT correctly") {
    Storage storage;
    NextTTable *nextT = (NextTTable *)storage.getTable(TableName::NEXT_T);
    Relationship<int, int> test =
        Relationship(RelationshipReference::NEXT_T, 1, 2);

    nextT->store(&test);

    // Relationship stored to FollowsTTable correctly
    REQUIRE(nextT->retrieveLeft(1).size() == 1);
    REQUIRE(nextT->retrieveRight(2).size() == 1);
}

TEST_CASE("Storage stores and retrieves IfControl correctly") {
    Storage storage;
    IfControlVarTable *ifsControl =
        (IfControlVarTable *)storage.getTable(TableName::I_CONTROL);
    Relationship<int, std::string> test =
        Relationship(RelationshipReference::USES, 1, std::string("x"));

    ifsControl->store(&test);

    // Relationship stored to FollowsTTable correctly
    REQUIRE(ifsControl->retrieveLeft(1).size() == 1);
    REQUIRE(ifsControl->retrieveRight("x").size() == 1);
}

TEST_CASE("Storage stores and retrieves WhileControl correctly") {
    Storage storage;
    WhileControlVarTable *whileControl =
        (WhileControlVarTable *)storage.getTable(TableName::W_CONTROL);
    Relationship<int, std::string> test =
        Relationship(RelationshipReference::USES, 1, std::string("x"));

    whileControl->store(&test);

    // Relationship stored to FollowsTTable correctly
    REQUIRE(whileControl->retrieveLeft(1).size() == 1);
    REQUIRE(whileControl->retrieveRight("x").size() == 1);
}
