#include "PKB/Facades/QueryFacade.h"
#include "QPS/QPS.h"
#include "SP/SP.h"

#include "catch.hpp"

TEST_CASE("QPS evaluate select statements") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "stmt s; Select s";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correctOutput{"1", "2"};
    REQUIRE(results == correctOutput);
}

TEST_CASE("QPS evaluate select assign statements") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "assign s; Select s";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correctOutput{"1", "2"};
    REQUIRE(results == correctOutput);
}

TEST_CASE("QPS evaluate select if statements") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    Statement test1 = Statement(1, StatementType::IF);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "if s; Select s";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correctOutput{"1"};
    REQUIRE(results == correctOutput);
}

TEST_CASE("QPS evaluate select variables") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    VariablesTable *variables = storage->getTable<VariablesTable>();
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "variable v; Select v";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correctOutput{"test1", "test2"};
    REQUIRE(results == correctOutput);
}

TEST_CASE("QPS evaluate select constants") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ConstantsTable *constants = storage->getTable<ConstantsTable>();
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    constants->store(&test1);
    constants->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "constant c; Select c";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correctOutput{"test1", "test2"};
    REQUIRE(results == correctOutput);
}

TEST_CASE("QPS evaluate select procedures") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    Procedure test1 = Procedure("test1", 1);
    Procedure test2 = Procedure("test2", 2);

    procedures->store(&test1);
    procedures->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "procedure p; Select p";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correctOutput{"test1", "test2"};
    REQUIRE(results == correctOutput);
}

TEST_CASE("QPS can evaluate select procedures when there are no procedures") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();

    QPS qps = QPS(&facade);

    std::string input = "procedure p; Select p";
    std::list<std::string> results;
    qps.evaluate(input, results);
    REQUIRE(results.size() == 0);
}

TEST_CASE("QPS evaluate syntax error") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    VariablesTable *variables = storage->getTable<VariablesTable>();
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    QPS qps = QPS(&facade);

    std::string extra_bracket =
        "variable v; Select v such that Modifies((1, v)";
    std::list<std::string> results;
    qps.evaluate(extra_bracket, results);
    std::list<std::string> correctOutput = {"SyntaxError"};
    REQUIRE(results == correctOutput);
}

TEST_CASE("QPS evaluate semantic error") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    VariablesTable *variables = storage->getTable<VariablesTable>();
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    QPS qps = QPS(&facade);

    std::string undeclared_synonym =
        "variable v; Select v such that Modifies(1, yey)";
    std::list<std::string> results;
    qps.evaluate(undeclared_synonym, results);
    std::list<std::string> correctOutput = {"SemanticError"};
    REQUIRE(results == correctOutput);
}

