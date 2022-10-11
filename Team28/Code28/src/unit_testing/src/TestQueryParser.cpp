#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"
#include "SP/SP.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    SolvableQuery solvableQ =
        QueryParser::parse("assign a; constant c; variable v; Select a such "
                           "that Modifies(1, v) pattern a(v, _\"x\"_)");

    REQUIRE(solvableQ.selectClause.syns[0].entity == EntityName::ASSIGN);
    REQUIRE_THROWS(
        QueryParser::parse("assign a; constant c; variable v; Select a;"));
    REQUIRE_THROWS(
        QueryParser::parse("assign a; constant c; variable v; Select a such "
                           "that Modifies(1, v) pattern a(v, _\"x\"_);"));
}

TEST_CASE("QueryParser can parse declaration correctly") {
    std::vector<std::string> clauses = {
        "assign a", "constant c", "variable v",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    Declaration declare = QueryParser::parseDeclaration(clauses);
    REQUIRE(declare.syns.size() == 3);

    std::vector<std::string> multiple_declaration_clauses = {
        "assign a, b, a2", "constant c, constant, yey", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    Declaration multiple_declare =
        QueryParser::parseDeclaration(multiple_declaration_clauses);
    REQUIRE(multiple_declare.syns.size() == 9);

    std::vector<std::string> syntax_clauses = {
        "assign a, b, a2,", "constant c, constant, yey", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    REQUIRE_THROWS(QueryParser::parseDeclaration(syntax_clauses));

    std::vector<std::string> semantic_clauses = {
        "assign a, b, a2", "constant c, constant, b", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    REQUIRE_THROWS(QueryParser::parseDeclaration(semantic_clauses));

    std::vector<std::string> invalid_name_start_clauses = {
        "assign 1, b, a2", "constant c, constant, sup", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    REQUIRE_THROWS(QueryParser::parseDeclaration(invalid_name_start_clauses));

    std::vector<std::string> invalid_name_special_char_clauses = {
        "assign a1, b, a2", "constant c, constant, z*", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    REQUIRE_THROWS(
        QueryParser::parseDeclaration(invalid_name_special_char_clauses));
}

TEST_CASE("QueryParser can parse select clause") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};

    std::string correct = "Select v";
    SelectClause selectClause = QueryParser::parseSelectClause(&correct, syns);
    Synonym selectedSynonym = selectClause.syns[0];
    REQUIRE(selectedSynonym.entity == EntityName::VARIABLE);
    REQUIRE(selectedSynonym.name == "v");

    std::string misspelled = "Selct v";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&misspelled, syns));

    std::string missing_synonym = "Select";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&missing_synonym, syns));
}

TEST_CASE("QueryParser can parse boolean select clause") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};

    std::string correct = "Select BOOLEAN";
    SelectClause selectClause = QueryParser::parseSelectClause(&correct, syns);
    REQUIRE(selectClause.selectType == SelectType::BOOLEAN);
    REQUIRE(selectClause.syns.size() == 0);

    std::string misspelled = "Select BOOLAN";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&misspelled, syns));

    std::string misspelled2 = "Select BoOlEaN";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&misspelled2, syns));
}

TEST_CASE("QueryParser can parse tuple select clause") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::ASSIGN, "a1"), Synonym(EntityName::ASSIGN, "a2"),
        Synonym(EntityName::PROCEDURE, "p")};

    std::string correct = "Select <v, a>";
    SelectClause selectClause = QueryParser::parseSelectClause(&correct, syns);
    REQUIRE(selectClause.selectType == SelectType::TUPLE);
    REQUIRE(selectClause.syns.size() == 2);

    std::string many = "Select <v, a, p, a1, a2>";
    SelectClause selectClauseMany = QueryParser::parseSelectClause(&many, syns);
    REQUIRE(selectClauseMany.selectType == SelectType::TUPLE);
    REQUIRE(selectClauseMany.syns.size() == 5);

    std::string one = "Select <v>";
    SelectClause selectClauseOne = QueryParser::parseSelectClause(&one, syns);
    REQUIRE(selectClauseOne.selectType == SelectType::TUPLE);
    REQUIRE(selectClauseOne.syns.size() == 1);

    std::string duplicate = "Select <v, a1, a1>";
    SelectClause selectClauseDuplicate =
        QueryParser::parseSelectClause(&duplicate, syns);
    REQUIRE(selectClauseDuplicate.selectType == SelectType::TUPLE);
    REQUIRE(selectClauseDuplicate.syns.size() == 3);

    std::string wrong_syntax = "Select <v, a1, <a2>>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrong_syntax, syns));

    std::string wrong_syntax2 = "Select <v, a1, <a2>, p>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrong_syntax2, syns));

    std::string wrong_syntax3 = "Select <v, <a2, p>, a1>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrong_syntax3, syns));
}

