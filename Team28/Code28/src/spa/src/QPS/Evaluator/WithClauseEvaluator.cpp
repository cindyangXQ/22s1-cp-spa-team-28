#include "WithClauseEvaluator.h"

// TOFIX: ensure all control paths return a value/throw exception
ClauseResult WithClauseEvaluator::evaluate(WithClause *withCl) {
    Reference left = withCl->refLeft;
    Reference right = withCl->refRight;
    if (left.type != ReferenceType::ATTR_REF &&
        right.type != ReferenceType::ATTR_REF) {
        return ClauseResult(left.value.value != right.value.value);
    }
}
