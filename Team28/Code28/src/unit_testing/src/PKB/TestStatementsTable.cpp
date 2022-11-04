#include "PKB/Tables/StatementsTable/StatementsTable.h"

#include "catch.hpp"

TEST_CASE("StatementsTable store container statements correctly") {
    StatementsTable table;
    Statement whileStmt = Statement(1, StatementType::WHILE);
    Statement ifStmt = Statement(2, StatementType::IF);

    table.store(&whileStmt);
    table.store(&ifStmt);

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
    // test is stored in the correct bucket
    REQUIRE(table.getStatementType(test.getLineNumber()) ==
            StatementType::ASSIGN);
}

TEST_CASE("StatementsTable throws exception for NONE") {
    StatementsTable table;
    Statement test = Statement(1, StatementType::NONE);

    // test is stored and retrieved correctly
    REQUIRE_THROWS(table.store(&test), "StatementType cannot be NONE");
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

TEST_CASE("isIfStatement works correctly") {
    StatementsTable table;
    Statement assignStmt = Statement(1, StatementType::ASSIGN);
    Statement callStmt = Statement(2, StatementType::CALL);
    Statement ifStmt = Statement(3, StatementType::IF);
    Statement printStmt = Statement(4, StatementType::PRINT);
    Statement readStmt = Statement(5, StatementType::READ);
    Statement whileStmt = Statement(6, StatementType::WHILE);

    table.store(&assignStmt);
    table.store(&callStmt);
    table.store(&ifStmt);
    table.store(&printStmt);
    table.store(&readStmt);
    table.store(&whileStmt);

    // Only Call, Print and Read statements return true
    REQUIRE(!table.isIfStatement(1));
    REQUIRE(!table.isIfStatement(2));
    REQUIRE(table.isIfStatement(3));
    REQUIRE(!table.isIfStatement(4));
    REQUIRE(!table.isIfStatement(5));
    REQUIRE(!table.isIfStatement(6));
}

TEST_CASE("StatementsTable getAllAsString works correctly") {
    StatementsTable table;
    Statement assignStmt = Statement(1, StatementType::ASSIGN);
    Statement callStmt = Statement(2, StatementType::CALL);
    Statement ifStmt = Statement(3, StatementType::IF);
    Statement printStmt = Statement(4, StatementType::PRINT);
    Statement readStmt = Statement(5, StatementType::READ);
    Statement whileStmt = Statement(6, StatementType::WHILE);

    table.store(&assignStmt);
    table.store(&callStmt);
    table.store(&ifStmt);
    table.store(&printStmt);
    table.store(&readStmt);
    table.store(&whileStmt);

    std::vector<std::string> expectedResult = {"1", "2", "3", "4", "5", "6"};
    std::vector<std::string> output = table.getAllAsString();
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("isAttributableStatement works correctly") {
    StatementsTable table;
    Statement assignStmt = Statement(1, StatementType::ASSIGN);
    Statement callStmt = Statement(2, StatementType::CALL);
    Statement ifStmt = Statement(3, StatementType::IF);
    Statement printStmt = Statement(4, StatementType::PRINT);
    Statement readStmt = Statement(5, StatementType::READ);
    Statement whileStmt = Statement(6, StatementType::WHILE);

    table.store(&assignStmt);
    table.store(&callStmt);
    table.store(&ifStmt);
    table.store(&printStmt);
    table.store(&readStmt);
    table.store(&whileStmt);

    // Only Call, Print and Read statements return true
    REQUIRE(!table.hasSecondaryAttribute(1));
    REQUIRE(table.hasSecondaryAttribute(2));
    REQUIRE(!table.hasSecondaryAttribute(3));
    REQUIRE(table.hasSecondaryAttribute(4));
    REQUIRE(table.hasSecondaryAttribute(5));
    REQUIRE(!table.hasSecondaryAttribute(6));
}

TEST_CASE("getMatchingValue works correctly") {
    StatementsTable table;
    Statement assignStmt = Statement(1, StatementType::ASSIGN);
    Statement callStmt = Statement(2, StatementType::CALL);
    Statement ifStmt = Statement(3, StatementType::IF);
    Statement printStmt = Statement(4, StatementType::PRINT);
    Statement readStmt = Statement(5, StatementType::READ);
    Statement whileStmt = Statement(6, StatementType::WHILE);

    table.store(&assignStmt);
    table.store(&callStmt);
    table.store(&ifStmt);
    table.store(&printStmt);
    table.store(&readStmt);
    table.store(&whileStmt);

    std::vector<Value> test;
    std::vector<Value> result;

    std::vector<std::string> correctStr;
    std::vector<std::string> wrongStr;

    // getMatchingValue by STMT works correctly

    correctStr = {"1", "2", "3", "4", "5", "6"};
    wrongStr = {"7", "8", "9"};

    for (std::string str : correctStr) {
        test = table.getMatchingValue(str, EntityName::STMT);
        result = {Value(ValueType::STMT_NUM, str)};
        REQUIRE(test == result);
    }

    for (std::string str : wrongStr) {
        test = table.getMatchingValue(str, EntityName::STMT);
        result = {};
        REQUIRE(test == result);
    }

    // getMatchingValue by ASSIGN works correctly
    correctStr = {"1"};
    wrongStr = {"2", "3", "4", "5", "6", "7", "8", "9"};
    for (std::string str : correctStr) {
        test = table.getMatchingValue(str, EntityName::ASSIGN);
        result = {Value(ValueType::STMT_NUM, str)};
        REQUIRE(test == result);
    }

    for (std::string str : wrongStr) {
        test = table.getMatchingValue(str, EntityName::ASSIGN);
        result = {};
        REQUIRE(test == result);
    }

    // getMatchingValue by CALL works correctly
    correctStr = {"2"};
    wrongStr = {"1", "3", "4", "5", "6", "7", "8", "9"};
    for (std::string str : correctStr) {
        test = table.getMatchingValue(str, EntityName::CALL);
        result = {Value(ValueType::STMT_NUM, str)};
        REQUIRE(test == result);
    }

    for (std::string str : wrongStr) {
        test = table.getMatchingValue(str, EntityName::CALL);
        result = {};
        REQUIRE(test == result);
    }

    // getMatchingValue by IF works correctly
    correctStr = {"3"};
    wrongStr = {"1", "2", "4", "5", "6", "7", "8", "9"};
    for (std::string str : correctStr) {
        test = table.getMatchingValue(str, EntityName::IF);
        result = {Value(ValueType::STMT_NUM, str)};
        REQUIRE(test == result);
    }

    for (std::string str : wrongStr) {
        test = table.getMatchingValue(str, EntityName::IF);
        result = {};
        REQUIRE(test == result);
    }
    // getMatchingValue by PRINT works correctly
    correctStr = {"4"};
    wrongStr = {"1", "2", "3", "5", "6", "7", "8", "9"};
    for (std::string str : correctStr) {
        test = table.getMatchingValue(str, EntityName::PRINT);
        result = {Value(ValueType::STMT_NUM, str)};
        REQUIRE(test == result);
    }

    for (std::string str : wrongStr) {
        test = table.getMatchingValue(str, EntityName::PRINT);
        result = {};
        REQUIRE(test == result);
    }
    // getMatchingValue by READ works correctly
    correctStr = {"5"};
    wrongStr = {"1", "2", "3", "4", "6", "7", "8", "9"};
    for (std::string str : correctStr) {
        test = table.getMatchingValue(str, EntityName::READ);
        result = {Value(ValueType::STMT_NUM, str)};
        REQUIRE(test == result);
    }

    for (std::string str : wrongStr) {
        test = table.getMatchingValue(str, EntityName::READ);
        result = {};
        REQUIRE(test == result);
    }
    // getMatchingValue by WHILE works correctly
    correctStr = {"6"};
    wrongStr = {"1", "2", "3", "4", "5", "7", "8", "9"};
    for (std::string str : correctStr) {
        test = table.getMatchingValue(str, EntityName::WHILE);
        result = {Value(ValueType::STMT_NUM, str)};
        REQUIRE(test == result);
    }

    for (std::string str : wrongStr) {
        test = table.getMatchingValue(str, EntityName::WHILE);
        result = {};
        REQUIRE(test == result);
    }
}

TEST_CASE("getAllValues works correctly") {
    StatementsTable table;
    Statement assignStmt = Statement(1, StatementType::ASSIGN);
    Statement callStmt = Statement(2, StatementType::CALL);
    Statement ifStmt = Statement(3, StatementType::IF);
    Statement printStmt = Statement(4, StatementType::PRINT);
    Statement readStmt = Statement(5, StatementType::READ);
    Statement whileStmt = Statement(6, StatementType::WHILE);

    table.store(&assignStmt);
    table.store(&callStmt);
    table.store(&ifStmt);
    table.store(&printStmt);
    table.store(&readStmt);
    table.store(&whileStmt);

    std::map<Value, std::vector<Value>> test;
    std::vector<Value> resKeys;
    std::vector<std::vector<Value>> resValues;

    std::vector<std::string> correctStr;

    std::vector<Value> keys;
    std::vector<std::vector<Value>> values;

    // getAllValues by STMT works correctly
    test = table.getAllValues(EntityName::STMT);

    correctStr = {"1", "2", "3", "4", "5", "6"};

    for (std::string str : correctStr) {
        Value val = Value(ValueType::STMT_NUM, str);
        resKeys.push_back(val);
        resValues.push_back({val});
    }

    for (auto &elem : test) {
        keys.push_back(elem.first);
        values.push_back(elem.second);
    }

    for (int i = 0; i < correctStr.size(); i++) {
        REQUIRE(keys[i] == resKeys[i]);
        REQUIRE(values[i] == resValues[i]);
    }

    // getAllValues by ASSIGN works correctly
    test = table.getAllValues(EntityName::ASSIGN);

    correctStr = {"1"};

    for (std::string str : correctStr) {
        Value val = Value(ValueType::STMT_NUM, str);
        resKeys.push_back(val);
        resValues.push_back({val});
    }

    for (auto &elem : test) {
        keys.push_back(elem.first);
        values.push_back(elem.second);
    }

    for (int i = 0; i < correctStr.size(); i++) {
        REQUIRE(keys[i] == resKeys[i]);
        REQUIRE(values[i] == resValues[i]);
    }

    // getAllValues by CALL works correctly
    test = table.getAllValues(EntityName::CALL);

    correctStr = {"2"};

    for (std::string str : correctStr) {
        Value val = Value(ValueType::STMT_NUM, str);
        resKeys.push_back(val);
        resValues.push_back({val});
    }

    for (auto &elem : test) {
        keys.push_back(elem.first);
        values.push_back(elem.second);
    }

    for (int i = 0; i < correctStr.size(); i++) {
        REQUIRE(keys[i] == resKeys[i]);
        REQUIRE(values[i] == resValues[i]);
    }

    // getAllValues by IF works correctly
    test = table.getAllValues(EntityName::IF);

    correctStr = {"3"};

    for (std::string str : correctStr) {
        Value val = Value(ValueType::STMT_NUM, str);
        resKeys.push_back(val);
        resValues.push_back({val});
    }

    for (auto &elem : test) {
        keys.push_back(elem.first);
        values.push_back(elem.second);
    }

    for (int i = 0; i < correctStr.size(); i++) {
        REQUIRE(keys[i] == resKeys[i]);
        REQUIRE(values[i] == resValues[i]);
    }
    // getAllValues by PRINT works correctly
    test = table.getAllValues(EntityName::PRINT);

    correctStr = {"4"};

    for (std::string str : correctStr) {
        Value val = Value(ValueType::STMT_NUM, str);
        resKeys.push_back(val);
        resValues.push_back({val});
    }

    for (auto &elem : test) {
        keys.push_back(elem.first);
        values.push_back(elem.second);
    }

    for (int i = 0; i < correctStr.size(); i++) {
        REQUIRE(keys[i] == resKeys[i]);
        REQUIRE(values[i] == resValues[i]);
    }

    // getAllValues by READ works correctly
    test = table.getAllValues(EntityName::READ);

    correctStr = {"5"};

    for (std::string str : correctStr) {
        Value val = Value(ValueType::STMT_NUM, str);
        resKeys.push_back(val);
        resValues.push_back({val});
    }

    for (auto &elem : test) {
        keys.push_back(elem.first);
        values.push_back(elem.second);
    }

    for (int i = 0; i < correctStr.size(); i++) {
        REQUIRE(keys[i] == resKeys[i]);
        REQUIRE(values[i] == resValues[i]);
    }

    // getAllValues by WHILE works correctly
    test = table.getAllValues(EntityName::WHILE);

    correctStr = {"6"};

    for (std::string str : correctStr) {
        Value val = Value(ValueType::STMT_NUM, str);
        resKeys.push_back(val);
        resValues.push_back({val});
    }

    for (auto &elem : test) {
        keys.push_back(elem.first);
        values.push_back(elem.second);
    }

    for (int i = 0; i < correctStr.size(); i++) {
        REQUIRE(keys[i] == resKeys[i]);
        REQUIRE(values[i] == resValues[i]);
    }
}

TEST_CASE("StatementsTable: getTableSize works correctly") {
    StatementsTable table;
    Statement assignStmt = Statement(1, StatementType::ASSIGN);
    Statement callStmt = Statement(2, StatementType::CALL);
    Statement ifStmt = Statement(3, StatementType::IF);
    Statement printStmt = Statement(4, StatementType::PRINT);
    Statement readStmt = Statement(5, StatementType::READ);
    Statement whileStmt = Statement(6, StatementType::WHILE);

    table.store(&assignStmt);
    table.store(&callStmt);
    table.store(&ifStmt);
    table.store(&printStmt);
    table.store(&readStmt);
    table.store(&whileStmt);

    REQUIRE(table.getTableSize() == 6);
}
