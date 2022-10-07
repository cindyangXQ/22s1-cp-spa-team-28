#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Parser/SolvableQuery.h"
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
    std::vector<std::string> getAll(Synonym select);
};
