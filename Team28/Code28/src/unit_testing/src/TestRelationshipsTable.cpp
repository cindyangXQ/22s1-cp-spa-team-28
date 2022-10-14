#include "catch.hpp"

#include "PKB/Tables/RelationshipsTable/ProcToProcRelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/ProcToVarRelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/RelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/StmtToStmtRelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/StmtToVarRelationshipsTable.h"

TEST_CASE("RelationshipsTable can store and retrieve correctly") {
    RelationshipsTable<int, int> table;
    Relationship<int, int> test =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);

    table.store(&test);

    // successfully stored 1 item to left map
    REQUIRE(table.retrieveLeft(1).size() == 1);
    // successfully stored 1 item to right map
    REQUIRE(table.retrieveRight(2).size() == 1);
}

TEST_CASE("RelationshipsTable can storeLeft and retrieve correctly") {
    RelationshipsTable<int, int> table;

    table.storeRightToLeftMap(1, 2);

    // successfully stored 1 item to left map
    REQUIRE(table.retrieveLeft(1).size() == 1);
    // nothing stored to right map
    REQUIRE(table.retrieveRight(2).size() == 0);
}

TEST_CASE("RelationshipsTable can storeRight and retrieve correctly") {
    RelationshipsTable<int, int> table;

    table.storeLeftToRightMap(1, 2);

    // successfully stored 1 item to right map
    REQUIRE(table.retrieveRight(1).size() == 1);
    // nothing stored to left map
    REQUIRE(table.retrieveLeft(2).size() == 0);
}

TEST_CASE("ParentTable can store multiple Rights to the same Left and retrieve "
          "correctly") {
    ParentTable parentTable;

    parentTable.storeRightToLeftMap(1, 2);
    parentTable.storeRightToLeftMap(1, 3);
    parentTable.storeRightToLeftMap(1, 4);

    // successfully stored 3 items to right map
    REQUIRE(parentTable.retrieveLeft(1).size() == 3);
    // all items are stored
    REQUIRE(parentTable.retrieveLeft(1).count(2) == 1);
    REQUIRE(parentTable.retrieveLeft(1).count(3) == 1);
    REQUIRE(parentTable.retrieveLeft(1).count(4) == 1);
    // no incorrect items are stored
    REQUIRE(parentTable.retrieveLeft(1).count(1) == 0);
    // nothing stored to left map
    REQUIRE(parentTable.retrieveRight(2).size() == 0);
}

TEST_CASE("ParentTTable can store multiple Lefts to the same Right and "
          "retrieve correctly") {
    ParentTTable parentTTable;

    parentTTable.storeLeftToRightMap(10, 1);
    parentTTable.storeLeftToRightMap(10, 2);
    parentTTable.storeLeftToRightMap(10, 5);

    // successfully stored 3 items to left map
    REQUIRE(parentTTable.retrieveRight(10).size() == 3);
    // all items are stored
    REQUIRE(parentTTable.retrieveRight(10).count(1) == 1);
    REQUIRE(parentTTable.retrieveRight(10).count(2) == 1);
    REQUIRE(parentTTable.retrieveRight(10).count(5) == 1);
    // no incorrect items are stored
    REQUIRE(parentTTable.retrieveRight(10).count(10) == 0);
    // nothing stored to right map
    REQUIRE(parentTTable.retrieveLeft(1).size() == 0);
}

TEST_CASE("FollowsTable can initialise, store and retrieve correctly") {
    FollowsTable followsTable;

    // Follows (1, 2)
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    followsTable.store(&test1);
    // Follows (2, 5)
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::FOLLOWS, 2, 5);
    followsTable.store(&test2);

    // successfully stored Follows (1, 2)
    REQUIRE(followsTable.retrieveLeft(1).size() == 1);
    REQUIRE(followsTable.retrieveRight(2).size() == 1);
    REQUIRE(followsTable.retrieveLeft(1).count(2) == 1);
    REQUIRE(followsTable.retrieveRight(2).count(1) == 1);

    // successfully stored Follows (2, 5)
    REQUIRE(followsTable.retrieveLeft(2).size() == 1);
    REQUIRE(followsTable.retrieveRight(5).size() == 1);
    REQUIRE(followsTable.retrieveLeft(2).count(5) == 1);
    REQUIRE(followsTable.retrieveRight(5).count(2) == 1);

    // 1 and 5 aren't stored on wrong map
    REQUIRE(followsTable.retrieveRight(1).size() == 0);
    REQUIRE(followsTable.retrieveLeft(5).size() == 0);
}

