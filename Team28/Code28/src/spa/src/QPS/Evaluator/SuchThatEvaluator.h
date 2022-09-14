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
    explicit SuchThatEvaluator(QueryFacade *queryFacade) : queryFacade(queryFacade) {};
    ClauseResult evaluate(SuchThatClause *suchThatCl);
    bool validate(SuchThatClause *suchThatCl);
};