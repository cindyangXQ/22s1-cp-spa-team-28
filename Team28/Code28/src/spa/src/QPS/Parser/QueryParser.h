#pragma once
#include "../../commons/Reference.h"
#include "../../commons/Statement.h"
#include "../../commons/Variable.h"
#include "../Clause/SelectClause.h"
#include "../Error/SemanticError.h"
#include "../Error/SyntaxError.h"
#include "../Utils.h"
#include "QueryParserRegex.h"
#include "SolvableQuery.h"
#include <iostream>
#include <regex>

/*
 * Class responsible for parsing the strings of query.
 */
class QueryParser {
public:
    static SolvableQuery parse(std::string query);
    static Declaration parseDeclaration(std::vector<std::string> clauses);
    static SelectType parseSelectClause(std::string *clause,
                                        std::vector<Synonym> syns);
    static SuchThatClause parseSuchThatClause(std::string *clause,
                                              std::vector<Synonym> syns);
    static PatternClause parsePatternClause(std::string *clause,
                                            std::vector<Synonym> syns);

private:
    static bool isValidName(std::string name);
    static bool isValidSuchThatClause(RelationshipReference relRef,
                                      Reference left, Reference right);
    static std::vector<Synonym> parseSynonyms(std::vector<std::string> tokens);
    static bool isDuplicateSynonymName(std::vector<Synonym> syns);
    static RelationshipReference getRelationshipReference(std::string input);
    static Reference getReference(std::string input, std::vector<Synonym> syns);
    static Synonym getSynonym(std::string input, std::vector<Synonym> syns);
};
