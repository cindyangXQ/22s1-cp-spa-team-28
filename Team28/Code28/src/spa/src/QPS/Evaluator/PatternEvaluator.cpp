#include "PatternEvaluator.h"

// TOFIX: use polymorphism instead (subclass patternClause)
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

// all of this can be polymorphed under 1 handle method
ClauseResult PatternEvaluator::handleAssign(PatternClause *patternCl) {
    if (patternCl->getEntRef().isASynonym()) {
        ClauseResult clauseResult = ClauseResult(
            std::vector{Reference(patternCl->getSyn()), patternCl->getEntRef()});
        AssignExpression expression =
            AssignExpression(patternCl->getExpression(), patternCl->getIsExact());
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
        AssignExpression expression =
            AssignExpression(patternCl->getExpression(), patternCl->getIsExact());
        std::vector<Value> result = queryFacade->getAssign(
            patternCl->getEntRef().getValueString(), expression);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternEvaluator::handleWhile(PatternClause *patternCl) {
    if (patternCl->getEntRef().isASynonym()) {
        ClauseResult clauseResult = ClauseResult(
            std::vector{Reference(patternCl->getSyn()), patternCl->getEntRef()});
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
    if (patternCl->getEntRef().isASynonym()) {
        ClauseResult clauseResult = ClauseResult(
            std::vector{Reference(patternCl->getSyn()), patternCl->getEntRef()});
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
        std::vector<Value> result = queryFacade->getCond(
            Designation::IF_C, patternCl->getEntRef().getValueString());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}
