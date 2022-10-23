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
    static SolvableQuery parse(std::string query);
    static Declaration parseDeclaration(std::vector<std::string> clauses);
    static SelectClause parseSelectClause(std::string *clause,
                                          std::vector<Synonym> syns);

    template <typename QueryClass>
    static void parse(std::string *clause, std::vector<Synonym> syns,
                      std::unordered_map<std::type_index,
                                         std::vector<QueryClause *>> *clauses) {
        while (std::regex_search(
            *clause, IS_CLAUSE_AND_MAP.find(typeid(QueryClass))->second)) {
            std::smatch matches;
            std::regex_match(*clause, matches,
                             WHOLE_CLAUSE_MAP.find(typeid(QueryClass))->second);
            std::string extractedClause = matches[1];
            std::regex_match(extractedClause, matches,
                             ARG_CLAUSE_MAP.find(typeid(QueryClass))->second);
            if (matches.size() !=
                REGEX_CHECK_MAP.find(typeid(QueryClass))->second) {
                throw SyntaxError("Invalid clause syntax");
            }
            QueryClass queryClass;
            queryClass.parse(matches, syns);
            if (!queryClass.validate()) {
                throw SemanticError("Invalid clause semantic");
            }
            QueryClause *queryClause = &queryClass;

            *clause = Utils::removeString(*clause, extractedClause);
            if (clauses->count(typeid(QueryClass))) {
                std::vector<QueryClause *> queryClassList =
                    clauses->at(typeid(QueryClass));
                queryClassList.push_back(queryClause);
            } else {
                clauses->insert({typeid(QueryClass), {queryClause}});
            }
        }
    }

    static SelectClause parseSelectTuple(std::string selectValue,
                                         std::vector<Synonym> syns);

    template <typename QueryClass> static bool isClause(std::string *clause) {
        return std::regex_search(
            *clause, IS_CLAUSE_MAP.find(typeid(QueryClass))->second);
    };
    static std::vector<Synonym> parseSynonyms(std::vector<std::string> tokens);
};
