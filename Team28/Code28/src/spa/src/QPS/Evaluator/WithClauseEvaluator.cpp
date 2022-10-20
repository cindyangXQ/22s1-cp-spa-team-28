#include "WithClauseEvaluator.h"

ClauseResult WithClauseEvaluator::evaluate(WithClause *withCl) {
    Reference left = withCl->getRefLeft();
    Reference right = withCl->getRefRight();
    if (left.getRefType() != ReferenceType::ATTR_REF &&
        right.getRefType() != ReferenceType::ATTR_REF) {
        return ClauseResult(left.getValueString() != right.getValueString());
    } else if (left.getRefType() == ReferenceType::ATTR_REF &&
               right.getRefType() == ReferenceType::ATTR_REF) {
        return handleBothSynonyms(left, right);
    } else {
        return handleOneSynonym(left, right);
    }
}

ClauseResult WithClauseEvaluator::handleBothSynonyms(Reference left,
                                                     Reference right) {
    ClauseResult clauseResult = ClauseResult({left, right});
    std::vector<std::pair<Value, Value>> result =
        this->queryFacade->solveBothAttribute(left, right);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i].first, result[i].second}));
    }
    return clauseResult;
}

ClauseResult WithClauseEvaluator::handleOneSynonym(Reference left,
                                                   Reference right) {
    Reference r;
    Value v;
    if (left.getRefType() == ReferenceType::ATTR_REF) {
        r = left;
        v = right.getValue();
    } else {
        r = right;
        v = left.getValue();
    }
    ClauseResult clauseResult = ClauseResult({r});
    std::vector<Value> result = this->queryFacade->solveOneAttribute(r, v);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i]}));
    }
    return clauseResult;
}