TEST_CASE("Parser can parse such that clause") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};
    std::string correct_input_num_syn = "such that Modifies(1, v)";
    std::vector<SuchThatClause> clause1;
    QueryParser::parseSuchThatClause(&correct_input_num_syn, syns, &clause1);
    REQUIRE(clause1.size() == 1);
    REQUIRE(clause1[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause1[0].refLeft.isSynonym == false);
    REQUIRE(clause1[0].refLeft.value.value == "1");
    REQUIRE(clause1[0].refRight.isSynonym == true);
    REQUIRE(clause1[0].refRight.syn.name == "v");

    std::string correct_input_syn_name = "such that Modifies(a, \"yey\")";
    std::vector<SuchThatClause> clause2;
    QueryParser::parseSuchThatClause(&correct_input_syn_name, syns, &clause2);
    REQUIRE(clause2.size() == 1);
    REQUIRE(clause2[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause2[0].refLeft.isSynonym == true);
    REQUIRE(clause2[0].refLeft.syn.name == "a");
    REQUIRE(clause2[0].refRight.isSynonym == false);
    REQUIRE(clause2[0].refRight.value.value == "yey");

    std::string correct_input_num_name = "such that Modifies(1, \"yey\")";
    std::vector<SuchThatClause> clause3;
    QueryParser::parseSuchThatClause(&correct_input_num_name, syns, &clause3);
    REQUIRE(clause3.size() == 1);
    REQUIRE(clause3[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause3[0].refLeft.isSynonym == false);
    REQUIRE(clause3[0].refLeft.value.value == "1");
    REQUIRE(clause3[0].refRight.isSynonym == false);
    REQUIRE(clause3[0].refRight.value.value == "yey");

    std::string correct_input_syn_syn = "such that Modifies(a, v)";
    std::vector<SuchThatClause> clause4;
    QueryParser::parseSuchThatClause(&correct_input_syn_syn, syns, &clause4);
    REQUIRE(clause4.size() == 1);
    REQUIRE(clause4[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause4[0].refLeft.isSynonym == true);
    REQUIRE(clause4[0].refLeft.syn.name == "a");
    REQUIRE(clause4[0].refRight.isSynonym == true);
    REQUIRE(clause4[0].refRight.syn.name == "v");

    std::string correct_input_syn_wildcard = "such that Modifies(a, _)";
    std::vector<SuchThatClause> clause5;
    QueryParser::parseSuchThatClause(&correct_input_syn_wildcard, syns,
                                     &clause5);
    REQUIRE(clause5.size() == 1);
    REQUIRE(clause5[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause5[0].refLeft.isSynonym == true);
    REQUIRE(clause5[0].refLeft.syn.name == "a");
    REQUIRE(clause5[0].refRight.isSynonym == false);
    REQUIRE(clause5[0].refRight.value.value == "_");

    std::string correct_input_wildcard_syn = "such that Follows(_, a)";
    std::vector<SuchThatClause> clause6;
    QueryParser::parseSuchThatClause(&correct_input_wildcard_syn, syns,
                                     &clause6);
    REQUIRE(clause6.size() == 1);
    REQUIRE(clause6[0].relationship == RelationshipReference::FOLLOWS);
    REQUIRE(clause6[0].refLeft.isSynonym == false);
    REQUIRE(clause6[0].refLeft.value.value == "_");
    REQUIRE(clause6[0].refRight.isSynonym == true);
    REQUIRE(clause6[0].refRight.syn.name == "a");

    std::string correct_input_wildcard_syn_star = "such that Follows*(_, a)";
    std::vector<SuchThatClause> clause7;
    QueryParser::parseSuchThatClause(&correct_input_wildcard_syn_star, syns,
                                     &clause7);
    REQUIRE(clause7.size() == 1);
    REQUIRE(clause7[0].relationship == RelationshipReference::FOLLOWS_T);
    REQUIRE(clause7[0].refLeft.isSynonym == false);
    REQUIRE(clause7[0].refLeft.value.value == "_");
    REQUIRE(clause7[0].refRight.isSynonym == true);
    REQUIRE(clause7[0].refRight.syn.name == "a");

    std::vector<SuchThatClause> ignore;
    std::string extra_bracket = "such that Modifies((1, v)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&extra_bracket, syns, &ignore));

    std::string undeclared_syn = "such that Modifies(1, p)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&undeclared_syn, syns, &ignore));

    std::string modifies_wildcard = "such that Modifies(_, v)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&modifies_wildcard, syns, &ignore));

    std::string uses_wildcard = "such that Uses(_, v)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&uses_wildcard, syns, &ignore));

    std::string missing_bracket = "such that Modifies(1, v";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&missing_bracket, syns, &ignore));

    std::string misspelled = "variable v; Select v suxh that Modifies(1, v)";
    REQUIRE_THROWS(QueryParser::parse(misspelled));

    std::string too_many_arguments = "such that Modifies(1, v, d)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&too_many_arguments, syns, &ignore));

    std::string too_few_arguments = "such that Modifies(1)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&too_few_arguments, syns, &ignore));

    std::string missing_relationship_name = "such that (1, v)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&missing_relationship_name,
                                                    syns, &ignore));
}

