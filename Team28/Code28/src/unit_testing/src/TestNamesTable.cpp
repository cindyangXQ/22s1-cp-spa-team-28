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
