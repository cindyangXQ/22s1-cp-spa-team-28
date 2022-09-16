#include "PatternEvaluator.h"

ClauseResult PatternEvaluator::evaluate(PatternClause *patternCl) {
    if (patternCl->patternType == PatternType::EMPTY) {
        return ClauseResult(false);
    }
    else if (patternCl->patternType == PatternType::ASSIGN) {
        if (patternCl->entRef.isSynonym) {
            ClauseResult clauseResult = ClauseResult(std::vector{ patternCl->syn, patternCl->entRef.syn });
            std::vector< std::pair<Value, Value> > result = queryFacade->getAssignAndVar(patternCl->expression);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{ result[i].first, result[i].second}));
            }
            return clauseResult;
        }
        else {
            ClauseResult clauseResult = ClauseResult(std::vector{ patternCl->syn });
            std::string varName = patternCl->entRef.value.value;
            std::vector<Value> result = queryFacade->getAssign(varName, patternCl->expression);
            for (int i = 0; i < result.size(); i++) {
                clauseResult.insert(Tuple(std::vector{ result[i] }));
            }
            return clauseResult;
        }
    }
}