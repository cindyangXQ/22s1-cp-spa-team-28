#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"
#include "SP/SP.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    SolvableQuery solvableQ =
        QueryParser::parse("assign a; constant c; variable v; Select a such "
                           "that Modifies(1, v) pattern a(v, _\"x\"_)");

    REQUIRE(solvableQ.selectClause.refs[0].getEntityName() ==
            EntityName::ASSIGN);
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

    std::vector<std::string> multipleDeclarationClauses = {
        "assign a, b, a2", "constant c, constant, yey", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    Declaration multipleDeclare =
        QueryParser::parseDeclaration(multipleDeclarationClauses);
    REQUIRE(multipleDeclare.syns.size() == 9);

    std::vector<std::string> syntaxClauses = {
        "assign a, b, a2,", "constant c, constant, yey", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    REQUIRE_THROWS(QueryParser::parseDeclaration(syntaxClauses));

    std::vector<std::string> semanticClauses = {
        "assign a, b, a2", "constant c, constant, b", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    REQUIRE_THROWS(QueryParser::parseDeclaration(semanticClauses));

    std::vector<std::string> invalidNameStartClauses = {
        "assign 1, b, a2", "constant c, constant, sup", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    REQUIRE_THROWS(QueryParser::parseDeclaration(invalidNameStartClauses));

    std::vector<std::string> invalidNameSpecialCharClauses = {
        "assign a1, b, a2", "constant c, constant, z*", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    REQUIRE_THROWS(
        QueryParser::parseDeclaration(invalidNameSpecialCharClauses));
}

TEST_CASE("QueryParser can parse select clause") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};

    std::string correct = "Select v";
    SelectClause selectClause = QueryParser::parseSelectClause(&correct, syns);
    Synonym selectedSynonym = selectClause.refs[0].getSynonym();
    REQUIRE(selectedSynonym.getEntityName() == EntityName::VARIABLE);
    REQUIRE(selectedSynonym.getName() == "v");

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
    REQUIRE(selectClause.refs.size() == 0);

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
    REQUIRE(selectClause.refs.size() == 2);

    std::string many = "Select <v, a, p, a1, a2>";
    SelectClause selectClauseMany = QueryParser::parseSelectClause(&many, syns);
    REQUIRE(selectClauseMany.selectType == SelectType::TUPLE);
    REQUIRE(selectClauseMany.refs.size() == 5);

    std::string one = "Select <v>";
    SelectClause selectClauseOne = QueryParser::parseSelectClause(&one, syns);
    REQUIRE(selectClauseOne.selectType == SelectType::TUPLE);
    REQUIRE(selectClauseOne.refs.size() == 1);

    std::string duplicate = "Select <v, a1, a1>";
    SelectClause selectClauseDuplicate =
        QueryParser::parseSelectClause(&duplicate, syns);
    REQUIRE(selectClauseDuplicate.selectType == SelectType::TUPLE);
    REQUIRE(selectClauseDuplicate.refs.size() == 3);

    std::string wrongSyntax = "Select <v, a1, <a2>>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrongSyntax, syns));

    std::string wrongSyntax2 = "Select <v, a1, <a2>, p>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrongSyntax2, syns));

    std::string wrong_syntax3 = "Select <v, <a2, p>, a1>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrong_syntax3, syns));

    std::string missing_synonym = "Select";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&missing_synonym, syns));
}

