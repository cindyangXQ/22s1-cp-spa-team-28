#include "SuchThatClause.h"

RelationshipReference SuchThatClause::getRelationship() {
    return this->relationship;
}
Reference SuchThatClause::getRefLeft() { return this->refLeft; }
Reference SuchThatClause::getRefRight() { return this->refRight; }

void SuchThatClause::parse(std::smatch matches, std::vector<Synonym> syns) {
    this->relationship = RELATIONSHIP_MAP.at(matches[2]);
    this->refLeft = Reference::getReference(matches[3], syns);
    this->refRight = Reference::getReference(matches[4], syns);
}

bool SuchThatClause::validate() {
    if (relationship == RelationshipReference::EMPTY) {
        return true;
    }
    bool isLeftValid;
    bool isRightValid;
    std::unordered_set<EntityName> validLeftArg =
        RELATIONSHIP_LEFT_ARG_MAP.at(relationship);
    std::unordered_set<EntityName> validRightArg =
        RELATIONSHIP_RIGHT_ARG_MAP.at(relationship);
    std::unordered_set<ReferenceType> validLeftRef =
        RELATIONSHIP_LEFT_REF_MAP.at(relationship);
    std::unordered_set<ReferenceType> validRightRef =
        RELATIONSHIP_RIGHT_REF_MAP.at(relationship);

    if (refLeft.isASynonym()) {
        isLeftValid = validLeftArg.count(refLeft.getEntityName());
    } else {
        isLeftValid = validLeftRef.count(refLeft.getRefType());
    }
    if (refRight.isASynonym()) {
        isRightValid = validRightArg.count(refRight.getEntityName());
    } else {
        isRightValid = validRightRef.count(refRight.getRefType());
    }
    return isLeftValid && isRightValid;
}

ClauseResult SuchThatClause::evaluate(QueryFacade *queryFacade) {
    if (relationship == RelationshipReference::EMPTY) {
        return ClauseResult(false);
    } else {
        if (!refLeft.isASynonym() && !refRight.isASynonym()) {
            return handleNoSynonym(queryFacade);
        } else if (refLeft.isASynonym() && !refRight.isASynonym()) {
            return handleLeftSynonym(queryFacade);
        } else if (!refLeft.isASynonym() && refRight.isASynonym()) {
            return handleRightSynonym(queryFacade);
        } else if (refLeft.isASynonym() && refRight.isASynonym()) {
            return handleBothSynonym(queryFacade);
        } else {
            return ClauseResult(true);
        }
    }
}

ClauseResult SuchThatClause::handleNoSynonym(QueryFacade *queryFacade) {
    // the boolean argument is isEmpty, if validate returns true,
    // isEmpty should be false.
    bool isTrue = queryFacade->validate(relationship, refLeft, refRight);
    bool isEmpty = !isTrue;
    return ClauseResult(isEmpty);
}

ClauseResult SuchThatClause::handleLeftSynonym(QueryFacade *queryFacade) {
    ClauseResult clauseResult = ClauseResult({refLeft});
    EntityName leftName = refLeft.getEntityName();
    std::vector<Value> result =
        queryFacade->solveLeft(relationship, refRight, leftName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i]}));
    }
    return clauseResult;
}

ClauseResult SuchThatClause::handleRightSynonym(QueryFacade *queryFacade) {
    ClauseResult clauseResult = ClauseResult({refRight});
    EntityName rightName = refRight.getEntityName();
    std::vector<Value> result =
        queryFacade->solveRight(relationship, refLeft, rightName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i]}));
    }
    return clauseResult;
}

ClauseResult SuchThatClause::handleBothSynonym(QueryFacade *queryFacade) {
    if (relationship == RelationshipReference::NEXT_T &&
        refLeft.getSynonymName() == refRight.getSynonymName()) {
        // TODO: clean up this if block
        ClauseResult clauseResult = ClauseResult({refLeft, refRight});

        std::vector<Value> result = queryFacade->solveReflexive(
            RelationshipReference::NEXT_T, refLeft.getEntityName());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple({result[i], result[i]}));
        }
        return clauseResult;
    }

    if (relationship == RelationshipReference::AFFECTS &&
        refLeft.getSynonymName() == refRight.getSynonymName()) {
        // TODO: clean up this if block
        ClauseResult clauseResult = ClauseResult({refLeft, refRight});

        std::vector<Value> result = queryFacade->solveReflexive(
            RelationshipReference::AFFECTS, refLeft.getEntityName());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple({result[i], result[i]}));
        }
        return clauseResult;
    }

    if (relationship == RelationshipReference::AFFECTS_T &&
        refLeft.getSynonymName() == refRight.getSynonymName()) {
        // TODO: clean up this if block
        ClauseResult clauseResult = ClauseResult({refLeft, refRight});

        std::vector<Value> result = queryFacade->solveReflexive(
            RelationshipReference::AFFECTS_T, refLeft.getEntityName());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple({result[i], result[i]}));
        }
        return clauseResult;
    }

    if (refLeft.getEntityName() == refRight.getEntityName() &&
        refLeft.getSynonymName() == refRight.getSynonymName()) {
        return ClauseResult(noSameSynonym.count(relationship));
    }
    ClauseResult clauseResult = ClauseResult({refLeft, refRight});
    EntityName leftName = refLeft.getEntityName();
    EntityName rightName = refRight.getEntityName();
    std::vector<std::pair<Value, Value>> result =
        queryFacade->solveBoth(relationship, leftName, rightName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i].first, result[i].second}));
    }
    return clauseResult;
}
