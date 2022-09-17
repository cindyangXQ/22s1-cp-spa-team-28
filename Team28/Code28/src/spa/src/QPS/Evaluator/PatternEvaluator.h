#pragma once

#include "../../PKB/QueryFacade.h"
#include "../Clause/PatternClause.h"
#include "ClauseResult.h"

/*
 * Evaluator for the pattern clause.
 */
class PatternEvaluator {
private:
    QueryFacade *queryFacade;
public:
    explicit PatternEvaluator(QueryFacade *queryFacade) : queryFacade(queryFacade) {};
    std::vector<ClauseResult> evaluate(PatternClause *patternCl);
};