TEST_CASE("QueryParser can parse select clause with attribute") {
    std::vector<Synonym> syns{
        Synonym(EntityName::CALL, "c"),  Synonym(EntityName::PROCEDURE, "proc"),
        Synonym(EntityName::PRINT, "p"), Synonym(EntityName::CONSTANT, "const"),
        Synonym(EntityName::WHILE, "w"), Synonym(EntityName::VARIABLE, "v")};

    std::string one = "Select w. stmt#";
    SelectClause selectClause = QueryParser::parseSelectClause(&one, syns);
    REQUIRE(selectClause.selectType == SelectType::SINGLE);
    REQUIRE(selectClause.refs.size() == 1);
    REQUIRE(selectClause.refs[0].getEntityName() == EntityName::WHILE);
    REQUIRE(selectClause.refs[0].getAttr() == EntityAttribute::STMT_NO);

    std::string many = "Select <const .value , p.varName, v. varName, c.stmt#>";
    SelectClause selectClauseMany = QueryParser::parseSelectClause(&many, syns);
    REQUIRE(selectClauseMany.selectType == SelectType::TUPLE);
    REQUIRE(selectClauseMany.refs.size() == 4);
    REQUIRE(selectClauseMany.refs[0].getEntityName() == EntityName::CONSTANT);
    REQUIRE(selectClauseMany.refs[0].getAttr() == EntityAttribute::VALUE);
    REQUIRE(selectClauseMany.refs[1].getEntityName() == EntityName::PRINT);
    REQUIRE(selectClauseMany.refs[1].getAttr() == EntityAttribute::VAR_NAME);
    REQUIRE(selectClauseMany.refs[2].getEntityName() == EntityName::VARIABLE);
    REQUIRE(selectClauseMany.refs[2].getAttr() == EntityAttribute::VAR_NAME);
    REQUIRE(selectClauseMany.refs[3].getEntityName() == EntityName::CALL);
    REQUIRE(selectClauseMany.refs[3].getAttr() == EntityAttribute::STMT_NO);

    std::string mixed = "Select <w , p.varName, v, c.stmt#>";
    SelectClause selectClauseMixed =
        QueryParser::parseSelectClause(&mixed, syns);
    REQUIRE(selectClauseMixed.selectType == SelectType::TUPLE);
    REQUIRE(selectClauseMixed.refs.size() == 4);

    std::string wrongAttribute = "Select v. stmt#";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrongAttribute, syns));

    std::string missingSynonym = "Select . stmt#";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&missingSynonym, syns));

    std::string missingAttribute = "constant c; Select c. ";
    REQUIRE_THROWS(QueryParser::parse(missingAttribute));
}

