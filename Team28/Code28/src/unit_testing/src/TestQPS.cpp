#include "PKB/Facades/QueryFacade.h"
#include "QPS/QPS.h"
#include "SP/SP.h"

#include "catch.hpp"

TEST_CASE("QPS can process simple queries to select statements") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "stmt s; Select s";
    std::string output = qps.processQuery(input);
    REQUIRE(output == "1, 2");
}

TEST_CASE("QPS can process simple queries to select variables") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "variable v; Select v";
    std::string output = qps.processQuery(input);
    REQUIRE(output == "test1, test2");
}

TEST_CASE("QPS can process simple queries to select constants") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    ConstantsTable *constants =
        (ConstantsTable *)storage.getTable(TableName::CONSTANTS);
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    constants->store(&test1);
    constants->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "constant c; Select c";
    std::string output = qps.processQuery(input);
    REQUIRE(output == "test1, test2");
}

TEST_CASE("QPS can process simple queries to select procedures") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    ProceduresTable *procedures =
        (ProceduresTable *)storage.getTable(TableName::PROCEDURES);
    Procedure test1 = Procedure("test1");
    Procedure test2 = Procedure("test2");

    procedures->store(&test1);
    procedures->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "procedure p; Select p";
    std::string output = qps.processQuery(input);
    REQUIRE(output == "test1, test2");
}

TEST_CASE("QPS can process simple queries to select procedures when there are "
          "no procedures") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    ProceduresTable *procedures =
        (ProceduresTable *)storage.getTable(TableName::PROCEDURES);

    QPS qps = QPS(&facade);

    std::string input = "procedure p; Select p";
    std::string output = qps.processQuery(input);
    REQUIRE(output.size() == 0);
}

TEST_CASE("QPS can process simple queries with semantic error") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    QPS qps = QPS(&facade);

    std::string undeclared_synonym =
        "variable v; Select v such that Modifies(1, yey)";
    REQUIRE_THROWS(qps.processQuery(undeclared_synonym));
}

TEST_CASE("QPS evaluate select statements") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "stmt s; Select s";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correct_output{"1", "2"};
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate select assign statements") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "assign s; Select s";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correct_output{"1", "2"};
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate select if statements") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    Statement test1 = Statement(1, StatementType::IF);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "if s; Select s";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correct_output{"1"};
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate select variables") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "variable v; Select v";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correct_output{"test1", "test2"};
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate select constants") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    ConstantsTable *constants =
        (ConstantsTable *)storage.getTable(TableName::CONSTANTS);
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    constants->store(&test1);
    constants->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "constant c; Select c";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correct_output{"test1", "test2"};
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate select procedures") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    ProceduresTable *procedures =
        (ProceduresTable *)storage.getTable(TableName::PROCEDURES);
    Procedure test1 = Procedure("test1");
    Procedure test2 = Procedure("test2");

    procedures->store(&test1);
    procedures->store(&test2);

    QPS qps = QPS(&facade);

    std::string input = "procedure p; Select p";
    std::list<std::string> results;
    qps.evaluate(input, results);
    std::list<std::string> correct_output{"test1", "test2"};
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS can evaluate select procedures when there are no procedures") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    ProceduresTable *procedures =
        (ProceduresTable *)storage.getTable(TableName::PROCEDURES);

    QPS qps = QPS(&facade);

    std::string input = "procedure p; Select p";
    std::list<std::string> results;
    qps.evaluate(input, results);
    REQUIRE(results.size() == 0);
}