TEST_CASE("Parser can parse such that clause with procedure") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::PROCEDURE, "proc")};
    std::string correct_input1 = "such that Modifies(proc, v)";
    std::vector<SuchThatClause> clause1;
    QueryParser::parseSuchThatClause(&correct_input1, syns, &clause1);
    REQUIRE(clause1.size() == 1);
    REQUIRE(clause1[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause1[0].refLeft.isSynonym == true);
    REQUIRE(clause1[0].refLeft.syn.name == "proc");
    REQUIRE(clause1[0].refRight.isSynonym == true);
    REQUIRE(clause1[0].refRight.syn.name == "v");

    std::string correct_input2 = "such that Uses(proc, \"x\")";
    std::vector<SuchThatClause> clause2;
    QueryParser::parseSuchThatClause(&correct_input2, syns, &clause2);
    REQUIRE(clause2.size() == 1);
    REQUIRE(clause2[0].relationship == RelationshipReference::USES);
    REQUIRE(clause2[0].refLeft.isSynonym == true);
    REQUIRE(clause2[0].refLeft.syn.name == "proc");
    REQUIRE(clause2[0].refRight.isSynonym == false);
    REQUIRE(clause2[0].refRight.value.value == "x");

    std::string correct_input3 = "such that Calls*(proc, _)";
    std::vector<SuchThatClause> clause3;
    QueryParser::parseSuchThatClause(&correct_input3, syns, &clause3);
    REQUIRE(clause3.size() == 1);
    REQUIRE(clause3[0].relationship == RelationshipReference::CALLS_T);
    REQUIRE(clause3[0].refLeft.isSynonym == true);
    REQUIRE(clause3[0].refLeft.syn.name == "proc");
    REQUIRE(clause3[0].refRight.isWildcard() == true);
}

TEST_CASE("Parser can detect semantic error of using wrong type of synonym and "
          "reference type") {
    std::vector<Synonym> syns{
        Synonym(EntityName::CONSTANT, "c"), Synonym(EntityName::VARIABLE, "v1"),
        Synonym(EntityName::VARIABLE, "v2"), Synonym(EntityName::ASSIGN, "a")};
    std::vector<SuchThatClause> ignore_suchThat;
    std::vector<PatternClause> ignore_pattern;
    std::string constant_used_in_follow_left = "such that Follow(c, \"1\")";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(
        &constant_used_in_follow_left, syns, &ignore_suchThat));

    std::string constant_used_in_uses_right = "such that Uses(a, c)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(
        &constant_used_in_uses_right, syns, &ignore_suchThat));

    std::string wrong_synonym_in_uses_left = "such that Uses(v1, v2)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&wrong_synonym_in_uses_left,
                                                    syns, &ignore_suchThat));

    std::string wrong_pattern_assign_synonym = "pattern a(c, _\"x\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(
        &wrong_pattern_assign_synonym, syns, &ignore_pattern));

    std::string wrong_pattern_assign_ref_type = "pattern a(\"1\", _\"x\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(
        &wrong_pattern_assign_ref_type, syns, &ignore_pattern));
}

