#pragma once
#include "../../SP/SP.h"
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

const std::unordered_map<std::type_index, int> REGEX_CHECK_MAP = {
    {typeid(SuchThatClause), SUCH_THAT_REGEX_CHECK},
    {typeid(PatternClause), PATTERN_REGEX_CHECK},
    {typeid(WithClause), WITH_REGEX_CHECK}};

/*
 * Class responsible for parsing the strings of query.
 */
class QueryParser {
public:
    template <typename QueryClass>
    static void parse(std::string *queryString, std::vector<Synonym> syns,
                      std::vector<QueryClause *> *clauses);
    template <typename QueryClass> static bool isClause(std::string *clause);
    static SolvableQuery parse(std::string query);
    static Declaration parseDeclaration(std::vector<std::string> clauses);
    static SelectClause parseSelectClause(std::string *queryString,
                                          std::vector<Synonym> syns);

    static SelectClause parseSelectTuple(std::string selectValue,
                                         std::vector<Synonym> syns);

    static std::vector<Synonym> parseSynonyms(std::vector<std::string> tokens);
};
