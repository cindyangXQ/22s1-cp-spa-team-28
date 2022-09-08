#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationship == RelationshipType::EMPTY) {
        return ClauseResult(true);
    }
}