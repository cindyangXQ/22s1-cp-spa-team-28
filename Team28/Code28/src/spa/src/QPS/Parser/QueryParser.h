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

const char CLAUSE_SEPARATOR = ';';
const char ARG_SEPARATOR = ',';
const int MIN_CLAUSE_NUM = 2;
const int SUCH_THAT_REGEX_CHECK = 5;
const int PATTERN_REGEX_CHECK = 7;
const int WITH_REGEX_CHECK = 4;

/*
 * Class responsible for parsing the strings of query.
 */
class QueryParser {
public:
    static SolvableQuery parse(std::string query);
    static Declaration parseDeclaration(std::vector<std::string> clauses);
    static SelectClause parseSelectClause(std::string *clause,
                                          std::vector<Synonym> syns);
    static void parseSuchThatClause(std::string *clause,
                                    std::vector<Synonym> syns,
                                    std::vector<SuchThatClause> *suchThatCls);
    static void parsePatternClause(std::string *clause,
                                   std::vector<Synonym> syns,
                                   std::vector<PatternClause> *patternCls);
    static void parseWithClause(std::string *clause, std::vector<Synonym> syns,
                                std::vector<WithClause> *withCls);

private:
    static SelectClause parseSelectTuple(std::string selectValue,
                                         std::vector<Synonym> syns);
    static bool isSelectClause(std::string *clause);
    static bool isSuchThatClause(std::string *clause);
    static bool isPatternClause(std::string *clause);
    static bool isWithClause(std::string *clause);
    static bool isValidSuchThatClause(RelationshipReference relRef,
                                      Reference left, Reference right);
    static bool isValidPatternClause(Synonym syn, Reference entRef,
                                     Expression expr);
    static bool isValidWithClause(Reference left, Reference right);
    static std::vector<Synonym> parseSynonyms(std::vector<std::string> tokens);
    static bool isDuplicateSynonymName(std::vector<Synonym> syns);
    static RelationshipReference getRelationshipReference(std::string input);
    static Reference getReference(std::string input, std::vector<Synonym> syns);
    static Synonym getSynonym(std::string input, std::vector<Synonym> syns);
};
