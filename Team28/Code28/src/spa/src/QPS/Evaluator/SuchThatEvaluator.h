#pragma once

#include "../../PKB/QueryFacade.h"
#include "../../commons/Relationship/Follows.h"
#include "../../commons/Relationship/Modifies.h"
#include "../Clause/SuchThatClause.h"
#include "RelationshipEvaluator.h"
#include "ClauseResult.h"

/*
 * Evaluator class for the such that clause.
 */
class SuchThatEvaluator {
private:
    QueryFacade *queryFacade;
    RelationshipEvaluator relationshipEvaluator;
public:
    explicit SuchThatEvaluator(QueryFacade *queryFacade) : 
    queryFacade(queryFacade),
    relationshipEvaluator(queryFacade) {};
    ClauseResult evaluate(SuchThatClause *suchThatCl);
};