TEST_CASE("Parser can parse such that clause") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};
    std::string numSyn = "such that Modifies(1, v)";
    std::vector<SuchThatClause> clause1;
    QueryParser::parseSuchThatClause(&numSyn, syns, &clause1);
    REQUIRE(clause1.size() == 1);
    REQUIRE(clause1[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause1[0].refLeft.isASynonym() == false);
    REQUIRE(clause1[0].refLeft.getValueString() == "1");
    REQUIRE(clause1[0].refRight.isASynonym() == true);
    REQUIRE(clause1[0].refRight.getSynonymName() == "v");

    std::string synName = "such that Modifies(a, \"yey\")";
    std::vector<SuchThatClause> clause2;
    QueryParser::parseSuchThatClause(&synName, syns, &clause2);
    REQUIRE(clause2.size() == 1);
    REQUIRE(clause2[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause2[0].refLeft.isASynonym() == true);
    REQUIRE(clause2[0].refLeft.getSynonymName() == "a");
    REQUIRE(clause2[0].refRight.isASynonym() == false);
    REQUIRE(clause2[0].refRight.getValueString() == "yey");

    std::string numName = "such that Modifies(1, \"yey\")";
    std::vector<SuchThatClause> clause3;
    QueryParser::parseSuchThatClause(&numName, syns, &clause3);
    REQUIRE(clause3.size() == 1);
    REQUIRE(clause3[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause3[0].refLeft.isASynonym() == false);
    REQUIRE(clause3[0].refLeft.getValueString() == "1");
    REQUIRE(clause3[0].refRight.isASynonym() == false);
    REQUIRE(clause3[0].refRight.getValueString() == "yey");

    std::string synSyn = "such that Modifies(a, v)";
    std::vector<SuchThatClause> clause4;
    QueryParser::parseSuchThatClause(&synSyn, syns, &clause4);
    REQUIRE(clause4.size() == 1);
    REQUIRE(clause4[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause4[0].refLeft.isASynonym() == true);
    REQUIRE(clause4[0].refLeft.getSynonymName() == "a");
    REQUIRE(clause4[0].refRight.isASynonym() == true);
    REQUIRE(clause4[0].refRight.getSynonymName() == "v");

    std::string synWildcard = "such that Modifies(a, _)";
    std::vector<SuchThatClause> clause5;
    QueryParser::parseSuchThatClause(&synWildcard, syns, &clause5);
    REQUIRE(clause5.size() == 1);
    REQUIRE(clause5[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause5[0].refLeft.isASynonym() == true);
    REQUIRE(clause5[0].refLeft.getSynonymName() == "a");
    REQUIRE(clause5[0].refRight.isASynonym() == false);
    REQUIRE(clause5[0].refRight.getValueString() == "_");

    std::string wildcardSyn = "such that Follows(_, a)";
    std::vector<SuchThatClause> clause6;
    QueryParser::parseSuchThatClause(&wildcardSyn, syns, &clause6);
    REQUIRE(clause6.size() == 1);
    REQUIRE(clause6[0].relationship == RelationshipReference::FOLLOWS);
    REQUIRE(clause6[0].refLeft.isASynonym() == false);
    REQUIRE(clause6[0].refLeft.getValueString() == "_");
    REQUIRE(clause6[0].refRight.isASynonym() == true);
    REQUIRE(clause6[0].refRight.getSynonymName() == "a");

    std::string wildcardSynStar = "such that Follows*(_, a)";
    std::vector<SuchThatClause> clause7;
    QueryParser::parseSuchThatClause(&wildcardSynStar, syns, &clause7);
    REQUIRE(clause7.size() == 1);
    REQUIRE(clause7[0].relationship == RelationshipReference::FOLLOWS_T);
    REQUIRE(clause7[0].refLeft.isASynonym() == false);
    REQUIRE(clause7[0].refLeft.getValueString() == "_");
    REQUIRE(clause7[0].refRight.isASynonym() == true);
    REQUIRE(clause7[0].refRight.getSynonymName() == "a");

    std::vector<SuchThatClause> ignore;
    std::string extraBracket = "such that Modifies((1, v)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&extraBracket, syns, &ignore));

    std::string undeclaredSyn = "such that Modifies(1, p)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&undeclaredSyn, syns, &ignore));

    std::string modifiesWildcard = "such that Modifies(_, v)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&modifiesWildcard, syns, &ignore));

    std::string usesWildcard = "such that Uses(_, v)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&usesWildcard, syns, &ignore));

    std::string missingBracket = "such that Modifies(1, v";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&missingBracket, syns, &ignore));

    std::string misspelled = "variable v; Select v suxh that Modifies(1, v)";
    REQUIRE_THROWS(QueryParser::parse(misspelled));

    std::string tooManyArguments = "such that Modifies(1, v, d)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&tooManyArguments, syns, &ignore));

    std::string tooFewArguments = "such that Modifies(1)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&tooFewArguments, syns, &ignore));

    std::string missingRelationshipName = "such that (1, v)";
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&missingRelationshipName,
                                                    syns, &ignore));
}

TEST_CASE("Parser can parse such that clause with procedure") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::PROCEDURE, "proc")};
    std::string synSyn = "such that Modifies(proc, v)";
    std::vector<SuchThatClause> clause1;
    QueryParser::parseSuchThatClause(&synSyn, syns, &clause1);
    REQUIRE(clause1.size() == 1);
    REQUIRE(clause1[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clause1[0].refLeft.isASynonym() == true);
    REQUIRE(clause1[0].refLeft.getSynonymName() == "proc");
    REQUIRE(clause1[0].refRight.isASynonym() == true);
    REQUIRE(clause1[0].refRight.getSynonymName() == "v");

    std::string synVal = "such that Uses(proc, \"x\")";
    std::vector<SuchThatClause> clause2;
    QueryParser::parseSuchThatClause(&synVal, syns, &clause2);
    REQUIRE(clause2.size() == 1);
    REQUIRE(clause2[0].relationship == RelationshipReference::USES);
    REQUIRE(clause2[0].refLeft.isASynonym() == true);
    REQUIRE(clause2[0].refLeft.getSynonymName() == "proc");
    REQUIRE(clause2[0].refRight.isASynonym() == false);
    REQUIRE(clause2[0].refRight.getValueString() == "x");

    std::string synWildcard = "such that Calls*(proc, _)";
    std::vector<SuchThatClause> clause3;
    QueryParser::parseSuchThatClause(&synWildcard, syns, &clause3);
    REQUIRE(clause3.size() == 1);
    REQUIRE(clause3[0].relationship == RelationshipReference::CALLS_T);
    REQUIRE(clause3[0].refLeft.isASynonym() == true);
    REQUIRE(clause3[0].refLeft.getSynonymName() == "proc");
    REQUIRE(clause3[0].refRight.isWildcard() == true);
}

