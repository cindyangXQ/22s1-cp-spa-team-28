#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationshipType == RelationshipReference::Empty) {
        return ClauseResult(true);
    }
}