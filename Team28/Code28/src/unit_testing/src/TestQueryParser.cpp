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
    std::vector<QueryClause*> clauses;
    QueryParser::parse<SuchThatClause>(&numSyn, syns, &clauses);
    SuchThatClause *clause = dynamic_cast<SuchThatClause*>(clauses[0]);
    Reference left1 = clause->getRefLeft();
    Reference right1 = clause->getRefRight();

    REQUIRE(clauses.size() == 1);
    REQUIRE(clause->getRelationship() == RelationshipReference::MODIFIES);
    REQUIRE(left1.isASynonym() == false);
    REQUIRE(left1.getValueString() == "1");
    REQUIRE(right1.isASynonym() == true);
    REQUIRE(right1.getSynonymName() == "v");
}