TEST_CASE("Parser can detect semantic error of using wrong type of synonym and "
          "reference type") {
    std::vector<Synonym> syns{
        Synonym(EntityName::CONSTANT, "c"), Synonym(EntityName::VARIABLE, "v1"),
        Synonym(EntityName::VARIABLE, "v2"), Synonym(EntityName::ASSIGN, "a")};
    std::vector<SuchThatClause> ignoreSuchThat;
    std::vector<PatternClause> ignorePattern;
    std::string constVal = "such that Follow(c, \"1\")";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&constVal, syns, &ignoreSuchThat));

    std::string synConst = "such that Uses(a, c)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&synConst, syns, &ignoreSuchThat));

    std::string wrongLeftArg = "such that Uses(v1, v2)";
    REQUIRE_THROWS(
        QueryParser::parseSuchThatClause(&wrongLeftArg, syns, &ignoreSuchThat));

    std::string wrongPatternSyn = "pattern a(c, _\"x\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&wrongPatternSyn, syns,
                                                   &ignorePattern));

    std::string wrongPatternRefType = "pattern a(\"1\", _\"x\"_)";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&wrongPatternRefType, syns,
                                                   &ignorePattern));
}

TEST_CASE("Parser can parse pattern clause") {
    std::string correct = "pattern a(v, _\"x\"_)";
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};
    std::vector<PatternClause> clause;
    QueryParser::parsePatternClause(&correct, syns, &clause);
    REQUIRE(clause.size() == 1);
    REQUIRE(clause[0].entRef.isASynonym() == true);
    REQUIRE(clause[0].entRef.getSynonymName() == "v");
    REQUIRE(clause[0].entRef.getEntityName() == EntityName::VARIABLE);
    REQUIRE(clause[0].syn.getName() == "a");
    REQUIRE(clause[0].syn.getEntityName() == EntityName::ASSIGN);
    REQUIRE(clause[0].expression == "(x)");
    REQUIRE(clause[0].isExact == false);

    std::string exact = "pattern a(v, \"x\")";
    std::vector<PatternClause> clauseExact;
    QueryParser::parsePatternClause(&exact, syns, &clauseExact);
    REQUIRE(clauseExact.size() == 1);
    REQUIRE(clauseExact[0].entRef.isASynonym() == true);
    REQUIRE(clauseExact[0].entRef.getSynonymName() == "v");
    REQUIRE(clauseExact[0].entRef.getEntityName() == EntityName::VARIABLE);
    REQUIRE(clauseExact[0].syn.getName() == "a");
    REQUIRE(clauseExact[0].syn.getEntityName() == EntityName::ASSIGN);
    REQUIRE(clauseExact[0].expression == "(x)");
    REQUIRE(clauseExact[0].isExact == true);

    std::string space = "pattern a(v, _    \"    x    \"    _)";
    std::vector<PatternClause> clauseSpace;
    QueryParser::parsePatternClause(&space, syns, &clauseSpace);
    REQUIRE(clauseSpace[0].expression == "(x)");
    REQUIRE(clauseSpace[0].isExact == false);

    std::vector<PatternClause> ignorePattern;
    std::string extraBracket = "pattern a((v, \"x\")";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&extraBracket, syns, &clause));

    std::string undeclaredSyn = "pattern a(p, \"x\")";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&undeclaredSyn, syns, &clause));

    std::string misspelled = "assign a; Select a pattrn a(\"x\", _)";
    REQUIRE_THROWS(QueryParser::parse(misspelled));

    std::string tooManyArguments = "pattern a(v, _, a)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&tooManyArguments, syns, &clause));

    std::string tooFewArguments = "pattern a(v)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&tooFewArguments, syns, &clause));

    std::string missingQuotation = "pattern a(v, \"x)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&missingQuotation, syns, &clause));

    std::string missingQuotation2 = "pattern a(v, x)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&missingQuotation2, syns, &clause));

    std::string extraQuotation = "pattern a(v, \"x+\"y\")";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&extraQuotation, syns, &clause));

    std::string extraUnderscore = "pattern a(v, _\"x_+y\"_)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&extraQuotation, syns, &clause));

    std::string missingUnderscore = "pattern a(v, _\"x\")";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&missingUnderscore, syns, &clause));

    std::string wrongExpressionSyntax = "pattern a(c, _\"+expression+hi\"_)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&wrongExpressionSyntax, syns, &clause));

    std::string missingBracketLeft = "pattern a(v, _\"(x\"_)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&missingBracketLeft, syns, &clause));

    std::string missingBracketLeft2 = "pattern a(v, \"(x\")";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&missingBracketLeft2, syns, &clause));

    std::string missingBracketRight = "pattern a(v, _\"x)\"_)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&missingBracketRight, syns, &clause));

    std::string missingBracketRight2 = "pattern a(v, \"x)\")";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&missingBracketRight2, syns, &clause));
}

