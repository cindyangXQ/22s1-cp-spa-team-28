#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    SolvableQuery solvableQ =
        QueryParser::parse("assign a; constant c; variable v; Select a such "
                           "that Modifies(1, v) pattern a(v, _\"x\"_)");

    REQUIRE(solvableQ.selectType.entity == EntityName::ASSIGN);
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
    SelectType selectedSynonym = QueryParser::parseSelectClause(&correct, syns);
    REQUIRE(selectedSynonym.entity == EntityName::VARIABLE);
    REQUIRE(selectedSynonym.name == "v");

    std::string misspelled = "Selct v";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&misspelled, syns));

    std::string missing_synonym = "Select";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&missing_synonym, syns));
}

TEST_CASE("Parser can parse such that clause") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};
    std::string correct_input_num_syn = "such that Modifies(1, v)";
    SuchThatClause clause1 =
        QueryParser::parseSuchThatClause(&correct_input_num_syn, syns);
    REQUIRE(clause1.relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause1.refLeft.isSynonym == false);
    REQUIRE(clause1.refLeft.value.value == "1");
    REQUIRE(clause1.refRight.isSynonym == true);
    REQUIRE(clause1.refRight.syn.name == "v");

    std::string correct_input_syn_name = "such that Modifies(a, \"yey\")";
    SuchThatClause clause2 =
        QueryParser::parseSuchThatClause(&correct_input_syn_name, syns);
    REQUIRE(clause2.relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause2.refLeft.isSynonym == true);
    REQUIRE(clause2.refLeft.syn.name == "a");
    REQUIRE(clause2.refRight.isSynonym == false);
    REQUIRE(clause2.refRight.value.value == "yey");

    std::string correct_input_num_name = "such that Modifies(1, \"yey\")";
    SuchThatClause clause3 =
        QueryParser::parseSuchThatClause(&correct_input_num_name, syns);
    REQUIRE(clause3.relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause3.refLeft.isSynonym == false);
    REQUIRE(clause3.refLeft.value.value == "1");
    REQUIRE(clause3.refRight.isSynonym == false);
    REQUIRE(clause3.refRight.value.value == "yey");

    std::string correct_input_syn_syn = "such that Modifies(a, v)";
    SuchThatClause clause4 =
        QueryParser::parseSuchThatClause(&correct_input_syn_syn, syns);
    REQUIRE(clause4.relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause4.refLeft.isSynonym == true);
    REQUIRE(clause4.refLeft.syn.name == "a");
    REQUIRE(clause4.refRight.isSynonym == true);
    REQUIRE(clause4.refRight.syn.name == "v");

    std::string correct_input_syn_wildcard = "such that Modifies(a, _)";
    SuchThatClause clause5 =
        QueryParser::parseSuchThatClause(&correct_input_syn_wildcard, syns);
    REQUIRE(clause5.relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause5.refLeft.isSynonym == true);
    REQUIRE(clause5.refLeft.syn.name == "a");
    REQUIRE(clause5.refRight.isSynonym == false);
    REQUIRE(clause5.refRight.value.value == "_");

    std::string correct_input_wildcard_syn = "such that Follows(_, a)";
    SuchThatClause clause6 =
        QueryParser::parseSuchThatClause(&correct_input_wildcard_syn, syns);
    REQUIRE(clause6.relationship == RelationshipReference::FOLLOWS);
    REQUIRE(clause6.refLeft.isSynonym == false);
    REQUIRE(clause6.refLeft.value.value == "_");
    REQUIRE(clause6.refRight.isSynonym == true);
    REQUIRE(clause6.refRight.syn.name == "a");

    std::string correct_input_wildcard_syn_star = "such that Follows*(_, a)";
    SuchThatClause clause7 = QueryParser::parseSuchThatClause(
        &correct_input_wildcard_syn_star, syns);
    REQUIRE(clause7.relationship == RelationshipReference::FOLLOWS_T);
    REQUIRE(clause7.refLeft.isSynonym == false);
    REQUIRE(clause7.refLeft.value.value == "_");
    REQUIRE(clause7.refRight.isSynonym == true);
    REQUIRE(clause7.refRight.syn.name == "a");

    std::string extra_bracket = "such that Modifies((1, v)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&extra_bracket, syns));

    std::string undeclared_syn = "such that Modifies(1, p)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&undeclared_syn, syns));

    std::string modifies_wildcard = "such that Modifies(_, v)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&modifies_wildcard, syns));

    std::string uses_wildcard = "such that Uses(_, v)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&uses_wildcard, syns));

    std::string missing_bracket = "such that Modifies(1, v";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&missing_bracket, syns));

    std::string misspelled = "variable v; Select v suxh that Modifies(1, v)";
    REQUIRE_THROWS(QueryParser::parse(misspelled));

    std::string too_many_arguments = "such that Modifies(1, v, d)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&too_many_arguments, syns));

    std::string too_few_arguments = "such that Modifies(1)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&too_few_arguments, syns));

    std::string missing_relationship_name = "such that (1, v)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&missing_relationship_name, syns));
}

TEST_CASE("Parser can detect semantic error of using wrong type of synonym and reference type") {
    std::vector<Synonym> syns{Synonym(EntityName::CONSTANT, "c"),
                              Synonym(EntityName::VARIABLE, "v1"),
                              Synonym(EntityName::VARIABLE, "v2"),
                              Synonym(EntityName::ASSIGN, "a")};

    std::string constant_used_in_follow_left = "such that Follow(c, \"1\")";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&constant_used_in_follow_left, syns));

    std::string constant_used_in_uses_right = "such that Uses(a, c)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&constant_used_in_uses_right, syns));

    std::string wrong_synonym_in_uses_left = "such that Uses(v1, v2)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&wrong_synonym_in_uses_left, syns));

    std::string wrong_pattern_assign_synonym = "pattern a(c, _\"x\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&wrong_pattern_assign_synonym, syns));

    std::string wrong_pattern_assign_ref_type = "pattern a(\"1\", _\"x\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&wrong_pattern_assign_ref_type, syns));
}

TEST_CASE("Parser can parse pattern clause") {
    std::string correct_input = "pattern a(v, _\"x\"_)";
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};
    PatternClause clause =
        QueryParser::parsePatternClause(&correct_input, syns);
    REQUIRE(clause.entRef.isSynonym == true);
    REQUIRE(clause.entRef.syn.name == "v");
    REQUIRE(clause.entRef.syn.entity == EntityName::VARIABLE);
    REQUIRE(clause.syn.name == "a");
    REQUIRE(clause.syn.entity == EntityName::ASSIGN);
    REQUIRE(clause.expression == "(x)");

    std::string extra_bracket = "pattern a((v, \"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&extra_bracket, syns));

    std::string undeclared_syn = "pattern a(p, \"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&undeclared_syn, syns));

    std::string misspelled = "assign a; Select a pattrn a(\"x\", _)";
    REQUIRE_THROWS(QueryParser::parse(misspelled));

    std::string too_many_arguments = "pattern a(v, _, c)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&too_many_arguments, syns));

    std::string too_few_arguments = "pattern a(c)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&too_few_arguments, syns));

    std::string missing_quotation = "pattern a(c, \"x)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&missing_quotation, syns));

    std::string missing_underscore = "pattern a(c, _\"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&missing_underscore, syns));

    std::string wrong_expression_syntax = "pattern a(c, _\"+x\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&wrong_expression_syntax, syns));
}