TEST_CASE("FollowsTTable can initialise, store and retrieve correctly") {
    FollowsTTable followsTTable;

    // Follows* (1, 2)
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS_T, 1, 2);
    followsTTable.store(&test1);
    // Follows* (1, 3), implicitly Follows (2, 3) but not testing latter
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::FOLLOWS_T, 1, 3);
    followsTTable.store(&test2);

    // successfully stored Follows* relations
    REQUIRE(followsTTable.retrieveLeft(1).size() == 2);
    REQUIRE(followsTTable.retrieveRight(2).size() == 1);
    REQUIRE(followsTTable.retrieveRight(3).size() == 1);

    // values in maps are correct
    REQUIRE(followsTTable.retrieveLeft(1).count(2) == 1);
    REQUIRE(followsTTable.retrieveLeft(1).count(3) == 1);
    REQUIRE(followsTTable.retrieveRight(2).count(1) == 1);
    REQUIRE(followsTTable.retrieveRight(3).count(1) == 1);

    // 1 and 3 aren't stored on wrong map
    REQUIRE(followsTTable.retrieveRight(1).size() == 0);
    REQUIRE(followsTTable.retrieveLeft(3).size() == 0);
}

TEST_CASE("ModifiesSTable can store multiple Rights to the same Left and "
          "retrieve correctly") {
    ModifiesSTable modifiesSTable;

    // Modifies (1, "x1"), Modifies (1, "read"), Modifies (1, "B99")
    modifiesSTable.storeRightToLeftMap(1, "x1");
    modifiesSTable.storeRightToLeftMap(1, "read");
    modifiesSTable.storeRightToLeftMap(1, "B99");

    // successfully stored 3 items to right map
    REQUIRE(modifiesSTable.retrieveLeft(1).size() == 3);
    // all items are stored
    REQUIRE(modifiesSTable.retrieveLeft(1).count("x1") == 1);
    REQUIRE(modifiesSTable.retrieveLeft(1).count("read") == 1);
    REQUIRE(modifiesSTable.retrieveLeft(1).count("B99") == 1);
    // nothing stored to left map
    REQUIRE(modifiesSTable.retrieveRight("x1").size() == 0);
}

TEST_CASE("ModifiesSTable can store multiple Lefts to the same Right and "
          "retrieve correctly") {
    ModifiesSTable modifiesSTable;

    // Modifies (1, "x1"), Modifies (20, "x1"), Modifies (1000, "x1")
    modifiesSTable.storeLeftToRightMap("x1", 1);
    modifiesSTable.storeLeftToRightMap("x1", 20);
    modifiesSTable.storeLeftToRightMap("x1", 1000);

    // successfully stored 3 items to left map
    REQUIRE(modifiesSTable.retrieveRight("x1").size() == 3);
    // all items are stored
    REQUIRE(modifiesSTable.retrieveRight("x1").count(1) == 1);
    REQUIRE(modifiesSTable.retrieveRight("x1").count(20) == 1);
    REQUIRE(modifiesSTable.retrieveRight("x1").count(1000) == 1);
    // nothing stored to right map
    REQUIRE(modifiesSTable.retrieveLeft(1).size() == 0);
}

TEST_CASE("ModifiesPTable can initialise, store and retrieve correctly") {
    ModifiesPTable modifiesPTable;

    // Modifies ("main", "v")
    Relationship<std::string, std::string> test1 = Relationship(
        RelationshipReference::MODIFIES, std::string("main"), std::string("v"));
    modifiesPTable.store(&test1);
    // Modifies ("foo", "v")
    Relationship<std::string, std::string> test2 = Relationship(
        RelationshipReference::MODIFIES, std::string("foo"), std::string("v"));
    modifiesPTable.store(&test2);

    // successfully stored Modifies relations
    REQUIRE(modifiesPTable.retrieveLeft("main").size() == 1);
    REQUIRE(modifiesPTable.retrieveLeft("foo").size() == 1);
    REQUIRE(modifiesPTable.retrieveRight("v").size() == 2);

    // values in maps are correct
    REQUIRE(modifiesPTable.retrieveLeft("main").count("v") == 1);
    REQUIRE(modifiesPTable.retrieveLeft("foo").count("v") == 1);
    REQUIRE(modifiesPTable.retrieveRight("v").count("main") == 1);
    REQUIRE(modifiesPTable.retrieveRight("v").count("foo") == 1);

    // "main" and "v" aren't stored on wrong map
    REQUIRE(modifiesPTable.retrieveRight("main").size() == 0);
    REQUIRE(modifiesPTable.retrieveLeft("v").size() == 0);
}