TEST_CASE("Parser can parse multiple such that clauses") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::STMT, "s"), Synonym(EntityName::READ, "r"),
        Synonym(EntityName::ASSIGN, "a1")};
    std::string input =
        "such that Modifies(a, _) pattern a(_, _) such that Follows(s, r) such "
        "that Uses(s, v) pattern a1(v, _\"weew\"_)";
    std::string remainingInput =
        " pattern a(_, _) such that Follows(s, r) such that Uses(s, v) pattern "
        "a1(v, _\"weew\"_)";
    std::vector<SuchThatClause> clauses;
    QueryParser::parseSuchThatClause(&input, syns, &clauses);
    REQUIRE(clauses.size() == 1);
    REQUIRE(clauses[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(input == remainingInput);
}

TEST_CASE("Parser can parse multiple pattern clauses") {
    std::vector<Synonym> syns{
        Synonym(EntityName::ASSIGN, "a1"), Synonym(EntityName::ASSIGN, "a2"),
        Synonym(EntityName::VARIABLE, "v1"), Synonym(EntityName::STMT, "s1"),
        Synonym(EntityName::STMT, "s2")};
    std::string input =
        "pattern a1(v1, _\"yey\"_) such that Follows(s1, s2) such "
        "that Uses(s2, v1) pattern a2(v1, _\"weew\"_)";
    std::string remainingInput = " such that Follows(s1, s2) such "
                                 "that Uses(s2, v1) pattern a2(v1, _\"weew\"_)";
    std::vector<PatternClause> clauses;
    QueryParser::parsePatternClause(&input, syns, &clauses);
    REQUIRE(clauses.size() == 1);
    REQUIRE(input == remainingInput);
}
TEST_CASE("Parser can parse multiple such that clauses with explicit and") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::STMT, "s"), Synonym(EntityName::READ, "r"),
        Synonym(EntityName::ASSIGN, "a1")};
    std::string input =
        "such that Modifies(a, _) and Follows(s, r) pattern a(_, _) such "
        "that Uses(s, v) pattern a1(v, _\"weew\"_)";
    std::string remainingInput = " pattern a(_, _) such "
                                 "that Uses(s, v) pattern a1(v, _\"weew\"_)";
    std::vector<SuchThatClause> clauses;
    QueryParser::parseSuchThatClause(&input, syns, &clauses);
    REQUIRE(clauses.size() == 2);
    REQUIRE(clauses[0].relationship == RelationshipReference::MODIFIES);
    REQUIRE(clauses[1].relationship == RelationshipReference::FOLLOWS);
    REQUIRE(input == remainingInput);
}

