#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Clause/SuchThatClause.h"
#include "ClauseResult.h"

/*
 * Set of relationship that does not accept the same synonym as both arguments.
 */
const std::unordered_set<RelationshipReference> noSameSynonym = {
    RelationshipReference::FOLLOWS, RelationshipReference::FOLLOWS_T,
    RelationshipReference::PARENT,  RelationshipReference::PARENT_T,
    RelationshipReference::USES,    RelationshipReference::MODIFIES,
    RelationshipReference::CALLS,   RelationshipReference::CALLS,
    RelationshipReference::NEXT};

/*
 * Evaluator class for the such that clause.
 */
class SuchThatEvaluator {
private:
    QueryFacade *queryFacade;

public:
    SuchThatEvaluator(QueryFacade *queryFacade) : queryFacade(queryFacade){};
    ClauseResult evaluate(SuchThatClause *suchThatCl);

private:
    ClauseResult handleNoSynonym(RelationshipReference relRef, Reference left,
                                 Reference right);
    ClauseResult handleLeftSynonym(RelationshipReference relRef, Reference left,
                                   Reference right);
    ClauseResult handleRightSynonym(RelationshipReference relRef,
                                    Reference left, Reference right);
    ClauseResult handleBothSynonym(RelationshipReference relRef, Reference left,
                                   Reference right);
};
