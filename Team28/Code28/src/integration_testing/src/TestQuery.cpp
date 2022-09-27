#include "catch.hpp"

#include "../../spa/src/SPA.h"

TEST_CASE("SPA can be initialised correctly") {
    SPA spa;
    std::list<std::string> results;

    spa.evaluateQuery("stmt s; Select s", results);

    REQUIRE_NOTHROW(spa.parse("hello.txt"));
    REQUIRE(results.size() == 0);
}

TEST_CASE("SPA can process query with 1 statement in PKB") {
    SPA spa;
    PKB pkb = spa.getPKB();
    Statement test = Statement(1, StatementType::ASSIGN);
    std::vector<Statement *> statements = {&test};
    std::list<std::string> results;

    pkb.populateFacade->storeStatements(&statements);
    spa.evaluateQuery("stmt s; Select s", results);

    REQUIRE(results.size() == 1);
    REQUIRE(*results.begin() == "1");
}

TEST_CASE("SPA can process query with multiple statements in PKB") {
    SPA spa;
    PKB pkb = spa.getPKB();
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::CALL);
    Statement test3 = Statement(3, StatementType::IF);
    Statement test4 = Statement(4, StatementType::PRINT);
    Statement test5 = Statement(5, StatementType::READ);
    Statement test6 = Statement(6, StatementType::WHILE);
    Statement test7 = Statement(7, StatementType::ASSIGN);
    Statement test8 = Statement(8, StatementType::CALL);
    std::vector<Statement *> statements = {&test1, &test2, &test3, &test4,
                                           &test5, &test6, &test7, &test8};
    std::list<std::string> results;
    std::list<std::string> expected = {"1", "2", "3", "4", "5", "6", "7", "8"};

    pkb.populateFacade->storeStatements(&statements);
    spa.evaluateQuery("stmt s; Select s", results);

    REQUIRE(results.size() == 8);

    for (std::string lineNo : expected) {
        REQUIRE(std::find(results.begin(), results.end(), lineNo) !=
                results.end());
    }
}

TEST_CASE("SPA can process query with 1 assign in PKB") {
    SPA spa;
    PKB pkb = spa.getPKB();
    Assignment test = Assignment(1, "x", "y");
    std::vector<Assignment *> assigns = {&test};
    std::list<std::string> results;

    pkb.populateFacade->storeAssignments(&assigns);
    spa.evaluateQuery("assign a; variable v; Select v pattern a(v,_)", results);

    REQUIRE(results.size() == 1);
    REQUIRE(*results.begin() == "x");
}

TEST_CASE("SPA can process query with multiple assigns in PKB") {
    SPA spa;
    PKB pkb = spa.getPKB();
    Assignment test1 = Assignment(1, "a", "b");
    Assignment test2 = Assignment(2, "b", "c");
    Assignment test3 = Assignment(3, "c", "d");
    Assignment test4 = Assignment(4, "d", "e");
    Assignment test5 = Assignment(5, "e", "f");
    Assignment test6 = Assignment(6, "f", "g");
    Assignment test7 = Assignment(7, "g", "h");
    Assignment test8 = Assignment(8, "h", "i");
    std::vector<Assignment *> assigns = {&test1, &test2, &test3, &test4,
                                         &test5, &test6, &test7, &test8};
    std::list<std::string> results;
    std::list<std::string> expected = {"a", "b", "c", "d", "e", "f", "g", "h"};

    pkb.populateFacade->storeAssignments(&assigns);
    spa.evaluateQuery("assign a; variable v; Select v pattern a(v,_)", results);

    REQUIRE(results.size() == 8);
    for (std::string var : expected) {
        REQUIRE(std::find(results.begin(), results.end(), var) !=
                results.end());
    }
}

TEST_CASE("SPA can process such that clause") {
    SPA spa;
    PKB pkb = spa.getPKB();
    Statement s1 = Statement(1, StatementType::ASSIGN);
    Statement s2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> test =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    std::vector<Relationship<int, int> *> relations = {&test};
    std::vector<Statement *> statements = {&s1, &s2};
    std::list<std::string> results;

    pkb.populateFacade->storeFollows(&relations);
    pkb.populateFacade->storeStatements(&statements);
    spa.evaluateQuery("stmt s; Select s such that Follows(1, s)", results);

    REQUIRE(results.size() == 1);
    REQUIRE(*results.begin() == "2");
}

TEST_CASE("SPA can process such that and pattern clause") {
    SPA spa;
    PKB pkb = spa.getPKB();
    Statement s1 = Statement(1, StatementType::ASSIGN);
    Statement s2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> test =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Assignment assign = Assignment(2, "x", "y");
    std::vector<Assignment *> assigns = {&assign};
    std::vector<Relationship<int, int> *> relations = {&test};
    std::vector<Statement *> statements = {&s1, &s2};
    std::list<std::string> results;

    pkb.populateFacade->storeFollows(&relations);
    pkb.populateFacade->storeStatements(&statements);
    pkb.populateFacade->storeAssignments(&assigns);
    spa.evaluateQuery(
        "stmt s; assign a; Select s such that Follows(1, s) pattern a(_,_)",
        results);

    REQUIRE(results.size() == 1);
    REQUIRE(*results.begin() == "2");
}
