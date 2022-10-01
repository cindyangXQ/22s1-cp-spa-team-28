#include "catch.hpp"

#include "PKB/Tables/RelationshipsTable/RelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/UsesControlVarTable.h" 

TEST_CASE("IfControlTable can initialise, store and retrieve correctly") {
    IfControlVarTable ifsTable;

    // Uses (3, "v")
    Relationship<int, std::string> test =
        Relationship(RelationshipReference::USES, 3, std::string("v"));
    ifsTable.store(&test);

    // successfully stored Uses (3, "v")
    REQUIRE(ifsTable.retrieveLeft(3).size() == 1);
    REQUIRE(ifsTable.retrieveRight("v").size() == 1);

    // values in maps are correct
    REQUIRE(ifsTable.retrieveLeft(3).count("v") == 1);
    REQUIRE(ifsTable.retrieveRight("v").count(3) == 1);
}

TEST_CASE("WhileControlTable can initialise, store and retrieve correctly") {
    WhileControlVarTable wTable;

    // Uses (1000, "varNameeeeeeeeeeeeeeeee")
    Relationship<int, std::string> test =
        Relationship(RelationshipReference::USES, 1000, std::string("varNameeeeeeeeeeeeeeeee"));
    wTable.store(&test);

    // successfully stored Uses (1000, "varNameeeeeeeeeeeeeeeee")
    REQUIRE(wTable.retrieveLeft(1000).size() == 1);
    REQUIRE(wTable.retrieveRight("varNameeeeeeeeeeeeeeeee").size() == 1);

    // values in maps are correct
    REQUIRE(wTable.retrieveLeft(1000).count("varNameeeeeeeeeeeeeeeee") == 1);
    REQUIRE(wTable.retrieveRight("varNameeeeeeeeeeeeeeeee").count(1000) == 1);
}

TEST_CASE("getAllStmts caches properly and returns all statements used") {
    WhileControlVarTable wTable;

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 2, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("x"));
    Relationship<int, std::string> test4 =
        Relationship(RelationshipReference::USES, 10, std::string("z"));    
    wTable.store(&test1);
    wTable.store(&test2);
    wTable.store(&test3);
    wTable.store(&test4);

    // Successfully returns all statements used
    std::vector<Value> expected = {Value(ValueType::STMT_NUM, "10"),
                                   Value(ValueType::STMT_NUM, "2"),
                                   Value(ValueType::STMT_NUM, "3")};
    std::vector<Value> output = wTable.getAllStmts();
    REQUIRE(std::equal(expected.begin(), expected.end(),
            output.begin()));
}

TEST_CASE("getStmt returns correct results") {
    WhileControlVarTable wTable;

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 2, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("x"));
    Relationship<int, std::string> test4 =
        Relationship(RelationshipReference::USES, 10, std::string("z"));    
    wTable.store(&test1);
    wTable.store(&test2);
    wTable.store(&test3);
    wTable.store(&test4);

    std::vector<Value> expected;
    std::vector<Value> output;

    // getStmt("_") returns {"10", "2", "3"}
    expected = {Value(ValueType::STMT_NUM, "10"),
                Value(ValueType::STMT_NUM, "2"),
                Value(ValueType::STMT_NUM, "3")};
    output = wTable.getStmt("_");
    REQUIRE(std::equal(expected.begin(), expected.end(),
            output.begin()));

    // getStmt("x") returns {"2", "3"}
    expected = {Value(ValueType::STMT_NUM, "2"),
                Value(ValueType::STMT_NUM, "3")};
    output = wTable.getStmt("x");
    REQUIRE(std::equal(expected.begin(), expected.end(),
            output.begin()));
    
    // getStmt("y") returns {"2"}
    expected = {Value(ValueType::STMT_NUM, "2")};
    output = wTable.getStmt("y");
    REQUIRE(std::equal(expected.begin(), expected.end(),
            output.begin()));

    // getStmt("a") returns {}
    expected = {};
    output = wTable.getStmt("a");
    REQUIRE(std::equal(expected.begin(), expected.end(),
            output.begin()));
    
    // getStmt("") returns {} : empty string
    expected = {};
    output = wTable.getStmt("");
    REQUIRE(std::equal(expected.begin(), expected.end(),
            output.begin()));
}

TEST_CASE("getStmtAndVar returns correct results") {
    WhileControlVarTable wTable;

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 2, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("x"));
    Relationship<int, std::string> test4 =
        Relationship(RelationshipReference::USES, 10, std::string("z"));    
    wTable.store(&test1);
    wTable.store(&test2);
    wTable.store(&test3);
    wTable.store(&test4);

    Value varX = Value(ValueType::VAR_NAME, "x");
    Value varY = Value(ValueType::VAR_NAME, "y");
    Value varZ = Value(ValueType::VAR_NAME, "z");
    Value stmt2 = Value(ValueType::STMT_NUM, "2");
    Value stmt3 = Value(ValueType::STMT_NUM, "3");
    Value stmt10 = Value(ValueType::STMT_NUM, "10");

    std::vector<std::pair<Value, Value>> expected;
    std::vector<std::pair<Value, Value>> output;

    // getStmtAndVar("_") returns {("10", "z"), ("2", "x"), ("2", "y"), ("3", "x")}
    expected = {std::make_pair(stmt10, varZ),
                std::make_pair(stmt2, varX),
                std::make_pair(stmt2, varY),
                std::make_pair(stmt3, varX)};
    output = wTable.getStmtAndVar();
    REQUIRE(std::equal(expected.begin(), expected.end(),
            output.begin()));
}