TEST_CASE("QPS evaluate syntax error") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    QPS qps = QPS(&facade);

    std::string extra_bracket =
        "variable v; Select v such that Modifies((1, v)";
    std::list<std::string> results;
    qps.evaluate(extra_bracket, results);
    std::list<std::string> correct_output = {"SyntaxError"};
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS evaluate semantic error") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    QPS qps = QPS(&facade);

    std::string undeclared_synonym =
        "variable v; Select v such that Modifies(1, yey)";
    std::list<std::string> results;
    qps.evaluate(undeclared_synonym, results);
    std::list<std::string> correct_output = {"SemanticError"};
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS can process queries with follows relationship") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    FollowsTable *follows =
        (FollowsTable *)storage.getTable(TableName::FOLLOWS);

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
    std::list<std::string> correct_output;

    input = "stmt s; Select s such that Follows(s, 2)";
    correct_output = {"1"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s; Select s such that Follows(1, s)";
    correct_output = {"2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s; Select s such that Follows(1, 2)";
    correct_output = {"1", "2", "3"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s; Select s such that Follows(2, 1)";
    correct_output = {};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s; Select s such that Follows(s, 1)";
    correct_output = {};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s; Select s such that Follows(_, 3)";
    correct_output = {"1", "2", "3"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s; Select s such that Follows(s, _)";
    correct_output = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s1; stmt s2; Select s1 such that Follows(s1, s2)";
    correct_output = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS can process queries with modifies relationship") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);
    ModifiesSTable *modifiesS =
        (ModifiesSTable *)storage.getTable(TableName::MODIFIES_S);

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
    std::list<std::string> correct_output;

    input = "stmt s; variable v; Select s such that Modifies(s, v)";
    correct_output = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s; variable v; Select s such that Modifies(s, \"a\")";
    correct_output = {"1"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "stmt s; variable v; Select s such that Modifies(_, \"a\")";
    results = {};
    qps.evaluate(input, results);
    correct_output = {"SemanticError"};
    REQUIRE(results == correct_output);

    input = "stmt s; variable v; Select s such that Modifies(1, \"a\")";
    correct_output = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS can process queries with pattern clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    AssignmentsTable *assignments =
        (AssignmentsTable *)storage.getTable(TableName::ASSIGNMENTS);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);

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
    std::list<std::string> correct_output;

    input = "assign a; Select a pattern a(\"a\", _)";
    correct_output = {"1"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; Select a pattern a(_, _)";
    correct_output = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; variable v; Select v pattern a(v, _)";
    correct_output = {"a", "b"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; variable v; Select a pattern a(\"b\", _\"a\"_)";
    correct_output = {"2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS can process queries with such that and pattern clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    AssignmentsTable *assignments =
        (AssignmentsTable *)storage.getTable(TableName::ASSIGNMENTS);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);
    ModifiesSTable *modifiesS =
        (ModifiesSTable *)storage.getTable(TableName::MODIFIES_S);
    ModifiesSTable *usesS =
        (ModifiesSTable *)storage.getTable(TableName::USES_S);

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
    std::list<std::string> correct_output;

    input = "assign a; variable v; Select a such that Modifies(1, \"a\") "
            "pattern a(\"b\", _\"a\"_)";
    correct_output = {"2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; variable v; Select v such that Modifies(1, v) pattern "
            "a(v, _\"b\"_)";
    correct_output = {"a"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; variable v; Select a such that Modifies(a, v) pattern "
            "a(v, _\"b\"_)";
    correct_output = {"1"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; variable v; Select v such that Uses(a, v) pattern a(_, "
            "_\"b\"_)";
    correct_output = {"b"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS can process queries with advanced pattern clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    StatementsTable *statements =
        (StatementsTable *)storage.getTable(TableName::STATEMENTS);
    AssignmentsTable *assignments =
        (AssignmentsTable *)storage.getTable(TableName::ASSIGNMENTS);
    VariablesTable *variables =
        (VariablesTable *)storage.getTable(TableName::VARIABLES);

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Variable var1 = Variable("a");
    Variable var2 = Variable("b");
    //(a+b*a)/a-b+1
    Assignment assignment1 = Assignment(1, "a", "(((((a)+((b)*(a)))/(a))-(b))+(1))");
    //b*a+(a-a/b)*b
    Assignment assignment2 = Assignment(2, "b", "(((b)*(a))+(((a)-((a)/(b)))*(b)))");
    assignments->store(&assignment1);
    assignments->store(&assignment2);
    statements->store(&line1);
    statements->store(&line2);
    variables->store(&var1);
    variables->store(&var2);

    QPS qps = QPS(&facade);
    std::string input;
    std::list<std::string> results;
    std::list<std::string> correct_output;

    input = "assign a; variable v; Select a pattern a(_, _\"b*a\"_)";
    correct_output = {"1", "2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; variable v; Select a pattern a(\"b\", \"b * a + (a - a / b) * b\")";
    correct_output = {"2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; variable v; Select v pattern a(v, _\"(a - a / b)     \"_)";
    correct_output = {"b"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "assign a; variable v; Select v pattern a(v, _\"   1     \"_)";
    correct_output = {"a"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS can process queries with while pattern clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    WhileControlVarTable *whiles =
        (WhileControlVarTable *)storage.getTable(TableName::W_CONTROL);

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
    std::list<std::string> correct_output;

    input = "while w; variable v; Select v pattern w(v, _)";
    correct_output = {"x", "y", "z"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "while w; variable v; Select w pattern w(_, _)";
    correct_output = {"1", "2", "3"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "while w; variable v; Select w pattern w(\"y\", _)";
    correct_output = {"2"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);
}

TEST_CASE("QPS can process queries with if pattern clause") {
    Storage storage;
    QueryFacade facade = QueryFacade(&storage);
    IfControlVarTable *ifs =
        (IfControlVarTable *)storage.getTable(TableName::I_CONTROL);

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
    std::list<std::string> correct_output;

    input = "if ifs; variable v; Select ifs pattern ifs(v, _, _)";
    correct_output = {"1", "2", "3"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "if ifs; variable v; Select v pattern ifs(v, _, _)";
    correct_output = {"x", "y", "z"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);

    input = "if ifs; variable v; Select ifs pattern ifs(\"z\", _, _)";
    correct_output = {"3"};
    results = {};
    qps.evaluate(input, results);
    REQUIRE(results == correct_output);
}