TEST_CASE("UsesSTable can initialise, store and retrieve correctly") {
    UsesSTable usesSTable;

    // Uses (3, "v")
    Relationship<int, std::string> test =
        Relationship(RelationshipReference::USES, 3, std::string("v"));
    usesSTable.store(&test);

    // successfully stored Uses (3, "v")
    REQUIRE(usesSTable.retrieveLeft(3).size() == 1);
    REQUIRE(usesSTable.retrieveRight("v").size() == 1);

    // values in maps are correct
    REQUIRE(usesSTable.retrieveLeft(3).count("v") == 1);
    REQUIRE(usesSTable.retrieveRight("v").count(3) == 1);
}

TEST_CASE("UsesPTable can initialise, store and retrieve correctly") {
    UsesPTable usesPTable;

    // Uses ("foo", "v")
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::USES, std::string("foo"), std::string("v"));
    usesPTable.store(&test);

    // successfully stored Uses ("foo", "v")
    REQUIRE(usesPTable.retrieveLeft("foo").size() == 1);
    REQUIRE(usesPTable.retrieveRight("v").size() == 1);

    // values in maps are correct
    REQUIRE(usesPTable.retrieveLeft("foo").count("v") == 1);
    REQUIRE(usesPTable.retrieveRight("v").count("foo") == 1);
}

TEST_CASE("CallsTable can initialise, store and retrieve correctly") {
    CallsTable callsTable;

    // Calls ("foo", "bar")
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::CALLS, std::string("foo"), std::string("bar"));
    callsTable.store(&test);

    // successfully stored Calls ("foo", "bar")
    REQUIRE(callsTable.retrieveLeft("foo").size() == 1);
    REQUIRE(callsTable.retrieveRight("bar").size() == 1);

    // values in maps are correct
    REQUIRE(callsTable.retrieveLeft("foo").count("bar") == 1);
    REQUIRE(callsTable.retrieveRight("bar").count("foo") == 1);
}

TEST_CASE("CallsTTable can initialise, store and retrieve correctly") {
    CallsTTable callsTTable;

    // Calls* ("foo", "bar")
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::CALLS_T, std::string("foo"), std::string("bar"));
    callsTTable.store(&test);

    // successfully stored Calls* ("foo", "bar")
    REQUIRE(callsTTable.retrieveLeft("foo").size() == 1);
    REQUIRE(callsTTable.retrieveRight("bar").size() == 1);

    // values in maps are correct
    REQUIRE(callsTTable.retrieveLeft("foo").count("bar") == 1);
    REQUIRE(callsTTable.retrieveRight("bar").count("foo") == 1);
}

TEST_CASE("NextTable can initialise, store and retrieve correctly") {
    NextTable nextTable;

    // Next (1, 2)
    Relationship<int, int> test =
        Relationship(RelationshipReference::NEXT, 1, 2);
    nextTable.store(&test);

    // successfully stored Next (1, 2)
    REQUIRE(nextTable.retrieveLeft(1).size() == 1);
    REQUIRE(nextTable.retrieveRight(2).size() == 1);

    // values in maps are correct
    REQUIRE(nextTable.retrieveLeft(1).count(2) == 1);
    REQUIRE(nextTable.retrieveRight(2).count(1) == 1);
}

TEST_CASE("NextTTable can initialise, store and retrieve correctly") {
    NextTTable nextTTable;

    // Next* (1, 2)
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::NEXT_T, 1, 2);
    nextTTable.store(&test1);
    // Next* (1, 3), implicitly Next* (2, 3) but not testing latter
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::NEXT_T, 1, 3);
    nextTTable.store(&test2);

    // successfully stored Next* (1, 2)
    REQUIRE(nextTTable.retrieveLeft(1).size() == 2);
    REQUIRE(nextTTable.retrieveRight(2).size() == 1);

    // values in maps are correct
    REQUIRE(nextTTable.retrieveLeft(1).count(2) == 1);
    REQUIRE(nextTTable.retrieveLeft(1).count(3) == 1);
    REQUIRE(nextTTable.retrieveRight(2).count(1) == 1);
}

