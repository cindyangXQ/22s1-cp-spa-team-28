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
    explicit PatternEvaluator(QueryFacade *queryFacade)
        : queryFacade(queryFacade){};
    ClauseResult evaluate(PatternClause *patternCl);
};
