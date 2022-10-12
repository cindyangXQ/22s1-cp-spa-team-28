#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Parser/SolvableQuery.h"
#include "../Utils.h"
#include "PatternEvaluator.h"
#include "QueryResult.h"
#include "SuchThatEvaluator.h"

class QueryEvaluator {
private:
    QueryFacade *queryFacade;
    SuchThatEvaluator suchThatEvaluator;
    PatternEvaluator patternEvaluator;

public:
    explicit QueryEvaluator(QueryFacade *queryFacade)
        : queryFacade(queryFacade),
          suchThatEvaluator(SuchThatEvaluator(queryFacade)),
          patternEvaluator(PatternEvaluator(queryFacade)){};
    QueryResult evaluate(SolvableQuery *solvableQ);
    std::vector<std::string> interpretQueryResult(QueryResult *queryResult);

private:
    void checkAllClauseResult(std::vector<ClauseResult> clauseResultList,
                              bool *isAnyTableEmpty, bool *haveTableToJoin);
    std::vector<std::string> extractSynonymFromTable(Synonym selectedSynonym,
                                                     ClauseTable result);
    std::vector<std::string> extractTuplesFromTable(std::vector<Synonym> selectSynonyms,
                                                     ClauseTable result);
    std::vector<std::string> getAll(Synonym select);
    std::vector<std::string> handleNoTables(QueryResult *queryResult);
    ClauseTable JoinAllClauseTables(std::vector<ClauseResult> clauseResultList);
};
