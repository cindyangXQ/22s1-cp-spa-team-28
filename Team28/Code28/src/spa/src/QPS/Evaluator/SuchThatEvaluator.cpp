#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationship == RelationshipReference::EMPTY) {
        return ClauseResult(false);
    } else {
        Reference left = suchThatCl->refLeft;
        Reference right = suchThatCl->refRight;
        RelationshipReference relRef = suchThatCl->relationship;

        if (relRef == RelationshipReference::FOLLOWS ||
            relRef == RelationshipReference::FOLLOWS_T ||
            relRef == RelationshipReference::PARENT ||
            relRef == RelationshipReference::PARENT_T ||
            relRef == RelationshipReference::USES ||
            relRef == RelationshipReference::MODIFIES ||
            relRef == RelationshipReference::CALLS ||
            relRef == RelationshipReference::CALLS_T ||
            relRef == RelationshipReference::NEXT) {
            if (left.isSynonym && right.isSynonym) {
                if (left.syn.entity == right.syn.entity &&
                    left.syn.name == right.syn.name) {
                    return ClauseResult(true);
                }
            }
        }

        if (!left.isSynonym && !right.isSynonym) {
            // the boolean argument is isEmpty, if validate returns true,
            // isEmpty should be false.
            return ClauseResult(!queryFacade->validate(relRef, left, right));
        } else if (left.isSynonym && !right.isSynonym) {
            ClauseResult clauseResult = ClauseResult(std::vector{left});
            EntityName leftName = left.syn.entity;
            std::vector<Value> result =
                queryFacade->solveLeft(relRef, right, leftName);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{result[i]}));
            }
            return clauseResult;
        } else if (!left.isSynonym && right.isSynonym) {
            ClauseResult clauseResult = ClauseResult(std::vector{right});
            EntityName rightName = right.syn.entity;
            std::vector<Value> result =
                queryFacade->solveRight(relRef, left, rightName);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{result[i]}));
            }
            return clauseResult;
        } else if (left.isSynonym && right.isSynonym) {
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
    }
}
