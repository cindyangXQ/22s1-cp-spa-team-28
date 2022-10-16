#include "PatternEvaluator.h"

ClauseResult PatternEvaluator::evaluate(PatternClause *patternCl) {
    if (patternCl->getPatternType() == PatternType::ASSIGN) {
        return handleAssign(patternCl);
    } else if (patternCl->getPatternType() == PatternType::WHILE) {
        return handleWhile(patternCl);
    } else if (patternCl->getPatternType() == PatternType::IF) {
        return handleIf(patternCl);
    } else {
        return ClauseResult(true);
    }
}

ClauseResult PatternEvaluator::handleAssign(PatternClause *patternCl) {
    Reference entRef = patternCl->getEntRef();
    if (entRef.isASynonym()) {
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(patternCl->getSyn()), entRef});
        AssignExpression expression = AssignExpression(
            patternCl->getExpression(), patternCl->getIsExact());
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getAssignAndVar(expression);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        std::string expr = patternCl->getExpression();
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(patternCl->getSyn())});
        AssignExpression expression = AssignExpression(
            patternCl->getExpression(), patternCl->getIsExact());
        std::vector<Value> result =
            queryFacade->getAssign(entRef.getValueString(), expression);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternEvaluator::handleWhile(PatternClause *patternCl) {
    if (patternCl->getEntRef().isASynonym()) {
        ClauseResult clauseResult = ClauseResult(std::vector{
            Reference(patternCl->getSyn()), patternCl->getEntRef()});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(Designation::WHILE_C);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        std::string expr = patternCl->getExpression();
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(patternCl->getSyn())});
        std::vector<Value> result = queryFacade->getCond(
            Designation::WHILE_C, patternCl->getEntRef().getValueString());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternEvaluator::handleIf(PatternClause *patternCl) {
    Reference entRef = patternCl->getEntRef();
    if (entRef.isASynonym()) {
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(patternCl->getSyn()), entRef});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(Designation::IF_C);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        std::string expr = patternCl->getExpression();
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(patternCl->getSyn())});
        std::vector<Value> result =
            queryFacade->getCond(Designation::IF_C, entRef.getValueString());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}
