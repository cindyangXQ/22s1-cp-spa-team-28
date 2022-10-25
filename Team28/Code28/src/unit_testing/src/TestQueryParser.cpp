#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"
#include "SP/SP.h"

#include "catch.hpp"

TEST_CASE("QueryParser is parsing correctly") {
    SolvableQuery solvableQ =
        QueryParser::parse("assign a; constant c; variable v; Select a such "
                           "that Modifies(1, v) pattern a(v, _\"x\"_)");
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
    std::vector<Synonym> syns = declare.getSyns();
    REQUIRE(syns.size() == 3);

    std::vector<std::string> multipleDeclarationClauses = {
        "assign a, b, a2", "constant c, constant, yey", "variable v, v2, v3",
        "Select v such that Modifies(1, v) pattern a(v, _)"};
    Declaration multipleDeclare =
        QueryParser::parseDeclaration(multipleDeclarationClauses);
    std::vector<Synonym> multipleSyns = multipleDeclare.getSyns();
    REQUIRE(multipleSyns.size() == 9);

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
    Synonym selectedSynonym = selectClause.getRefs()[0].getSynonym();
    REQUIRE(selectedSynonym.getEntityName() == EntityName::VARIABLE);
    REQUIRE(selectedSynonym.getName() == "v");

    std::string misspelled = "Selct v";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&misspelled, syns));

    std::string missingSynonym = "Select";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&missingSynonym, syns));

    std::string wildcard = "Select _";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wildcard, syns));

    std::string number = "Select 1";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&number, syns));
}

TEST_CASE("QueryParser can parse boolean select clause") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};

    std::string correct = "Select BOOLEAN";
    SelectClause selectClause = QueryParser::parseSelectClause(&correct, syns);
    std::vector<Reference> refs = selectClause.getRefs();
    REQUIRE(selectClause.getSelectType() == SelectType::BOOLEAN);
    REQUIRE(refs.size() == 0);

    std::string misspelled = "Select BOOLAN";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&misspelled, syns));

    std::string misspelled2 = "Select BoOlEaN";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&misspelled2, syns));
}

TEST_CASE("QueryParser can parse select boolean when boolean is declared as "
    "synonym") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a"),
                              Synonym(EntityName::STMT, "BOOLEAN")};
    std::string select_boolean = "Select BOOLEAN";
    SelectClause selectClause = QueryParser::parseSelectClause(&select_boolean, syns);
    REQUIRE(selectClause.getSelectType() == SelectType::SINGLE);
    REQUIRE(selectClause.getRefs().size() == 1);
    Synonym selectedSynonym = selectClause.getRefs()[0].getSynonym();
    REQUIRE(selectedSynonym.getEntityName() == EntityName::STMT);
    REQUIRE(selectedSynonym.getName() == "BOOLEAN");
}

TEST_CASE("QueryParser can parse tuple select clause") {
    std::vector<Synonym> syns{
        Synonym(EntityName::VARIABLE, "v"), Synonym(EntityName::ASSIGN, "a"),
        Synonym(EntityName::ASSIGN, "a1"), Synonym(EntityName::ASSIGN, "a2"),
        Synonym(EntityName::PROCEDURE, "p")};

    std::string correct = "Select <v, a>";
    SelectClause selectClause = QueryParser::parseSelectClause(&correct, syns);
    std::vector<Reference> refs = selectClause.getRefs();
    REQUIRE(selectClause.getSelectType() == SelectType::TUPLE);
    REQUIRE(refs.size() == 2);

    std::string many = "Select <v, a, p, a1, a2>";
    SelectClause selectClauseMany = QueryParser::parseSelectClause(&many, syns);
    std::vector<Reference> refsMany = selectClauseMany.getRefs();
    REQUIRE(selectClauseMany.getSelectType() == SelectType::TUPLE);
    REQUIRE(refsMany.size() == 5);

    std::string one = "Select <v>";
    SelectClause selectClauseOne = QueryParser::parseSelectClause(&one, syns);
    std::vector<Reference> refsOne = selectClauseOne.getRefs();
    REQUIRE(selectClauseOne.getSelectType() == SelectType::TUPLE);
    REQUIRE(refsOne.size() == 1);

    std::string duplicate = "Select <v, a1, a1>";
    SelectClause selectClauseDuplicate =
        QueryParser::parseSelectClause(&duplicate, syns);
    std::vector<Reference> refsDuplicate = selectClauseDuplicate.getRefs();
    REQUIRE(selectClauseDuplicate.getSelectType() == SelectType::TUPLE);
    REQUIRE(refsDuplicate.size() == 3);

    std::string wrongSyntax = "Select <v, a1, <a2>>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrongSyntax, syns));

    std::string wrongSyntax2 = "Select <v, a1, <a2>, p>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrongSyntax2, syns));

    std::string wrongSyntax3 = "Select <v, <a2, p>, a1>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wrongSyntax3, syns));

    std::string wildcard = "Select <_, v>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&wildcard, syns));

    std::string number = "Select <v, 1>";
    REQUIRE_THROWS(QueryParser::parseSelectClause(&number, syns));
}

