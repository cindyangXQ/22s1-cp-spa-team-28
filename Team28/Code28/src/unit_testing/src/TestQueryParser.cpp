#include "QPS/QueryParser.h"
#include "QPS/SolvableQuery.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    SolvableQuery solvableQ = QueryParser::parse("Assign a; Constant c; Variable v; Select v such that Modifies(1, v) pattern a(v, _)");
    solvableQ.evaluate();
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