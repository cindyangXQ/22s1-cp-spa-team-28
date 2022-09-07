#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    SolvableQuery solvableQ = QueryParser::parse("assign a; constant c; variable v; Select v such that Modifies(1, v) pattern a(v, _)");

    REQUIRE(solvableQ.selectType.entity == DesignEntity::VARIABLE);
}

TEST_CASE("QueryParser can parse declaration correctly") {
	std::vector<std::string> clauses = { "assign a","constant c", "variable v", "Select v such that Modifies(1, v) pattern a(v, _)" };
	Declaration declare = QueryParser::parseDeclaration(clauses);
	REQUIRE(declare.syns.size() == 3);

	std::vector<std::string> multiple_declaration_clauses = { "assign a, b, a2","constant c, constant, yey", "variable v, v2, v3", "Select v such that Modifies(1, v) pattern a(v, _)" };
	Declaration multiple_declare = QueryParser::parseDeclaration(multiple_declaration_clauses);
	REQUIRE(multiple_declare.syns.size() == 9);

	std::vector<std::string> syntax_clauses = { "assign a, b, a2,","constant c, constant, yey", "variable v, v2, v3", "Select v such that Modifies(1, v) pattern a(v, _)" };
	REQUIRE_THROWS(QueryParser::parseDeclaration(syntax_clauses));

	std::vector<std::string> semantic_clauses = { "assign a, b, a2","constant c, constant, b", "variable v, v2, v3", "Select v such that Modifies(1, v) pattern a(v, _)" };
	REQUIRE_THROWS(QueryParser::parseDeclaration(semantic_clauses));

	std::vector<std::string> invalid_name_start_clauses = { "assign 1, b, a2","constant c, constant, sup", "variable v, v2, v3", "Select v such that Modifies(1, v) pattern a(v, _)" };
	REQUIRE_THROWS(QueryParser::parseDeclaration(invalid_name_start_clauses));

	std::vector<std::string> invalid_name_special_char_clauses = { "assign a1, b, a2","constant c, constant, z*", "variable v, v2, v3", "Select v such that Modifies(1, v) pattern a(v, _)" };
	REQUIRE_THROWS(QueryParser::parseDeclaration(invalid_name_special_char_clauses));
}

TEST_CASE("Parser can parse such that clause") {
	std::string correct_input = "such that Modifies(1,v) pattern a(_, x)";
	std::vector<Synonym> syns{ Synonym(DesignEntity::VARIABLE, "v"), Synonym(DesignEntity::ASSIGN, "a") };
	SuchThatClause clause = QueryParser::parseSuchThatClause(correct_input, syns);
	/*
	// correct relationship type extracted
	REQUIRE(clause.relationshipType == RelRef::Modifies);
	// correct left operand extracted
	REQUIRE(clause.refLeft.stmtNum == 1);
	// correct relationship type extracted
	REQUIRE(clause.refRight.name ==  "v");
	*/
	std::string extra_bracket = "such that Modifies((1, v) pattern a(_, x)";
	REQUIRE_THROWS(QueryParser::parseSuchThatClause(extra_bracket, syns));

	std::string undeclared_syn = "such that Modifies(1, p) pattern a(_, x)";
	REQUIRE_THROWS(QueryParser::parseSuchThatClause(undeclared_syn, syns));
}

TEST_CASE("Parser can parse pattern clause") {
    std::string correct_input = "such that Modifies(1,v) pattern a(v, \"x\")";
    std::vector<Synonym> syns{ Synonym(DesignEntity::VARIABLE, "v"), Synonym(DesignEntity::ASSIGN, "a") };
    PatternClause clause = QueryParser::parsePatternClause(correct_input, syns);

    std::string extra_bracket = "such that Modifies(1, v) pattern a((v, \"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(extra_bracket, syns));

    std::string undeclared_syn = "such that Modifies(1, v) pattern a(p, \"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(undeclared_syn, syns));
}