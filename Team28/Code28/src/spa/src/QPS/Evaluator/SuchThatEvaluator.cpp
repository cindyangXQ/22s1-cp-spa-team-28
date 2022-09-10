#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationship == RelationshipReference::EMPTY) {
        return ClauseResult(true);
    }
}