TEST_CASE("Parser can parse pattern clause") {
    std::string correct_input = "pattern a(v, _\"x\"_)";
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};
    std::vector<PatternClause> clause;
    QueryParser::parsePatternClause(&correct_input, syns, &clause);
    REQUIRE(clause.size() == 1);
    REQUIRE(clause[0].entRef.isSynonym == true);
    REQUIRE(clause[0].entRef.syn.name == "v");
    REQUIRE(clause[0].entRef.syn.entity == EntityName::VARIABLE);
    REQUIRE(clause[0].syn.name == "a");
    REQUIRE(clause[0].syn.entity == EntityName::ASSIGN);
    REQUIRE(clause[0].expression == "(x)");
    REQUIRE(clause[0].isExact == false);

    std::string correct_input_exact = "pattern a(v, \"x\")";
    std::vector<PatternClause> clause_exact;
    QueryParser::parsePatternClause(&correct_input_exact, syns, &clause_exact);
    REQUIRE(clause_exact.size() == 1);
    REQUIRE(clause_exact[0].entRef.isSynonym == true);
    REQUIRE(clause_exact[0].entRef.syn.name == "v");
    REQUIRE(clause_exact[0].entRef.syn.entity == EntityName::VARIABLE);
    REQUIRE(clause_exact[0].syn.name == "a");
    REQUIRE(clause_exact[0].syn.entity == EntityName::ASSIGN);
    REQUIRE(clause_exact[0].expression == "(x)");
    REQUIRE(clause_exact[0].isExact == true);

    std::string correct_input_space = "pattern a(v, _    \"    x    \"    _)";
    std::vector<PatternClause> clause_space;
    QueryParser::parsePatternClause(&correct_input_space, syns, &clause_space);
    REQUIRE(clause_space[0].expression == "(x)");
    REQUIRE(clause_space[0].isExact == false);

    std::vector<PatternClause> ignore_pattern;
    std::string extra_bracket = "pattern a((v, \"x\")";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&extra_bracket, syns, &ignore_pattern));

    std::string undeclared_syn = "pattern a(p, \"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&undeclared_syn, syns,
                                                   &ignore_pattern));

    std::string misspelled = "assign a; Select a pattrn a(\"x\", _)";
    REQUIRE_THROWS(QueryParser::parse(misspelled));

    std::string too_many_arguments = "pattern a(v, _, a)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&too_many_arguments, syns,
                                                   &ignore_pattern));

    std::string too_few_arguments = "pattern a(v)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&too_few_arguments, syns,
                                                   &ignore_pattern));

    std::string missing_quotation = "pattern a(v, \"x)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&missing_quotation, syns,
                                                   &ignore_pattern));

    std::string missing_underscore = "pattern a(v, _\"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&missing_underscore, syns,
                                                   &ignore_pattern));

    std::string wrong_expression_syntax = "pattern a(c, _\"+expression+hi\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&wrong_expression_syntax,
                                                   syns, &ignore_pattern));

    std::string missing_bracket_left = "pattern a(v, _\"(x\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&missing_bracket_left, syns,
                                                   &ignore_pattern));

    std::string missing_bracket_left2 = "pattern a(v, \"(x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&missing_bracket_left2, syns,
                                                   &ignore_pattern));

    std::string missing_bracket_right = "pattern a(v, _\"x)\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&missing_bracket_right, syns,
                                                   &ignore_pattern));

    std::string missing_bracket_right2 = "pattern a(v, \"x)\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&missing_bracket_right2,
                                                   syns, &ignore_pattern));
}

TEST_CASE("Parser can parse multiple such that clauses") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::STMT, "s"), Synonym(EntityName::READ, "r"),
        Synonym(EntityName::ASSIGN, "a1")};
    std::string input =
        "such that Modifies(a, _) pattern a(_, _) such that Follows(s, r) such "
        "that Uses(s, v) pattern a1(v, _\"weew\"_)";
    std::string remaining_input =
        " pattern a(_, _) such that Follows(s, r) such that Uses(s, v) pattern "
        "a1(v, _\"weew\"_)";
    std::vector<SuchThatClause> clauses;
    QueryParser::parseSuchThatClause(&input, syns, &clauses);
    REQUIRE(clauses.size() == 1);
    REQUIRE(clauses[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(input == remaining_input);
}

TEST_CASE("Parser can parse multiple pattern clauses") {
    std::vector<Synonym> syns{
        Synonym(EntityName::ASSIGN, "a1"), Synonym(EntityName::ASSIGN, "a2"),
        Synonym(EntityName::VARIABLE, "v1"), Synonym(EntityName::STMT, "s1"),
        Synonym(EntityName::STMT, "s2")};
    std::string input =
        "pattern a1(v1, _\"yey\"_) such that Follows(s1, s2) such "
        "that Uses(s2, v1) pattern a2(v1, _\"weew\"_)";
    std::string remaining_input =
        " such that Follows(s1, s2) such "
        "that Uses(s2, v1) pattern a2(v1, _\"weew\"_)";
    std::vector<PatternClause> clauses;
    QueryParser::parsePatternClause(&input, syns, &clauses);
    REQUIRE(clauses.size() == 1);
    REQUIRE(input == remaining_input);
}
TEST_CASE("Parser can parse multiple such that clauses with explicit and") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::STMT, "s"), Synonym(EntityName::READ, "r"),
        Synonym(EntityName::ASSIGN, "a1")};
    std::string input =
        "such that Modifies(a, _) and Follows(s, r) pattern a(_, _) such "
        "that Uses(s, v) pattern a1(v, _\"weew\"_)";
    std::string remaining_input = " pattern a(_, _) such "
                                  "that Uses(s, v) pattern a1(v, _\"weew\"_)";
    std::vector<SuchThatClause> clauses;
    QueryParser::parseSuchThatClause(&input, syns, &clauses);
    REQUIRE(clauses.size() == 2);
    REQUIRE(clauses[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clauses[1].relationship == RelationshipReference::FOLLOWS);
    REQUIRE(input == remaining_input);
}

