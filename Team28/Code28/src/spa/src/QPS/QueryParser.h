#pragma once

#include "SolvableQuery.h"

class QueryParser {
public:
    static void parse(std::string query);
    static Declaration parseDeclaration(std::vector<std::string> clauses);
    static Synonym parseSynonym(std::string desc);
    static SelectClause parseSelectClause(std::string mainClause, std::vector<Synonym> syns);
    static SuchThatClause parseSuchThatClause(std::string mainClause);
    static PatternClause parsePatternClause(std::string mainClause);
};