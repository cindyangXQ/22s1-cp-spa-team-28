#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->getRelationship() == RelationshipReference::EMPTY) {
        return ClauseResult(false);
    } else {
        Reference left = suchThatCl->getRefLeft();
        Reference right = suchThatCl->getRefRight();
        RelationshipReference relRef = suchThatCl->getRelationship();

        if (!left.isASynonym() && !right.isASynonym()) {
            return handleNoSynonym(relRef, left, right);
        } else if (left.isASynonym() && !right.isASynonym()) {
            return handleLeftSynonym(relRef, left, right);
        } else if (!left.isASynonym() && right.isASynonym()) {
            return handleRightSynonym(relRef, left, right);
        } else if (left.isASynonym() && right.isASynonym()) {
            return handleBothSynonym(relRef, left, right);
        } else {
            return ClauseResult(true);
        }
    }
}

ClauseResult SuchThatEvaluator::handleNoSynonym(RelationshipReference relRef,
                                                Reference left,
                                                Reference right) {
    // the boolean argument is isEmpty, if validate returns true,
    // isEmpty should be false.
    bool isTrue = queryFacade->validate(relRef, left, right);
    bool isEmpty = !isTrue;
    return ClauseResult(isEmpty);
}

ClauseResult SuchThatEvaluator::handleLeftSynonym(RelationshipReference relRef,
                                                  Reference left,
                                                  Reference right) {
    ClauseResult clauseResult = ClauseResult({left});
    EntityName leftName = left.getEntityName();
    std::vector<Value> result = queryFacade->solveLeft(relRef, right, leftName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i]}));
    }
    return clauseResult;
}

ClauseResult SuchThatEvaluator::handleRightSynonym(RelationshipReference relRef,
                                                   Reference left,
                                                   Reference right) {
    ClauseResult clauseResult = ClauseResult({right});
    EntityName rightName = right.getEntityName();
    std::vector<Value> result =
        queryFacade->solveRight(relRef, left, rightName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i]}));
    }
    return clauseResult;
}

ClauseResult SuchThatEvaluator::handleBothSynonym(RelationshipReference relRef,
                                                  Reference left,
                                                  Reference right) {
    if (left.getEntityName() == right.getEntityName() &&
        left.getSynonymName() == right.getSynonymName()) {
        return ClauseResult(noSameSynonym.count(relRef));
    }
    ClauseResult clauseResult = ClauseResult({left, right});
    EntityName leftName = left.getEntityName();
    EntityName rightName = right.getEntityName();
    std::vector<std::pair<Value, Value>> result =
        queryFacade->solveBoth(relRef, leftName, rightName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple({result[i].first, result[i].second}));
    }
    return clauseResult;
}
