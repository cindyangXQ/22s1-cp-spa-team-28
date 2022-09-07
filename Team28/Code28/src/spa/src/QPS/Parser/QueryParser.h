#pragma once
#include <iostream>
#include <regex>
#include "SolvableQuery.h"
#include "../Utils.h"
#include "../../commons/Reference.h"
#include "../../commons//Variable.h"
#include "../../commons//Statement.h"
#include "../Error/ParseError.h"

/*
 * Class responsible for parsing the strings of query.
 */
class QueryParser {
public:
    static SolvableQuery parse(std::string query);
    static Declaration parseDeclaration(std::vector<std::string> clauses);
    static SelectType parseSelectClause(std::string mainClause, std::vector<Synonym> syns);
    static SuchThatClause parseSuchThatClause(std::string mainClause, std::vector<Synonym> syns);
    static PatternClause parsePatternClause(std::string mainClause, std::vector<Synonym> syns);
private:
    static bool checkValidName(std::string name);
    static std::vector<Synonym> parseSynonyms(std::vector<std::string> tokens);
    static DesignEntity getDesignEntity(std::string input);
    static bool checkDuplicateSynonymName(std::vector<Synonym> syns);
    static bool isSuchThatClause(std::vector<std::string> tokens, size_t start);
    static bool isPatternClause(std::vector<std::string> tokens, size_t start);
    static RelationshipReference getRelationshipReference(std::string input);
    static Reference getReference(std::string input, std::vector<Synonym> syns);
    static Synonym getSynonym(std::string input, std::vector<Synonym> syns);
};