TEST_CASE("QPS can process queries with follows relationship") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Statement line3 = Statement(3, StatementType::ASSIGN);
    Relationship<int, int> rs1 =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Relationship<int, int> rs2 =
        Relationship(RelationshipReference::FOLLOWS, 2, 3);
    statements->store(&line1);
    statements->store(&line2);
    statements->store(&line3);
    follows->store(&rs1);
    follows->store(&rs2);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correctOutput;
    std::unordered_set<std::string> resultsSet;
    std::unordered_set<std::string> correctOutputSet;

    input = "stmt s; Select s such that Follows(s, 2)";
    correctOutput = {"1"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; Select s such that Follows(1, s)";
    correctOutput = {"2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; Select s such that Follows(1, 2)";
    correctOutput = {"1", "2", "3"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; Select s such that Follows(2, 1)";
    correctOutput = {};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; Select s such that Follows(s, 1)";
    correctOutput = {};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; Select s such that Follows(_, 3)";
    correctOutput = {"1", "2", "3"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; Select s such that Follows(s, _)";
    correctOutput = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s1; stmt s2; Select s1 such that Follows(s1, s2)";
    correctOutput = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);
}

TEST_CASE("QPS can process queries with modifies relationship") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Variable var1 = Variable("a");
    Variable var2 = Variable("b");
    Relationship<int, std::string> rs1 =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("a"));
    Relationship<int, std::string> rs2 =
        Relationship(RelationshipReference::MODIFIES, 2, std::string("b"));
    statements->store(&line1);
    statements->store(&line2);
    variables->store(&var1);
    variables->store(&var2);
    modifiesS->store(&rs1);
    modifiesS->store(&rs2);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correctOutput;
    std::unordered_set<std::string> resultsSet;
    std::unordered_set<std::string> correctOutputSet;

    input = "stmt s; variable v; Select s such that Modifies(s, v)";
    correctOutput = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; variable v; Select s such that Modifies(s, \"a\")";
    correctOutput = {"1"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; variable v; Select s such that Modifies(_, \"a\")";
    results = {};
    qps.evaluate(input, results);
    correctOutput = {"SemanticError"};
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "stmt s; variable v; Select s such that Modifies(1, \"a\")";
    correctOutput = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);
}

TEST_CASE("QPS can process queries with pattern clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    AssignmentsTable *assignments = storage->getTable<AssignmentsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();

    Variable var1 = Variable("a");
    Variable var2 = Variable("b");
    Assignment assignment1 = Assignment(1, "a", "(b)");
    Assignment assignment2 = Assignment(2, "b", "(a)");
    assignments->store(&assignment1);
    assignments->store(&assignment2);
    variables->store(&var1);
    variables->store(&var2);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correctOutput;
    std::unordered_set<std::string> resultsSet;
    std::unordered_set<std::string> correctOutputSet;

    input = "assign a; Select a pattern a(\"a\", _)";
    correctOutput = {"1"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "assign a; Select a pattern a(_, _)";
    correctOutput = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "assign a; variable v; Select v pattern a(v, _)";
    correctOutput = {"a", "b"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "assign a; variable v; Select a pattern a(\"b\", _\"a\"_)";
    correctOutput = {"2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);
}

TEST_CASE("QPS can process queries with such that and pattern clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    AssignmentsTable *assignments = storage->getTable<AssignmentsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesSTable *modifiesS = storage->getTable<ModifiesSTable>();
    UsesSTable *usesS = storage->getTable<UsesSTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Variable var1 = Variable("a");
    Variable var2 = Variable("b");
    Assignment assignment1 = Assignment(1, "a", "(b)");
    Assignment assignment2 = Assignment(2, "b", "(a)");
    assignments->store(&assignment1);
    assignments->store(&assignment2);
    Relationship<int, std::string> rs1 =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("a"));
    Relationship<int, std::string> rs2 =
        Relationship(RelationshipReference::MODIFIES, 2, std::string("b"));
    Relationship<int, std::string> rs3 =
        Relationship(RelationshipReference::USES, 1, std::string("b"));
    Relationship<int, std::string> rs4 =
        Relationship(RelationshipReference::USES, 2, std::string("a"));
    statements->store(&line1);
    statements->store(&line2);
    variables->store(&var1);
    variables->store(&var2);
    modifiesS->store(&rs1);
    modifiesS->store(&rs2);
    usesS->store(&rs3);
    usesS->store(&rs4);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correctOutput;
    std::unordered_set<std::string> resultsSet;
    std::unordered_set<std::string> correctOutputSet;

    input = "assign a; variable v; Select a such that Modifies(1, \"a\") "
            "pattern a(\"b\", _\"a\"_)";
    correctOutput = {"2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "assign a; variable v; Select v such that Modifies(1, v) pattern "
            "a(v, _\"b\"_)";
    correctOutput = {"a"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "assign a; variable v; Select a such that Modifies(a, v) pattern "
            "a(v, _\"b\"_)";
    correctOutput = {"1"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "assign a; variable v; Select v such that Uses(a, v) pattern a(_, "
            "_\"b\"_)";
    correctOutput = {"b"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);
}

TEST_CASE("QPS can process queries with advanced pattern clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    AssignmentsTable *assignments = storage->getTable<AssignmentsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Variable var1 = Variable("a");
    Variable var2 = Variable("b");
    //(a+b*a)/a-b+1
    Assignment assignment1 =
        Assignment(1, "a", "(((((a)+((b)*(a)))/(a))-(b))+(1))");
    // b*a+(a-a/b)*b
    Assignment assignment2 =
        Assignment(2, "b", "(((b)*(a))+(((a)-((a)/(b)))*(b)))");
    assignments->store(&assignment1);
    assignments->store(&assignment2);
    statements->store(&line1);
    statements->store(&line2);
    variables->store(&var1);
    variables->store(&var2);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correctOutput;
    std::unordered_set<std::string> resultsSet;
    std::unordered_set<std::string> correctOutputSet;

    input = "assign a; variable v; Select a pattern a(_, _\"b*a\"_)";
    correctOutput = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "assign a; variable v; Select a pattern a(\"b\", \"b * a + (a - a "
            "/ b) * b\")";
    correctOutput = {"2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input =
        "assign a; variable v; Select v pattern a(v, _\"(a - a / b)     \"_)";
    correctOutput = {"b"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "assign a; variable v; Select v pattern a(v, _\"   1     \"_)";
    correctOutput = {"a"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);
}

TEST_CASE("QPS can process queries with procedure") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    ModifiesPTable *modifiesP = storage->getTable<ModifiesPTable>();
    UsesPTable *usesP = storage->getTable<UsesPTable>();
    CallsTable *call = storage->getTable<CallsTable>();
    CallsTTable *callT = storage->getTable<CallsTTable>();

    Procedure proc1 = Procedure("foo", 1);
    Procedure proc2 = Procedure("bar", 2);
    Procedure proc3 = Procedure("foobar", 3);
    Variable var1 = Variable("a");
    Variable var2 = Variable("b");
    Relationship<std::string, std::string> rs1 = Relationship(
        RelationshipReference::MODIFIES, std::string("foo"), std::string("a"));
    Relationship<std::string, std::string> rs2 = Relationship(
        RelationshipReference::USES, std::string("bar"), std::string("b"));
    Relationship<std::string, std::string> rs3 =
        Relationship(RelationshipReference::CALLS, std::string("foo"),
                     std::string("foobar"));
    Relationship<std::string, std::string> rs4 =
        Relationship(RelationshipReference::CALLS_T, std::string("bar"),
                     std::string("foobar"));
    procedures->store(&proc1);
    procedures->store(&proc2);
    procedures->store(&proc3);
    variables->store(&var1);
    variables->store(&var2);
    modifiesP->store(&rs1);
    usesP->store(&rs2);
    call->store(&rs3);
    callT->store(&rs4);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correctOutput;
    std::unordered_set<std::string> resultsSet;
    std::unordered_set<std::string> correctOutputSet;

    input = "procedure proc1, proc2; variable v; Select proc1 such that "
            "Calls*(proc1, proc2)";
    correctOutput = {"bar"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "procedure proc1, proc2; variable v; Select proc1 such that "
            "Calls(_, proc1)";
    correctOutput = {"foobar"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "procedure proc1, proc2; variable v; Select v such that "
            "Uses(\"bar\", v)";
    correctOutput = {"b"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "procedure proc1, proc2; variable v; Select proc1 such that "
            "Modifies(proc1, _)";
    correctOutput = {"foo"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);
}

TEST_CASE("QPS can process queries with while pattern clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    WhileControlVarTable *whiles = storage->getTable<WhileControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("z"));
    whiles->store(&test1);
    whiles->store(&test2);
    whiles->store(&test3);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correctOutput;
    std::unordered_set<std::string> resultsSet;
    std::unordered_set<std::string> correctOutputSet;

    input = "while w; variable v; Select v pattern w(v, _)";
    correctOutput = {"x", "y", "z"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "while w; variable v; Select w pattern w(_, _)";
    correctOutput = {"1", "2", "3"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "while w; variable v; Select w pattern w(\"y\", _)";
    correctOutput = {"2"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);
}

TEST_CASE("QPS can process queries with if pattern clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    IfControlVarTable *ifs = storage->getTable<IfControlVarTable>();

    Relationship<int, std::string> test1 =
        Relationship(RelationshipReference::USES, 1, std::string("x"));
    Relationship<int, std::string> test2 =
        Relationship(RelationshipReference::USES, 2, std::string("y"));
    Relationship<int, std::string> test3 =
        Relationship(RelationshipReference::USES, 3, std::string("z"));
    ifs->store(&test1);
    ifs->store(&test2);
    ifs->store(&test3);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correctOutput;
    std::unordered_set<std::string> resultsSet;
    std::unordered_set<std::string> correctOutputSet;

    input = "if ifs; variable v; Select ifs pattern ifs(v, _, _)";
    correctOutput = {"1", "2", "3"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "if ifs; variable v; Select v pattern ifs(v, _, _)";
    correctOutput = {"x", "y", "z"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);

    input = "if ifs; variable v; Select ifs pattern ifs(\"z\", _, _)";
    correctOutput = {"3"};
    results = {};
    qps.evaluate(input, results);
    resultsSet.clear();
    correctOutputSet.clear();
    resultsSet.insert(results.begin(), results.end());
    correctOutputSet.insert(correctOutput.begin(), correctOutput.end());
    REQUIRE(resultsSet == correctOutputSet);
}