TEST_CASE("QueryParser can parse select clause with attribute") {
    std::vector<Synonym> syns{
        Synonym(EntityName::CALL, "c"),  Synonym(EntityName::PROCEDURE, "proc"),
        Synonym(EntityName::PRINT, "p"), Synonym(EntityName::CONSTANT, "const"),
        Synonym(EntityName::WHILE, "w"), Synonym(EntityName::VARIABLE, "v")};

    std::string one = "Select w. stmt#";
    SelectClause selectClause = QueryParser::parseSelectClause(&one, syns);
    std::vector<Reference> refs = selectClause.getRefs();
    REQUIRE(selectClause.getSelectType() == SelectType::SINGLE);
    REQUIRE(refs.size() == 1);
    REQUIRE(refs[0].getEntityName() == EntityName::WHILE);
    REQUIRE(refs[0].getAttr() == EntityAttribute::STMT_NO);

    std::string many = "Select <const .value , p.varName, v. varName, c.stmt#>";
    SelectClause selectClauseMany = QueryParser::parseSelectClause(&many, syns);
    std::vector<Reference> refsMany = selectClauseMany.getRefs();
    REQUIRE(selectClauseMany.getSelectType() == SelectType::TUPLE);
    REQUIRE(refsMany.size() == 4);
    REQUIRE(refsMany[0].getEntityName() ==
            EntityName::CONSTANT);
    REQUIRE(refsMany[0].getAttr() == EntityAttribute::VALUE);
    REQUIRE(refsMany[1].getEntityName() == EntityName::PRINT);
    REQUIRE(refsMany[1].getAttr() ==
            EntityAttribute::VAR_NAME);
    REQUIRE(refsMany[2].getEntityName() ==
            EntityName::VARIABLE);
    REQUIRE(refsMany[2].getAttr() ==
            EntityAttribute::VAR_NAME);
    REQUIRE(refsMany[3].getEntityName() == EntityName::CALL);
    REQUIRE(refsMany[3].getAttr() ==
            EntityAttribute::STMT_NO);

    std::string mixed = "Select <w , p.varName, v, c.stmt#>";
    SelectClause selectClauseMixed =
        QueryParser::parseSelectClause(&mixed, syns);
    std::vector<Reference> refsMixed = selectClauseMixed.getRefs();
    REQUIRE(selectClauseMixed.getSelectType() == SelectType::TUPLE);
    REQUIRE(refsMixed.size() == 4);

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
    Reference left1 = clause1[0].getRefLeft();
    Reference right1 = clause1[0].getRefRight();

    REQUIRE(clause1.size() == 1);
    REQUIRE(clause1[0].getRelationship() == RelationshipReference::MODIFIES);
    REQUIRE(left1.isASynonym() == false);
    REQUIRE(left1.getValueString() == "1");
    REQUIRE(right1.isASynonym() == true);
    REQUIRE(right1.getSynonymName() == "v");

    std::string synName = "such that Modifies(a, \"yey\")";
    std::vector<SuchThatClause> clause2;
    QueryParser::parseSuchThatClause(&synName, syns, &clause2);
    Reference left2 = clause2[0].getRefLeft();
    Reference right2 = clause2[0].getRefRight();
    REQUIRE(clause2.size() == 1);
    REQUIRE(clause2[0].getRelationship() == RelationshipReference::MODIFIES);
    REQUIRE(left2.isASynonym() == true);
    REQUIRE(left2.getSynonymName() == "a");
    REQUIRE(right2.isASynonym() == false);
    REQUIRE(right2.getValueString() == "yey");

    std::string numName = "such that Modifies(1, \"yey\")";
    std::vector<SuchThatClause> clause3;
    QueryParser::parseSuchThatClause(&numName, syns, &clause3);
    Reference left3 = clause3[0].getRefLeft();
    Reference right3 = clause3[0].getRefRight();
    REQUIRE(clause3.size() == 1);
    REQUIRE(clause3[0].getRelationship() == RelationshipReference::MODIFIES);
    REQUIRE(left3.isASynonym() == false);
    REQUIRE(left3.getValueString() == "1");
    REQUIRE(right3.isASynonym() == false);
    REQUIRE(right3.getValueString() == "yey");

    std::string synSyn = "such that Modifies(a, v)";
    std::vector<SuchThatClause> clause4;
    QueryParser::parseSuchThatClause(&synSyn, syns, &clause4);
    Reference left4 = clause4[0].getRefLeft();
    Reference right4 = clause4[0].getRefRight();
    REQUIRE(clause4.size() == 1);
    REQUIRE(clause4[0].getRelationship() == RelationshipReference::MODIFIES);
    REQUIRE(left4.isASynonym() == true);
    REQUIRE(left4.getSynonymName() == "a");
    REQUIRE(right4.isASynonym() == true);
    REQUIRE(right4.getSynonymName() == "v");

    std::string synWildcard = "such that Modifies(a, _)";
    std::vector<SuchThatClause> clause5;
    QueryParser::parseSuchThatClause(&synWildcard, syns, &clause5);
    Reference left5 = clause5[0].getRefLeft();
    Reference right5 = clause5[0].getRefRight();
    REQUIRE(clause5.size() == 1);
    REQUIRE(clause5[0].getRelationship() == RelationshipReference::MODIFIES);
    REQUIRE(left5.isASynonym() == true);
    REQUIRE(left5.getSynonymName() == "a");
    REQUIRE(right5.isASynonym() == false);
    REQUIRE(right5.getValueString() == "_");

    std::string wildcardSyn = "such that Follows(_, a)";
    std::vector<SuchThatClause> clause6;
    QueryParser::parseSuchThatClause(&wildcardSyn, syns, &clause6);
    Reference left6 = clause6[0].getRefLeft();
    Reference right6 = clause6[0].getRefRight();
    REQUIRE(clause6.size() == 1);
    REQUIRE(clause6[0].getRelationship() == RelationshipReference::FOLLOWS);
    REQUIRE(left6.isASynonym() == false);
    REQUIRE(left6.getValueString() == "_");
    REQUIRE(right6.isASynonym() == true);
    REQUIRE(right6.getSynonymName() == "a");

    std::string wildcardSynStar = "such that Follows*(_, a)";
    std::vector<SuchThatClause> clause7;
    QueryParser::parseSuchThatClause(&wildcardSynStar, syns, &clause7);
    Reference left7 = clause7[0].getRefLeft();
    Reference right7 = clause7[0].getRefRight();
    REQUIRE(clause7.size() == 1);
    REQUIRE(clause7[0].getRelationship() == RelationshipReference::FOLLOWS_T);
    REQUIRE(left7.isASynonym() == false);
    REQUIRE(left7.getValueString() == "_");
    REQUIRE(right7.isASynonym() == true);
    REQUIRE(right7.getSynonymName() == "a");

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
    Reference left1 = clause1[0].getRefLeft();
    Reference right1 = clause1[0].getRefRight();
    REQUIRE(clause1.size() == 1);
    REQUIRE(clause1[0].getRelationship() == RelationshipReference::MODIFIES);
    REQUIRE(left1.isASynonym() == true);
    REQUIRE(left1.getSynonymName() == "proc");
    REQUIRE(right1.isASynonym() == true);
    REQUIRE(right1.getSynonymName() == "v");

    std::string synVal = "such that Uses(proc, \"x\")";
    std::vector<SuchThatClause> clause2;
    QueryParser::parseSuchThatClause(&synVal, syns, &clause2);
    Reference left2 = clause2[0].getRefLeft();
    Reference right2 = clause2[0].getRefRight();
    REQUIRE(clause2.size() == 1);
    REQUIRE(clause2[0].getRelationship() == RelationshipReference::USES);
    REQUIRE(left2.isASynonym() == true);
    REQUIRE(left2.getSynonymName() == "proc");
    REQUIRE(right2.isASynonym() == false);
    REQUIRE(right2.getValueString() == "x");

    std::string synWildcard = "such that Calls*(proc, _)";
    std::vector<SuchThatClause> clause3;
    QueryParser::parseSuchThatClause(&synWildcard, syns, &clause3);
    Reference left3 = clause3[0].getRefLeft();
    Reference right3 = clause3[0].getRefRight();
    REQUIRE(clause3.size() == 1);
    REQUIRE(clause3[0].getRelationship() == RelationshipReference::CALLS_T);
    REQUIRE(left3.isASynonym() == true);
    REQUIRE(left3.getSynonymName() == "proc");
    REQUIRE(right3.isWildcard() == true);
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
    Reference entRef = clause[0].getEntRef();
    Synonym syn = clause[0].getSyn();
    REQUIRE(clause.size() == 1);
    REQUIRE(entRef.isASynonym() == true);
    REQUIRE(entRef.getSynonymName() == "v");
    REQUIRE(entRef.getEntityName() == EntityName::VARIABLE);
    REQUIRE(syn.getName() == "a");
    REQUIRE(syn.getEntityName() == EntityName::ASSIGN);
    REQUIRE(clause[0].getExpression() == "(x)");
    REQUIRE(clause[0].getIsExact() == false);

    std::string exact = "pattern a(v, \"x\")";
    std::vector<PatternClause> clauseExact;
    QueryParser::parsePatternClause(&exact, syns, &clauseExact);
    Reference entRefExact = clauseExact[0].getEntRef();
    Synonym synExact = clauseExact[0].getSyn();
    REQUIRE(clauseExact.size() == 1);
    REQUIRE(entRefExact.isASynonym() == true);
    REQUIRE(entRefExact.getSynonymName() == "v");
    REQUIRE(entRefExact.getEntityName() == EntityName::VARIABLE);
    REQUIRE(synExact.getName() == "a");
    REQUIRE(synExact.getEntityName() == EntityName::ASSIGN);
    REQUIRE(clauseExact[0].getExpression() == "(x)");
    REQUIRE(clauseExact[0].getIsExact() == true);

    std::string space = "pattern a(v, _    \"    x    \"    _)";
    std::vector<PatternClause> clauseSpace;
    QueryParser::parsePatternClause(&space, syns, &clauseSpace);
    REQUIRE(clauseSpace[0].getExpression() == "(x)");
    REQUIRE(clauseSpace[0].getIsExact() == false);

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
    REQUIRE(clauses[0].getRelationship() == RelationshipReference::MODIFIES);
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
    REQUIRE(clauses[0].getRelationship() == RelationshipReference::MODIFIES);
    REQUIRE(clauses[1].getRelationship() == RelationshipReference::FOLLOWS);
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
    REQUIRE(clause[0].getExpression() == "_");
    REQUIRE(clause[0].getPatternType() == PatternType::WHILE);

    std::string space = "pattern w(_,    _      )";
    std::vector<PatternClause> clauseSpace;
    QueryParser::parsePatternClause(&space, syns, &clauseSpace);
    REQUIRE(clauseSpace[0].getExpression() == "_");
    REQUIRE(clauseSpace[0].getPatternType() == PatternType::WHILE);

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
    REQUIRE(clause[0].getExpression() == "_");
    REQUIRE(clause[0].getPatternType() == PatternType::IF);

    std::string space = "pattern ifs(   _    ,   _   ,     _    )";
    std::vector<PatternClause> clauseSpace;
    QueryParser::parsePatternClause(&space, syns, &clauseSpace);
    REQUIRE(clauseSpace[0].getExpression() == "_");
    REQUIRE(clauseSpace[0].getPatternType() == PatternType::IF);

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
    Reference left1 = clause[0].getRefLeft();
    Reference right1 = clause[0].getRefRight();
    REQUIRE(left1.getRefType() == ReferenceType::ATTR_REF);
    REQUIRE(left1.getAttr() == EntityAttribute::PROC_NAME);
    REQUIRE(right1.getValueString() == "main");

    std::string attrVal2 = "with  const.value = 2";
    std::vector<WithClause> clause2;
    QueryParser::parseWithClause(&attrVal2, syns, &clause2);
    Reference left2 = clause2[0].getRefLeft();
    Reference right2 = clause2[0].getRefRight();
    REQUIRE(left2.getRefType() == ReferenceType::ATTR_REF);
    REQUIRE(left2.getAttr() == EntityAttribute::VALUE);
    REQUIRE(right2.getValueString() == "2");

    std::string valVal = "with  2 = 1";
    std::vector<WithClause> clauseValVal;
    QueryParser::parseWithClause(&valVal, syns, &clauseValVal);
    Reference left3 = clauseValVal[0].getRefLeft();
    Reference right3 = clauseValVal[0].getRefRight();
    REQUIRE(left3.getValueString() == "2");
    REQUIRE(right3.getValueString() == "1");

    std::string attrAttr = "with  p. procName = r .varName";
    std::vector<WithClause> clauseAttrAttr;
    QueryParser::parseWithClause(&attrAttr, syns, &clauseAttrAttr);
    Reference left4 = clauseAttrAttr[0].getRefLeft();
    Reference right4 = clauseAttrAttr[0].getRefRight();
    REQUIRE(left4.getAttr() ==
            EntityAttribute::PROC_NAME);
    REQUIRE(right4.getAttr() ==
            EntityAttribute::VAR_NAME);

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