#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Clause/PatternClause.h"
#include "ClauseResult.h"

/*
 * Evaluator for the pattern clause.
 */
class PatternEvaluator {
private:
    QueryFacade *queryFacade;

public:
    PatternEvaluator(QueryFacade *queryFacade) : queryFacade(queryFacade){};
    ClauseResult evaluate(PatternClause *patternCl);

private:
    ClauseResult handleAssign(PatternClause *patternCl);
    ClauseResult handleIf(PatternClause *patternCl);
    ClauseResult handleWhile(PatternClause *patternCl);
};