TEST_CASE("Parser can parse multiple pattern clauses with explicit and") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::STMT, "s"), Synonym(EntityName::READ, "r"),
        Synonym(EntityName::ASSIGN, "a1")};
    std::string input = "pattern a(_, _) and a1(v, _\"weew\"_) such that "
                        "Modifies(a, _) and Follows(s, r) such "
                        "that Uses(s, v)";
    std::string remainingInput =
        " such that Modifies(a, _) and Follows(s, r) such "
        "that Uses(s, v)";
    std::vector<PatternClause> clauses;
    QueryParser::parsePatternClause(&input, syns, &clauses);
    REQUIRE(clauses.size() == 2);
    REQUIRE(input == remainingInput);
}

TEST_CASE("Parser can catch mismatch in explicit and") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::STMT, "s"), Synonym(EntityName::READ, "r"),
        Synonym(EntityName::ASSIGN, "a1")};
    std::string patternInput = "pattern a(_, _) and Follows(s, r) such that "
                               "Modifies(a, _) and a1(v, _\"weew\"_) such "
                               "that Uses(s, v)";

    std::vector<PatternClause> patternClauses;
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&patternInput, syns, &patternClauses));
    std::string suchThatInput =
        "such that Modifies(a, _) and a1(v, _\"weew\"_) pattern a(_, _) and "
        "Follows(s, r) such that Uses(s, v)";

    std::vector<SuchThatClause> suchThatClauses;
    REQUIRE_THROWS(QueryParser::parseSuchThatClause(&suchThatInput, syns,
                                                    &suchThatClauses));
}

TEST_CASE("Parser can parse while pattern clauses") {
    std::vector<Synonym> syns{Synonym(EntityName::WHILE, "w"),
                              Synonym(EntityName::WHILE, "w1"),
                              Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::VARIABLE, "v2")};

    std::string correct = "pattern w(v, _)";
    std::vector<PatternClause> clause;
    QueryParser::parsePatternClause(&correct, syns, &clause);
    REQUIRE(clause[0].expression == "_");
    REQUIRE(clause[0].patternType == PatternType::WHILE);

    std::string space = "pattern w(_,    _      )";
    std::vector<PatternClause> clauseSpace;
    QueryParser::parsePatternClause(&space, syns, &clauseSpace);
    REQUIRE(clauseSpace[0].expression == "_");
    REQUIRE(clauseSpace[0].patternType == PatternType::WHILE);

    std::string noWildcard = "pattern w(v, \"x\")";
    REQUIRE_THROWS(QueryParser::parsePatternClause(&noWildcard, syns, &clause));
}

TEST_CASE("Parser can parse if pattern clauses") {
    std::vector<Synonym> syns{Synonym(EntityName::IF, "ifs"),
                              Synonym(EntityName::IF, "ifs2"),
                              Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::VARIABLE, "v2")};

    std::string correct = "pattern ifs(v, _, _)";
    std::vector<PatternClause> clause;
    QueryParser::parsePatternClause(&correct, syns, &clause);
    REQUIRE(clause[0].expression == "_");
    REQUIRE(clause[0].patternType == PatternType::IF);

    std::string space = "pattern ifs(   _    ,   _   ,     _    )";
    std::vector<PatternClause> clauseSpace;
    QueryParser::parsePatternClause(&space, syns, &clauseSpace);
    REQUIRE(clauseSpace[0].expression == "_");
    REQUIRE(clauseSpace[0].patternType == PatternType::IF);

    std::string noWildcardLeft = "pattern ifs(v, \"x\", _)";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&noWildcardLeft, syns, &clause));

    std::string noWildcardRight = "pattern ifs(v, _, \"x\")";
    REQUIRE_THROWS(
        QueryParser::parsePatternClause(&noWildcardRight, syns, &clause));
}

