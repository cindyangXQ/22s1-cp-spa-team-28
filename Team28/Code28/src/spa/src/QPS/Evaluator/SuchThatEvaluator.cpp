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
            // the boolean argument is isEmpty, if validate returns true, isEmpty should be false.
            return ClauseResult(!queryFacade->validate(relRef, left, right));
        }
        else if (left.isSynonym && !right.isSynonym) {
            ClauseResult clauseResult = ClauseResult(std::vector{ left.syn });
            EntityName leftName = left.syn.entity;
            std::vector<Value> result = queryFacade->solveLeft(relRef, right, leftName);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{ result[i] }));
            }
            return clauseResult;
        }
        else if (!left.isSynonym && right.isSynonym) {          
            ClauseResult clauseResult = ClauseResult(std::vector{ right.syn });
            EntityName rightName = right.syn.entity;
            std::vector<Value> result = queryFacade->solveRight(relRef, left, rightName);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{ result[i] }));
            }
            return clauseResult;
        }
        else if (left.isSynonym && right.isSynonym) {
            ClauseResult clauseResult = ClauseResult(std::vector{ left.syn, right.syn });
            EntityName leftName = left.syn.entity;
            EntityName rightName = right.syn.entity;
            std::vector< std::pair<Value, Value> > result = queryFacade->solveBoth(relRef, leftName, rightName);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{ result[i].first, result[i].second}));
            }
            return clauseResult;
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
        return (suchThatCl->refLeft.type != ReferenceType::ENT_REF && suchThatCl->refRight.type != ReferenceType::ENT_REF);
    }
    else if (relRef == RelationshipReference::USES || relRef == RelationshipReference::MODIFIES) {
        if (suchThatCl->refLeft.type == ReferenceType::WILDCARD) {
            return false;
        }
        return (suchThatCl->refRight.type == ReferenceType::ENT_REF);
    }
}