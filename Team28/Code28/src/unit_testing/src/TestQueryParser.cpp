#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    SolvableQuery solvableQ = QueryParser::parse("Assign a; Constant c; Variable v; Select v such that Modifies(1, v) pattern a(v, _)");

    REQUIRE(solvableQ.selectType.entity == DesignEntity::VARIABLE);
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