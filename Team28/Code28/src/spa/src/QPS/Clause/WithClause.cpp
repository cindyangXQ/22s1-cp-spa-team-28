#include "WithClause.h"

Reference WithClause::getRefLeft() { return this->refLeft; }
Reference WithClause::getRefRight() { return this->refRight; }

void WithClause::parse(std::smatch matches, std::vector<Synonym> syns) {
    this->refLeft = Reference::getReference(matches[2], syns);
    this->refRight = Reference::getReference(matches[3], syns);
    if (refLeft.getRefType() == ReferenceType::WILDCARD ||
        refRight.getRefType() == ReferenceType::WILDCARD) {
        throw SyntaxError("With clause arguments cannot be wildcards");
    }
}

bool WithClause::validate() {
    if (refLeft.isASynonym() || refRight.isASynonym()) {
        return false;
    }
    if (refLeft.getAttr() == EntityAttribute::PROC_NAME ||
        refLeft.getAttr() == EntityAttribute::VAR_NAME ||
        refLeft.getRefType() == ReferenceType::ENT_REF) {
        return (refRight.getAttr() == EntityAttribute::PROC_NAME ||
                refRight.getAttr() == EntityAttribute::VAR_NAME ||
                refRight.getRefType() == ReferenceType::ENT_REF);
    } else if (refLeft.getAttr() == EntityAttribute::STMT_NO ||
               refLeft.getAttr() == EntityAttribute::VALUE ||
               refLeft.getRefType() == ReferenceType::STMT_REF) {
        return (refRight.getAttr() == EntityAttribute::STMT_NO ||
                refRight.getAttr() == EntityAttribute::VALUE ||
                refRight.getRefType() == ReferenceType::STMT_REF);
    } else {
        return false;
    }
}

ClauseResult WithClause::evaluate(QueryFacade *queryFacade) {
    if (refLeft.getRefType() != ReferenceType::ATTR_REF &&
        refRight.getRefType() != ReferenceType::ATTR_REF) {
        return ClauseResult(refLeft.getValueString() !=
                            refRight.getValueString());
    } else if (refLeft.getRefType() == ReferenceType::ATTR_REF &&
               refRight.getRefType() == ReferenceType::ATTR_REF) {
        return handleBothSynonyms(queryFacade);
    } else {
        return handleOneSynonym(queryFacade);
    }
}

ClauseResult WithClause::handleBothSynonyms(QueryFacade *queryFacade) {
    ClauseResult clauseResult = ClauseResult({refLeft, refRight});
    std::vector<std::pair<Value, Value>> result =
        queryFacade->solveBothAttribute(refLeft, refRight);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i].first, result[i].second}));
    }
    return clauseResult;
}

ClauseResult WithClause::handleOneSynonym(QueryFacade *queryFacade) {
    Reference r;
    Value v;
    if (refLeft.getRefType() == ReferenceType::ATTR_REF) {
        r = refLeft;
        v = refRight.getValue();
    } else {
        r = refRight;
        v = refLeft.getValue();
    }
    ClauseResult clauseResult = ClauseResult({r});
    std::vector<Value> result = queryFacade->solveOneAttribute(r, v);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i]}));
    }
    return clauseResult;
}

void WithClause::replaceFirstReference(Reference *newRef) {
    this->refLeft = *newRef;
}

void WithClause::replaceSecondReference(Reference *newRef) {
    this->refRight = *newRef;
}

std::unordered_set<std::string> WithClause::getSynonymsUsed() {
    return this->synsUsed;
}

void WithClause::populateSynsUsed() {
    if (refLeft.getRefType() == ReferenceType::ATTR_REF) {
        synsUsed.insert(refLeft.getSynonymName());
    }
    if (refLeft.getRefType() == ReferenceType::ATTR_REF) {
        synsUsed.insert(refRight.getSynonymName());
    }
}

double WithClause::getOptimizeScore() {
    double baseScore = 0.25;
    double synScore = 1.0;
    if (this->synsUsed.size() == 0) {
        synScore = 0.01;
    } else if (this->synsUsed.size() == 1) {
        synScore = 0.5;
    }
    return baseScore * synScore;
}