TEST_CASE("Parser can parse with clauses") {
    std::vector<Synonym> syns{
        Synonym(EntityName::CALL, "c"),  Synonym(EntityName::PROCEDURE, "p"),
        Synonym(EntityName::READ, "r"),  Synonym(EntityName::CONSTANT, "const"),
        Synonym(EntityName::WHILE, "w"), Synonym(EntityName::ASSIGN, "a")};

    std::string attrVal = "with  c . procName = \"main\"";
    std::vector<WithClause> clause;
    QueryParser::parseWithClause(&attrVal, syns, &clause);
    REQUIRE(clause[0].refLeft.getRefType() == ReferenceType::ATTR_REF);
    REQUIRE(clause[0].refLeft.getAttr() == EntityAttribute::PROC_NAME);
    REQUIRE(clause[0].refRight.getValueString() == "main");

    std::string attrVal2 = "with  const.value = 2";
    std::vector<WithClause> clause2;
    QueryParser::parseWithClause(&attrVal2, syns, &clause2);
    REQUIRE(clause2[0].refLeft.getRefType() == ReferenceType::ATTR_REF);
    REQUIRE(clause2[0].refLeft.getAttr() == EntityAttribute::VALUE);
    REQUIRE(clause2[0].refRight.getValueString() == "2");

    std::string valVal = "with  2 = 1";
    std::vector<WithClause> clauseValVal;
    QueryParser::parseWithClause(&valVal, syns, &clauseValVal);
    REQUIRE(clauseValVal[0].refLeft.getValueString() == "2");
    REQUIRE(clauseValVal[0].refRight.getValueString() == "1");

    std::string attrAttr = "with  p. procName = r .varName";
    std::vector<WithClause> clauseAttrAttr;
    QueryParser::parseWithClause(&attrAttr, syns, &clauseAttrAttr);
    REQUIRE(clauseAttrAttr[0].refLeft.getAttr() == EntityAttribute::PROC_NAME);
    REQUIRE(clauseAttrAttr[0].refRight.getAttr() == EntityAttribute::VAR_NAME);

    std::string missingDot = "with  w stmt# = 2";
    REQUIRE_THROWS(QueryParser::parseWithClause(&missingDot, syns, &clause));

    std::string missingEqualSign = "with  w.stmt# 2";
    REQUIRE_THROWS(
        QueryParser::parseWithClause(&missingEqualSign, syns, &clause));

    std::string extraQuotationMark = "with const.value = \"0\"";
    REQUIRE_THROWS(
        QueryParser::parseWithClause(&extraQuotationMark, syns, &clause));

    std::string missingQuotationMark = "with r.varName = varName";
    REQUIRE_THROWS(
        QueryParser::parseWithClause(&missingQuotationMark, syns, &clause));

    std::string extraEqualSign = "with c.stmt# == 2";
    REQUIRE_THROWS(
        QueryParser::parseWithClause(&extraEqualSign, syns, &clause));

    std::string invalidAttr = "with w.value = 10";
    REQUIRE_THROWS(QueryParser::parseWithClause(&invalidAttr, syns, &clause));

    std::string synonymNotFound = "with while.value = 10";
    REQUIRE_THROWS(
        QueryParser::parseWithClause(&synonymNotFound, syns, &clause));

    std::string missingAttr = "with const. = 10";
    REQUIRE_THROWS(QueryParser::parseWithClause(&missingAttr, syns, &clause));

    std::string missingAttr2 = "with const = 10";
    REQUIRE_THROWS(QueryParser::parseWithClause(&missingAttr2, syns, &clause));

    std::string missingSyn = "with .value = 10";
    REQUIRE_THROWS(QueryParser::parseWithClause(&missingSyn, syns, &clause));

    std::string missingSyn2 = "with value = 10";
    REQUIRE_THROWS(QueryParser::parseWithClause(&missingSyn2, syns, &clause));

    std::string wildcard = "with _ = a.stmt#";
    REQUIRE_THROWS(QueryParser::parseWithClause(&wildcard, syns, &clause));

    std::string bothWildcard = "with _ = _";
    REQUIRE_THROWS(QueryParser::parseWithClause(&bothWildcard, syns, &clause));

    std::string mismatchArgType = "with r.varName = w.stmt#";
    REQUIRE_THROWS(
        QueryParser::parseWithClause(&mismatchArgType, syns, &clause));

    std::string mismatchArgType2 = "with p.procName = 1";
    REQUIRE_THROWS(
        QueryParser::parseWithClause(&mismatchArgType2, syns, &clause));

    std::string mismatchArgType3 = "with \"main\" = a.stmt#";
    REQUIRE_THROWS(
        QueryParser::parseWithClause(&mismatchArgType3, syns, &clause));
}