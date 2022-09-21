#include "PKB/Tables/NamesTable/NamesTable.h"
#include "commons/Constant.h"
#include "commons/Procedure.h"
#include "commons/Variable.h"

#include "catch.hpp"

TEST_CASE("ConstantsTable (derived from NamesTable) is initialised correctly") {
    ConstantsTable table;

    REQUIRE(table.getTableSize() == 0);
}

TEST_CASE("ConstantsTable can store and retrieve correctly") {
    ConstantsTable table;
    Constant test = Constant("Test");

    table.store(&test);

    // test is stored and retrieved correctly
    REQUIRE(*table.retrieve(test.getName()) == test);
    // invalid index returns a nullptr for retrieve()
    REQUIRE(table.retrieve("not test") == nullptr);
    // tableSize updated correctly
    REQUIRE(table.getTableSize() == 1);
}

TEST_CASE("VariablesTable can store and retrieve correctly") {
    VariablesTable table;
    Variable test = Variable("Test");

    table.store(&test);

    // test is stored and retrieved correctly
    REQUIRE(*table.retrieve(test.getName()) == test);
    // invalid index returns a nullptr for retrieve()
    REQUIRE(table.retrieve("not test") == nullptr);
    // tableSize updated correctly
    REQUIRE(table.getTableSize() == 1);
}

TEST_CASE("ProceduresTable can store and retrieve correctly") {
    ProceduresTable table;
    Procedure test = Procedure("Test");

    table.store(&test);

    // test is stored and retrieved correctly
    REQUIRE(*table.retrieve(test.getName()) == test);
    // invalid index returns a nullptr for retrieve()
    REQUIRE(table.retrieve("not test") == nullptr);
    // tableSize updated correctly
    REQUIRE(table.getTableSize() == 1);
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
    REQUIRE(table.getTableSize() == 3);
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
    Procedure test1 = Procedure("test1");
    Procedure test2 = Procedure("test2");

    table.store(&test1);
    table.store(&test2);

    // number of elements in table is equal to number stored
    REQUIRE(table.getAll().size() == 2);
}
