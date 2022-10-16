#include "SuchThatEvaluator.h"

// TOFIX: ensure all control paths return a value/throw exception
// TOFIX: inherit suchthatClause to polymorph handle
ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationship == RelationshipReference::EMPTY) {
        return ClauseResult(false);
    } else {
        Reference left = suchThatCl->refLeft;
        Reference right = suchThatCl->refRight;
        RelationshipReference relRef = suchThatCl->relationship;

        if (!left.isSynonym && !right.isSynonym) {
            return handleNoSynonym(relRef, left, right);
        } else if (left.isSynonym && !right.isSynonym) {
            return handleLeftSynonym(relRef, left, right);
        } else if (!left.isSynonym && right.isSynonym) {
            return handleRightSynonym(relRef, left, right);
        } else if (left.isSynonym && right.isSynonym) {
            return handleBothSynonym(relRef, left, right);
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
    // Possible improvement: overload constructor
    ClauseResult clauseResult = ClauseResult(std::vector{left});
    EntityName leftName = left.syn.entity;
    std::vector<Value> result = queryFacade->solveLeft(relRef, right, leftName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple(std::vector{result[i]}));
    }
    return clauseResult;
}

ClauseResult SuchThatEvaluator::handleRightSynonym(RelationshipReference relRef,
                                                   Reference left,
                                                   Reference right) {
    ClauseResult clauseResult = ClauseResult(std::vector{right});
    EntityName rightName = right.syn.entity;
    std::vector<Value> result =
        queryFacade->solveRight(relRef, left, rightName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(Tuple(std::vector{result[i]}));
    }
    return clauseResult;
}

ClauseResult SuchThatEvaluator::handleBothSynonym(RelationshipReference relRef,
                                                  Reference left,
                                                  Reference right) {
    if (left.syn.entity == right.syn.entity &&
        left.syn.name == right.syn.name) {
        return ClauseResult(noSameSynonym.count(relRef));
    }
    ClauseResult clauseResult = ClauseResult(std::vector{left, right});
    EntityName leftName = left.syn.entity;
    EntityName rightName = right.syn.entity;
    std::vector<std::pair<Value, Value>> result =
        queryFacade->solveBoth(relRef, leftName, rightName);
    for (int i = 0; i < result.size(); i++) {
        clauseResult.insert(
            Tuple(std::vector{result[i].first, result[i].second}));
    }
    return clauseResult;
}