TEST_CASE("Parser can parse multiple pattern clauses with explicit and") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::STMT, "s"), Synonym(EntityName::READ, "r"),
        Synonym(EntityName::ASSIGN, "a1")};
    std::string input = "pattern a(_, _) and a1(v, _\"weew\"_) such that "
                        "Modifies(a, _) and Follows(s, r) such "
                        "that Uses(s, v)";
    std::string remaining_input =
        " such that Modifies(a, _) and Follows(s, r) such "
        "that Uses(s, v)";
    std::vector<PatternClause> clauses;
    QueryParser::parsePatternClause(&input, syns, &clauses);
    REQUIRE(clauses.size() == 2);
    REQUIRE(input == remaining_input);
}
TEST_CASE("Parser can catch mismatch in explicit and") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::STMT, "s"), Synonym(EntityName::READ, "r"),
        Synonym(EntityName::ASSIGN, "a1")};
    std::string pattern_input = "pattern a(_, _) and Follows(s, r) such that "
                                "Modifies(a, _) and a1(v, _\"weew\"_) such "
                                "that Uses(s, v)";

    std::vector<PatternClause> pattern_clauses;
    REQUIRE_THROWS(QueryParser::parsePatternClause(&pattern_input, syns,
                                                   &pattern_clauses));
    std::string such_that_input =
        "such that Modifies(a, _) and a1(v, _\"weew\"_) pattern a(_, _) and "
        "Follows(s, r) such that Uses(s, v)";

    std::vector<SuchThatClause> such_that_clauses;
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&such_that_input, syns,
                                                    &such_that_clauses));
}

TEST_CASE("Parser can parse while pattern clauses") {
    std::vector<Synonym> syns{Synonym(EntityName::WHILE, "w"),
                              Synonym(EntityName::WHILE, "w1"),
                              Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::VARIABLE, "v2")};

    std::string correct_input = "pattern w(v, _)";
    std::vector<PatternClause> clause;
    QueryParser::parsePatternClause(&correct_input, syns, &clause);
    REQUIRE(clause[0].expression == "_");
    REQUIRE(clause[0].patternType == PatternType::WHILE);

    std::string correct_input_space = "pattern w(_,    _      )";
    std::vector<PatternClause> clause_space;
    QueryParser::parsePatternClause(&correct_input_space, syns, &clause_space);
    REQUIRE(clause_space[0].expression == "_");
    REQUIRE(clause_space[0].patternType == PatternType::WHILE);

    std::string non_wild_card_expression = "pattern w(v, \"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&non_wild_card_expression,
                                                   syns, &clause));
}

TEST_CASE("Parser can parse if pattern clauses") {
    std::vector<Synonym> syns{Synonym(EntityName::IF, "ifs"),
                              Synonym(EntityName::IF, "ifs2"),
                              Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::VARIABLE, "v2")};

    std::string correct_input = "pattern ifs(v, _, _)";
    std::vector<PatternClause> clause;
    QueryParser::parsePatternClause(&correct_input, syns, &clause);
    REQUIRE(clause[0].expression == "_");
    REQUIRE(clause[0].patternType == PatternType::IF);

    std::string correct_input_space =
        "pattern ifs(   _    ,   _   ,     _    )";
    std::vector<PatternClause> clause_space;
    QueryParser::parsePatternClause(&correct_input_space, syns, &clause_space);
    REQUIRE(clause_space[0].expression == "_");
    REQUIRE(clause_space[0].patternType == PatternType::IF);

    std::string non_wild_card_expression_left = "pattern ifs(v, \"x\", _)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(
        &non_wild_card_expression_left, syns, &clause));

    std::string non_wild_card_expression_right = "pattern ifs(v, _, \"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(
        &non_wild_card_expression_left, syns, &clause));
}
