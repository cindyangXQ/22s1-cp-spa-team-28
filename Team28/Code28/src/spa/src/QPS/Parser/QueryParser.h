#pragma once
#include <iostream>
#include "SolvableQuery.h"
#include "../Utils.h"
#include "../Reference/Reference.h"
#include "../Reference/VariableName.h"
#include "../Reference/StatementNumber.h"
#include "../Error/ParseError.h"

/*
 * Class responsible for parsing the strings of query.
 */
class QueryParser {
public:
    static SolvableQuery parse(std::string query);
    static Declaration parseDeclaration(std::vector<std::string> clauses);
    static Synonym parseSynonym(std::string desc);
    static SelectType parseSelectClause(std::string mainClause, std::vector<Synonym> syns);
    static SuchThatClause parseSuchThatClause(std::string mainClause, std::vector<Synonym> syns);
    static PatternClause parsePatternClause(std::string mainClause, std::vector<Synonym> syns);
private:
    static bool isSuchThatClause(std::vector<std::string> tokens, size_t start);
    static bool isPatternClause(std::vector<std::string> tokens, size_t start);
    static RelationshipReference getRelationshipReference(std::string input);
    static Reference getReference(std::string input, std::vector<Synonym> syns);
    static Synonym getSynonym(std::string input, std::vector<Synonym> syns);
};