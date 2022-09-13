#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationship == RelationshipReference::EMPTY || !validate(suchThatCl)) {
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
            return ClauseResult(false);
        }
        else if (!left.isSynonym && right.isSynonym) {
            EntityName rightName = right.syn.entity;
            std::vector<Value> result = queryFacade->solveRight(relRef, left, rightName);
            return ClauseResult(false);
        }
        else if (left.isSynonym && right.isSynonym) {
            EntityName leftName = left.syn.entity;
            EntityName rightName = right.syn.entity;
            std::vector< std::pair<Value, Value> > result = queryFacade->solveBoth(relRef, leftName, rightName);
            return ClauseResult(false);
        }
    }
}

bool SuchThatEvaluator::validate(SuchThatClause *suchThatCl) {
    RelationshipReference relRef = suchThatCl->relationship;
    if (relRef == RelationshipReference::EMPTY) {
        return true;
    }
    else if (relRef == RelationshipReference::FOLLOWS 
        || relRef == RelationshipReference::FOLLOWS_T 
        || relRef == RelationshipReference::PARENT 
        || relRef == RelationshipReference::PARENT_T) {
        return (suchThatCl->refLeft.type == ReferenceType::STMT_REF && suchThatCl->refRight.type == ReferenceType::STMT_REF);
    }
    else if (relRef == RelationshipReference::USES || relRef == RelationshipReference::MODIFIES) {
        if (suchThatCl->refLeft.type == ReferenceType::WILDCARD) {
            return false;
        }
        return (suchThatCl->refRight.type == ReferenceType::ENT_REF);
    }
}