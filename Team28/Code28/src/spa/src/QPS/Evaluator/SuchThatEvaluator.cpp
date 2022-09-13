#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationship == RelationshipReference::EMPTY) {
        return ClauseResult(false);
    }
    else if (suchThatCl->relationship == RelationshipReference::FOLLOWS) {
        Follows follows = Follows(suchThatCl->relationship, suchThatCl->refLeft, suchThatCl->refRight);
        return follows.evaluate();
    }
    else if (suchThatCl->relationship == RelationshipReference::MODIFIES) {
        Modifies modifies = Modifies(suchThatCl->relationship, suchThatCl->refLeft, suchThatCl->refRight);
        return modifies.evaluate();
    }
}