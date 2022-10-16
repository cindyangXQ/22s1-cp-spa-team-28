#include "PatternEvaluator.h"

ClauseResult PatternEvaluator::evaluate(PatternClause *patternCl) {
    if (patternCl->patternType == PatternType::ASSIGN) {
        return handleAssign(patternCl);
    } else if (patternCl->patternType == PatternType::WHILE) {
        return handleWhile(patternCl);
    } else if (patternCl->patternType == PatternType::IF) {
        return handleIf(patternCl);
    }
}

ClauseResult PatternEvaluator::handleAssign(PatternClause *patternCl) {
    if (patternCl->entRef.isSynonym) {
        ClauseResult clauseResult = ClauseResult(
            std::vector{Reference(patternCl->syn), patternCl->entRef});
        std::vector<std::pair<Value, Value>> result =
            patternCl->isExact
                ? queryFacade->getAssignAndVarExact(patternCl->expression)
                : queryFacade->getAssignAndVar(patternCl->expression);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        std::string expr = patternCl->expression;
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(patternCl->syn)});
        std::vector<Value> result =
            patternCl->isExact
                ? queryFacade->getAssignExact(patternCl->entRef.value.value,
                                              patternCl->expression)
                : queryFacade->getAssign(patternCl->entRef.value.value,
                                         patternCl->expression);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternEvaluator::handleWhile(PatternClause *patternCl) {
    if (patternCl->entRef.isSynonym) {
        ClauseResult clauseResult = ClauseResult(
            std::vector{Reference(patternCl->syn), patternCl->entRef});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(Designation::WHILE_C);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        std::string expr = patternCl->expression;
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(patternCl->syn)});
        std::vector<Value> result = queryFacade->getCond(
            Designation::WHILE_C, patternCl->entRef.value.value);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternEvaluator::handleIf(PatternClause *patternCl) {
    if (patternCl->entRef.isSynonym) {
        ClauseResult clauseResult = ClauseResult(
            std::vector{Reference(patternCl->syn), patternCl->entRef});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(Designation::IF_C);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        std::string expr = patternCl->expression;
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(patternCl->syn)});
        std::vector<Value> result = queryFacade->getCond(
            Designation::IF_C, patternCl->entRef.value.value);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}
