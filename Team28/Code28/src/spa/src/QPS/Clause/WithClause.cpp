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
    this->populateSynsUsed();
}

bool WithClause::validate() {
    if (refLeft.isASynonym() || refRight.isASynonym()) {
        return false;
    }
    if (refLeft.getRefType() == ReferenceType::ATTR_REF &&
        refRight.getRefType() == ReferenceType::ATTR_REF) {
        if (refLeft.getAttr() == EntityAttribute::PROC_NAME ||
            refLeft.getAttr() == EntityAttribute::VAR_NAME) {
            return (refRight.getAttr() == EntityAttribute::PROC_NAME ||
                    refRight.getAttr() == EntityAttribute::VAR_NAME);
        } else {
            return (refRight.getAttr() == EntityAttribute::STMT_NO ||
                    refRight.getAttr() == EntityAttribute::VALUE);
        }
    } else if (refLeft.getRefType() == ReferenceType::ATTR_REF &&
               refRight.getRefType() != ReferenceType::ATTR_REF) {
        if (refLeft.getAttr() == EntityAttribute::PROC_NAME ||
            refLeft.getAttr() == EntityAttribute::VAR_NAME) {
            return refRight.getRefType() == ReferenceType::ENT_REF;
        } else {
            return refRight.getRefType() == ReferenceType::STMT_REF;
        }
    } else if (refLeft.getRefType() != ReferenceType::ATTR_REF &&
               refRight.getRefType() == ReferenceType::ATTR_REF) {
        if (refRight.getAttr() == EntityAttribute::PROC_NAME ||
            refRight.getAttr() == EntityAttribute::VAR_NAME) {
            return refLeft.getRefType() == ReferenceType::ENT_REF;
        } else {
            return refLeft.getRefType() == ReferenceType::STMT_REF;
        }
    } else {
        if (refLeft.getRefType() == ReferenceType::ENT_REF) {
            return refRight.getRefType() == ReferenceType::ENT_REF;
        } else if (refLeft.getRefType() == ReferenceType::STMT_REF) {
            return refRight.getRefType() == ReferenceType::STMT_REF;
        }
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

void WithClause::populateOptimizeScore(QueryFacade *queryFacade) {
    double multiplier = 0.01;
    if (synsUsed.size() == 0) {
        multiplier *= 0.01;
    }
    double baseScore = 0.0;
    if (this->refLeft.isASynonym()) {
        baseScore += queryFacade->getTableSize(this->refLeft.getDesignation());
    }
    if (this->refRight.isASynonym()) {
        baseScore += queryFacade->getTableSize(this->refRight.getDesignation());
    }
    if (multiplier * baseScore < 0) {
        this->score = INT_MAX;
    } else {
        this->score = multiplier * baseScore;
    }
}

double WithClause::getOptimizeScore() { return this->score; }

bool WithClause::replace(Reference synRef, Reference valRef) {
    bool replaced = false;
    if (refLeft.getRefType() == ReferenceType::ATTR_REF &&
        refLeft.getSynonymName() == synRef.getSynonymName() &&
        refLeft.getAttr() == synRef.getAttr()) {
        refLeft = valRef;
        replaced = true;
    }
    if (refRight.getRefType() == ReferenceType::ATTR_REF &&
        refRight.getSynonymName() == synRef.getSynonymName() &&
        refRight.getAttr() == synRef.getAttr()) {
        refRight = valRef;
        replaced = true;
    }
    return replaced;
}