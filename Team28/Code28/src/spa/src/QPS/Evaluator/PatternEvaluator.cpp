#include "PatternEvaluator.h"

ClauseResult PatternEvaluator::evaluate(PatternClause *patternCl) {
    if (patternCl->patternType == PatternType::EMPTY) {
        return ClauseResult(false);
    }
    else if (patternCl->patternType == PatternType::ASSIGN) {
        if (patternCl->entRef.isSynonym) {
            std::string expr = patternCl->expression;
            if (expr.find('_') != std::string::npos && expr.size() >= 5) {
                expr = expr.substr(2, expr.size() - 4);
            }
            ClauseResult clauseResult = ClauseResult(std::vector{ patternCl->syn, patternCl->entRef.syn });
            std::vector< std::pair<Value, Value> > result = queryFacade->getAssignAndVar(expr);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{ result[i].first, result[i].second}));
            }
            return clauseResult;
        }
        else {
            std::string expr = patternCl->expression;
            if (expr.find('_') != std::string::npos && expr.size() >= 5) {
                expr = expr.substr(2, expr.size() - 4);
            }
            ClauseResult clauseResult = ClauseResult(std::vector{ patternCl->syn });
            std::vector<Value> result = queryFacade->getAssign(patternCl->entRef.value.value, expr);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{ result[i] }));
            }
            return clauseResult;
        }
    }
}