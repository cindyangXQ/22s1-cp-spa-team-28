#include "PKB/Tables/NamesTable/NamesTable.h"
#include "commons/Constant.h"
#include "commons/Procedure.h"
#include "commons/Variable.h"

#include <algorithm>
#include <string>

#include "catch.hpp"

TEST_CASE("ConstantsTable can store and retrieve correctly") {
    ConstantsTable table;
    Constant test = Constant("Test");

    table.store(&test);

    // test is stored and retrieved correctly
    REQUIRE(*table.retrieve(test.getName()) == test);
    // invalid index returns a nullptr for retrieve()
    REQUIRE(table.retrieve("not test") == nullptr);
}

TEST_CASE("VariablesTable can store and retrieve correctly") {
    VariablesTable table;
    Variable test = Variable("Test");

    table.store(&test);

    // test is stored and retrieved correctly
    REQUIRE(*table.retrieve(test.getName()) == test);
    // invalid index returns a nullptr for retrieve()
    REQUIRE(table.retrieve("not test") == nullptr);
}

TEST_CASE("ProceduresTable can store and retrieve correctly") {
    ProceduresTable table;
    Procedure test = Procedure("Test", 1);

    table.store(&test);

    // test is stored and retrieved correctly
    REQUIRE(*table.retrieve(test.getName()) == test);
    // invalid index returns a nullptr for retrieve()
    REQUIRE(table.retrieve("not test") == nullptr);
}

TEST_CASE("ConstantsTable can store 10 items correctly") {
    ConstantsTable table;
    Constant a = Constant("a");
    table.store(&a);
    Constant b = Constant("b");
    table.store(&b);
    Constant c = Constant("c");
    table.store(&c);

    REQUIRE(*table.retrieve(a.getName()) == a);
    REQUIRE(*table.retrieve("b") == b);
}

TEST_CASE("VariablesTable can getAll statements correctly") {
    VariablesTable table;
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    table.store(&test1);
    table.store(&test2);

    // number of elements in table is equal to number stored
    REQUIRE(table.getAll().size() == 2);
}

TEST_CASE("ConstantsTable can getAll statements correctly") {
    ConstantsTable table;
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    table.store(&test1);
    table.store(&test2);

    // number of elements in table is equal to number stored
    REQUIRE(table.getAll().size() == 2);
}

TEST_CASE("ProceduresTable can getAll statements correctly") {
    ProceduresTable table;
    Procedure test1 = Procedure("test1", 1);
    Procedure test2 = Procedure("test2", 2);

    table.store(&test1);
    table.store(&test2);

    // number of elements in table is equal to number stored
    REQUIRE(table.getAll().size() == 2);
}

TEST_CASE("ConstantsTable::getMatchingValue works correctly") {
    ConstantsTable table;
    Constant c1 = Constant("1");
    Constant c2 = Constant("2");
    Constant c3 = Constant("3");
    Constant c4 = Constant("4");
    Constant c5 = Constant("5");
    Constant c6 = Constant("6");

    table.store(&c1);
    table.store(&c2);
    table.store(&c3);
    table.store(&c4);
    table.store(&c5);
    table.store(&c6);

    // getMatchingValue gets each Constant correctly
    std::vector<Value> test;
    std::vector<Value> result;

    std::vector<std::string> correctStr = {"1", "2", "3", "4", "5", "6"};
    std::vector<std ::string> wrongStr = {"7", "8", "9"};

    for (std::string str : correctStr) {
        test = table.getMatchingValue(
            str, EntityName::STMT); // EntityName is placeholder
        result = {Value(ValueType::STMT_NUM, str)};
        REQUIRE(test == result);
    }
}

TEST_CASE("ConstantsTable::getAllValues works correctly") {
    ConstantsTable table;
    Constant c1 = Constant("1");
    Constant c2 = Constant("2");
    Constant c3 = Constant("3");
    Constant c4 = Constant("4");
    Constant c5 = Constant("5");
    Constant c6 = Constant("6");

    table.store(&c1);
    table.store(&c2);
    table.store(&c3);
    table.store(&c4);
    table.store(&c5);
    table.store(&c6);

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
}

TEST_CASE("ProceduresTable::getAllStmtNum works correctly") {
    ProceduresTable table;
    Procedure p1 = Procedure("test1", 1);
    Procedure p2 = Procedure("test2", 5);
    Procedure p3 = Procedure("test3", 8);
    Procedure p4 = Procedure("test4", 11);
    Procedure p5 = Procedure("test5", 15);
    Procedure p6 = Procedure("test6", 23);

    table.store(&p1);
    table.store(&p2);
    table.store(&p3);
    table.store(&p4);
    table.store(&p5);
    table.store(&p6);

    // Procedure stmt nums match up
    std::vector<int> test = table.getAllStmtNum();
    std::vector<int> result = {1, 5, 8, 11, 15, 23};

    REQUIRE(test == result);
}

TEST_CASE("VariablesTable getAllAsString works correctly") {
    VariablesTable table;
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    table.store(&test1);
    table.store(&test2);

    std::vector<std::string> expectedResult = {"test1", "test2"};
    std::vector<std::string> output = table.getAllAsString();
    std::sort(output.begin(), output.end());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("ConstantsTable getAllAsString works correctly") {
    ConstantsTable table;
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    table.store(&test1);
    table.store(&test2);

    std::vector<std::string> expectedResult = {"test1", "test2"};
    std::vector<std::string> output = table.getAllAsString();
    std::sort(output.begin(), output.end());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("ProceduresTable getAllAsString works correctly") {
    ProceduresTable table;
    Procedure test1 = Procedure("test1", 1);
    Procedure test2 = Procedure("test2", 2);

    table.store(&test1);
    table.store(&test2);

    std::vector<std::string> expectedResult = {"test1", "test2"};
    std::vector<std::string> output = table.getAllAsString();
    std::sort(output.begin(), output.end());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}
