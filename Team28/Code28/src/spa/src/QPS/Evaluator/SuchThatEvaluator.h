#pragma once

#include "../../PKB/QueryFacade.h"
#include "../Clause/SuchThatClause.h"
#include "ClauseResult.h"

/*
 * Evaluator class for the such that clause.
 */
class SuchThatEvaluator {
private:
    QueryFacade *queryFacade;
public:
    explicit SuchThatEvaluator(QueryFacade *queryFacade);
    ClauseResult evaluate(SuchThatClause *suchThatCl);
};