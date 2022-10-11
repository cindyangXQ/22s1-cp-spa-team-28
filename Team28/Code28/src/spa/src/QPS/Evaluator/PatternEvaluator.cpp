#include "PatternEvaluator.h"

ClauseResult PatternEvaluator::evaluate(PatternClause *patternCl) {
    if (patternCl->patternType == PatternType::EMPTY) {
        return ClauseResult(false);
    } else if (patternCl->patternType == PatternType::ASSIGN) {
        if (patternCl->entRef.isSynonym) {
            ClauseResult clauseResult = ClauseResult(
                std::vector{patternCl->syn, patternCl->entRef.syn});
            std::vector<std::pair<Value, Value>> result =
                queryFacade->getAssignAndVar(patternCl->expression,
                                             patternCl->isExact);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(
                    Tuple(std::vector{result[i].first, result[i].second}));
            }
            return clauseResult;
        } else {
            std::string expr = patternCl->expression;
            ClauseResult clauseResult =
                ClauseResult(std::vector{patternCl->syn});
            std::vector<Value> result = queryFacade->getAssign(
                patternCl->entRef.value.value, patternCl->expression,
                patternCl->isExact);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{result[i]}));
            }
            return clauseResult;
        }
    } else if (patternCl->patternType == PatternType::WHILE) {
        if (patternCl->entRef.isSynonym) {
            ClauseResult clauseResult = ClauseResult(
                std::vector{patternCl->syn, patternCl->entRef.syn});
            std::vector<std::pair<Value, Value>> result =
                queryFacade->getWhileAndVar();
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(
                    Tuple(std::vector{result[i].first, result[i].second}));
            }
            return clauseResult;
        } else {
            std::string expr = patternCl->expression;
            ClauseResult clauseResult =
                ClauseResult(std::vector{patternCl->syn});
            std::vector<Value> result =
                queryFacade->getWhile(patternCl->entRef.value.value);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{result[i]}));
            }
            return clauseResult;
        }
    } else if (patternCl->patternType == PatternType::IF) {
        if (patternCl->entRef.isSynonym) {
            ClauseResult clauseResult = ClauseResult(
                std::vector{patternCl->syn, patternCl->entRef.syn});
            std::vector<std::pair<Value, Value>> result =
                queryFacade->getIfAndVar();
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(
                    Tuple(std::vector{result[i].first, result[i].second}));
            }
            return clauseResult;
        } else {
            std::string expr = patternCl->expression;
            ClauseResult clauseResult =
                ClauseResult(std::vector{patternCl->syn});
            std::vector<Value> result =
                queryFacade->getIf(patternCl->entRef.value.value);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{result[i]}));
            }
            return clauseResult;
        }
    }
}
