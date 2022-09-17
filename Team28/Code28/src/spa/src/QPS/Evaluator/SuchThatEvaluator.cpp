#include "SuchThatEvaluator.h"

ClauseResult SuchThatEvaluator::evaluate(SuchThatClause *suchThatCl) {
    if (suchThatCl->relationship == RelationshipReference::EMPTY) {
        return ClauseResult(false);
    } else {
        Reference left = suchThatCl->refLeft;
        Reference right = suchThatCl->refRight;
        RelationshipReference relRef = suchThatCl->relationship;

        if (!left.isSynonym && !right.isSynonym) {
            // the boolean argument is isEmpty, if validate returns true,
            // isEmpty should be false.
            return ClauseResult(!queryFacade->validate(relRef, left, right));
        } else if (left.isSynonym && !right.isSynonym) {
            ClauseResult clauseResult = ClauseResult(std::vector{left.syn});
            EntityName leftName = left.syn.entity;
            std::vector<Value> result =
                queryFacade->solveLeft(relRef, right, leftName);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{result[i]}));
            }
            return clauseResult;
        } else if (!left.isSynonym && right.isSynonym) {
            ClauseResult clauseResult = ClauseResult(std::vector{right.syn});
            EntityName rightName = right.syn.entity;
            std::vector<Value> result =
                queryFacade->solveRight(relRef, left, rightName);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{result[i]}));
            }
            return clauseResult;
        } else if (left.isSynonym && right.isSynonym) {
            ClauseResult clauseResult =
                ClauseResult(std::vector{left.syn, right.syn});
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