TEST_CASE("CallProcTable can initialise, store and retrieve correctly") {
    CallProcTable callProcTable;

    // procedure main { calls bar; calls bar; calls foo }
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::CALLS, 1, std::string("bar"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::CALLS, 2, std::string("bar"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::CALLS, 3, std::string("foo"));
    callProcTable.store(&test1);
    callProcTable.store(&test2);
    callProcTable.store(&test3);

    // successfully stored Call-ProcName relationship
    REQUIRE(callProcTable.retrieveLeft(1).size() == 1);
    REQUIRE(callProcTable.retrieveLeft(2).size() == 1);
    REQUIRE(callProcTable.retrieveLeft(3).size() == 1);
    REQUIRE(callProcTable.retrieveRight("bar").size() == 2);
    REQUIRE(callProcTable.retrieveRight("foo").size() == 1);

    // values in maps are correct
    REQUIRE(callProcTable.retrieveLeft(1).count("bar") == 1);
    REQUIRE(callProcTable.retrieveLeft(2).count("bar") == 1);
    REQUIRE(callProcTable.retrieveLeft(3).count("foo") == 1);
    REQUIRE(callProcTable.retrieveRight("bar").count(1) == 1);
    REQUIRE(callProcTable.retrieveRight("bar").count(2) == 1);
    REQUIRE(callProcTable.retrieveRight("foo").count(3) == 1);
}

TEST_CASE("isLeftValueExist works correctly") {
    FollowsTable follows;

    // procedure main { calls bar; calls bar; calls foo }
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::FOLLOWS, 2, 3);
    follows.store(&test1);
    follows.store(&test2);

    // successfully stored Call-ProcName relationship
    REQUIRE(follows.isLeftValueExist(1));
    REQUIRE(follows.isLeftValueExist(2));
    REQUIRE(!follows.isLeftValueExist(3));
}

TEST_CASE("retrieveSingleRight works for Left with only 1 Right") {
    CallProcTable callProcTable;

    // procedure main { calls bar; calls bar; calls foo }
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::CALLS, 1, std::string("bar"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::CALLS, 2, std::string("bar"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::CALLS, 3, std::string("foo"));
    callProcTable.store(&test1);
    callProcTable.store(&test2);
    callProcTable.store(&test3);

    // successfully stored Call-ProcName relationship
    REQUIRE(callProcTable.retrieveSingleRight(1) == "bar");
    REQUIRE(callProcTable.retrieveSingleRight(2) == "bar");
    REQUIRE(callProcTable.retrieveSingleRight(3) == "foo");
}

TEST_CASE(
    "retrieveSingleRight throws exception for Left with multiple rights") {
    UsesSTable usesS;

    // procedure main { calls bar; calls bar; calls foo }
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 1, std::string("y"));
    usesS.store(&test1);
    usesS.store(&test2);

    // successfully stored Call-ProcName relationship
    REQUIRE_THROWS(usesS.retrieveSingleRight(1),
                   "There exists more than 1 Right value mapped to given Left");
}

