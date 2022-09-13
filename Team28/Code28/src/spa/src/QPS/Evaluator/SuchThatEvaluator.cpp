#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationship == RelationshipReference::EMPTY) {
        return ClauseResult(false);
    }
    else {
        Reference left = suchThatCl->refLeft;
        Reference right = suchThatCl->refRight;
        RelationshipReference relRef = suchThatCl->relationship;

        if (!left.isSynonym && !right.isSynonym) {
            return ClauseResult(queryFacade->validate(relRef, left, right));
        }
        else if (left.isSynonym && !right.isSynonym) {
            EntityName leftName = left.syn.entity;
            std::vector<Value> result = queryFacade->solveLeft(relRef, right, leftName);
            return ClauseResult();
        }
        else if (!left.isSynonym && right.isSynonym) {
            EntityName rightName = right.syn.entity;
            std::vector<Value> result = queryFacade->solveRight(relRef, left, rightName);
            return ClauseResult();
        }
        else if (left.isSynonym && right.isSynonym) {
            EntityName leftName = left.syn.entity;
            EntityName rightName = right.syn.entity;
            std::vector< std::pair<Value, Value> > result = queryFacade->solveBoth(relRef, leftName, rightName);
            return ClauseResult();
        }
    }
}