#include "WithClauseEvaluator.h"

ClauseResult WithClauseEvaluator::evaluate(WithClause *withCl) {
    Reference left = withCl->getRefLeft();
    Reference right = withCl->getRefRight();
    if (left.getRefType() != ReferenceType::ATTR_REF &&
        right.getRefType() != ReferenceType::ATTR_REF) {
        return ClauseResult(left.getValueString() != right.getValueString());
    }
    return ClauseResult(true);
}
