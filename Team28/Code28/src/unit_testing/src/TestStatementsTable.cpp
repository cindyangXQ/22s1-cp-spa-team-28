#include "PKB/StatementsTable.h"

#include "catch.hpp"

TEST_CASE("StatementsTable is initialised correctly") {
    StatementsTable table;

    REQUIRE(table.getTableSize() == 0);
}

TEST_CASE("StatementsTable store container statements correctly") {
    StatementsTable table;
    Statement whileStmt = Statement(1, StatementType::WHILE);
    Statement ifStmt = Statement(2, StatementType::IF);

    table.store(&whileStmt);
    table.store(&ifStmt);

    REQUIRE(table.getTableSize() == 2);
    REQUIRE(table.getStatementType(whileStmt.getLineNumber()) ==
            StatementType::WHILE);
    REQUIRE(table.getStatementType(ifStmt.getLineNumber()) ==
            StatementType::IF);
}

TEST_CASE(
    "StatementsTable can store and retrieve a statement of type not NONE") {
    StatementsTable table;
    Statement test = Statement(1, StatementType::ASSIGN);

    table.store(&test);

    // test is stored and retrieved correctly
    REQUIRE(*table.retrieve(test.getLineNumber()) == test);
    // invalid index returns a nullptr for retrieve()
    REQUIRE(table.retrieve(-1) == nullptr);
    // tableSize updated correctly
    REQUIRE(table.getTableSize() == 1);
    // test is stored in the correct bucket
    REQUIRE(table.getStatementType(test.getLineNumber()) ==
            StatementType::ASSIGN);
}

TEST_CASE("StatementsTable can getAll statements correctly") {
    StatementsTable table;
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    table.store(&test1);
    table.store(&test2);

    // number of elements in table is equal to number stored
    REQUIRE(table.getAll().size() == 2);
}

TEST_CASE("StatementsTable can getStatementByType correctly") {
    StatementsTable table;
    Statement assignStmt = Statement(1, StatementType::ASSIGN);
    Statement callStmt = Statement(1, StatementType::CALL);
    Statement ifStmt = Statement(1, StatementType::IF);
    Statement printStmt = Statement(1, StatementType::PRINT);
    Statement readStmt = Statement(1, StatementType::READ);
    Statement whileStmt = Statement(1, StatementType::WHILE);

    table.store(&assignStmt);
    table.store(&callStmt);
    table.store(&ifStmt);
    table.store(&printStmt);
    table.store(&readStmt);
    table.store(&whileStmt);

    // 1 statement of each type successfully retrieved
    REQUIRE(table.getStatementsByType(StatementType::ASSIGN).size() == 1);
    REQUIRE(table.getStatementsByType(StatementType::CALL).size() == 1);
    REQUIRE(table.getStatementsByType(StatementType::IF).size() == 1);
    REQUIRE(table.getStatementsByType(StatementType::PRINT).size() == 1);
    REQUIRE(table.getStatementsByType(StatementType::READ).size() == 1);
    REQUIRE(table.getStatementsByType(StatementType::WHILE).size() == 1);
}
