#include "RelationshipEvaluator.h"

ClauseResult RelationshipEvaluator::evaluate(Relationship<Reference, Reference> relationship) {
    Reference left = relationship.getLeft();
    Reference right = relationship.getRight();
    if (!left.isSynonym && !right.isSynonym) {
        return ClauseResult(QueryFacade::validate(relationship.getRelationshipReference(), left, right));
    }
    else if (left.isSynonym && !right.isSynonym) {
        EntityName leftName = left.syn.entity;
        std::vector<Value> result = QueryFacade::solveLeft(relationship.getRelationshipReference(), leftName, right);
    }
    else if (!left.isSynonym && right.isSynonym) {
        EntityName rightName = right.syn.entity;
        std::vector<Value> result = QueryFacade::solveRight(relationship.getRelationshipReference(), left, rightName);
    }
    else if (left.isSynonym && right.isSynonym) {
        EntityName leftName = left.syn.entity;
        EntityName rightName = right.syn.entity;
        std::vector<Value> result = QueryFacade::solveBoth(relationship.getRelationshipReference(), leftName, rightName);
    }
}