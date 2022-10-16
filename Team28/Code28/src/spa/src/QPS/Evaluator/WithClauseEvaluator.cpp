#include "WithClauseEvaluator.h"

ClauseResult WithClauseEvaluator::evaluate(WithClause *withCl) {
    Reference left = withCl->refLeft;
    Reference right = withCl->refRight;
    if (left.getRefType() != ReferenceType::ATTR_REF &&
        right.getRefType() != ReferenceType::ATTR_REF) {
        return ClauseResult(left.getValueString() != right.getValueString());
    }
}