TEST_CASE("usesS getMatchingValue works correctly") {
    UsesSTable usesS;

    // procedure main { x = x + 1; y = y + 1; }
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    usesS.store(&test1);
    usesS.store(&test2);
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // "x" retrieves {1}
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = usesS.getMatchingValue("x", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // "y" retrieves {2}
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = usesS.getMatchingValue("y", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // "z" retrieves {}
    expectedResult = {};
    output = usesS.getMatchingValue("z", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("usesS getAllValues works correctly") {
    UsesSTable usesS;

    // procedure main { x = x + 1; y = y + 1; print y;}
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("y"));
    usesS.store(&test1);
    usesS.store(&test2);
    usesS.store(&test3);
    std::map<Value, std::vector<Value>> expectedResult;
    std::map<Value, std::vector<Value>> output;

    // Correctly retrieved
    expectedResult = {
        {Value(ValueType::VAR_NAME, "x"), {Value(ValueType::STMT_NUM, "1")}},
        {Value(ValueType::VAR_NAME, "y"),
         {Value(ValueType::STMT_NUM, "2"), Value(ValueType::STMT_NUM, "3")}}};
    output = usesS.getAllValues(EntityName::STMT);
    for (auto const &[key, value] : expectedResult) {
        REQUIRE(output.count(key) > 0);
        std::vector<Value> outputVector = output[key];

        for (Value v : value) {
            REQUIRE(std::find(outputVector.begin(), outputVector.end(), v) !=
                    outputVector.end());
        }
    }
}

TEST_CASE("modifiesS getMatchingValue works correctly") {
    ModifiesSTable modifiesS;

    // procedure main { x = x + 1; y = y + 1; }
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    modifiesS.store(&test1);
    modifiesS.store(&test2);
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // "x" retrieves {1}
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = modifiesS.getMatchingValue("x", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // "y" retrieves {2}
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = modifiesS.getMatchingValue("y", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // "z" retrieves {}
    expectedResult = {};
    output = modifiesS.getMatchingValue("z", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("callProc getMatchingValue works correctly") {
    CallProcTable callProcs;

    // procedure main { call foo; call bar; }
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::CALLS, 1, std::string("foo"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::CALLS, 2, std::string("bar"));
    callProcs.store(&test1);
    callProcs.store(&test2);
    std::vector<Value> expectedResult;
    std::vector<Value> output;

    // "foo" retrieves {1}
    expectedResult = {Value(ValueType::STMT_NUM, "1")};
    output = callProcs.getMatchingValue("foo", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // "bar" retrieves {2}
    expectedResult = {Value(ValueType::STMT_NUM, "2")};
    output = callProcs.getMatchingValue("bar", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // "z" retrieves {}
    expectedResult = {};
    output = callProcs.getMatchingValue("z", EntityName::STMT);
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("callProc getAllValues works correctly") {
    CallProcTable callProcs;

    // procedure main { call foo; call bar; call bar; }
    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::CALLS, 1, std::string("foo"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::CALLS, 2, std::string("bar"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::CALLS, 3, std::string("bar"));
    callProcs.store(&test1);
    callProcs.store(&test2);
    callProcs.store(&test3);

    std::map<Value, std::vector<Value>> expectedResult;
    std::map<Value, std::vector<Value>> output;

    // Correctly retrieved
    expectedResult = {
        {Value(ValueType::VAR_NAME, "foo"), {Value(ValueType::STMT_NUM, "1")}},
        {Value(ValueType::VAR_NAME, "bar"),
         {Value(ValueType::STMT_NUM, "2"), Value(ValueType::STMT_NUM, "3")}}};
    output = callProcs.getAllValues(EntityName::STMT);
    for (auto const &[key, value] : expectedResult) {
        REQUIRE(output.count(key) > 0);
        std::vector<Value> outputVector = output[key];

        for (Value v : value) {
            REQUIRE(std::find(outputVector.begin(), outputVector.end(), v) !=
                    outputVector.end());
        }
    }
}

TEST_CASE("StmtToStmtRsTables getMatchingValue and getAllValues returns empty "
          "vector") {
    FollowsTable follows;
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    follows.store(&test1);

    // Return empty
    REQUIRE(follows.getMatchingValue("1", EntityName::STMT).size() == 0);
    REQUIRE(follows.getMatchingValue("2", EntityName::STMT).size() == 0);
    REQUIRE(follows.getAllValues(EntityName::STMT).size() == 0);
}

TEST_CASE("ProcToProcRsTables getMatchingValue and getAllValues returns empty "
          "vector") {
    CallsTable calls;
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::CALLS, std::string("foo"), std::string("bar"));
    calls.store(&test);

    // Return empty
    REQUIRE(calls.getMatchingValue("foo", EntityName::STMT).size() == 0);
    REQUIRE(calls.getMatchingValue("bar", EntityName::STMT).size() == 0);
    REQUIRE(calls.getAllValues(EntityName::STMT).size() == 0);
}

TEST_CASE("ProcToVarRsTables getMatchingValue and getAllValues returns empty "
          "vector") {
    UsesPTable usesP;
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::CALLS, std::string("foo"), std::string("x"));
    usesP.store(&test);

    // Return empty
    REQUIRE(usesP.getMatchingValue("foo", EntityName::STMT).size() == 0);
    REQUIRE(usesP.getMatchingValue("x", EntityName::STMT).size() == 0);
    REQUIRE(usesP.getAllValues(EntityName::STMT).size() == 0);
}