#include "PatternEvaluator.h"

ClauseResult PatternEvaluator::evaluate(PatternClause *patternCl) {
    if (patternCl->patternType == PatternType::EMPTY) {
        return ClauseResult(false);
    }
    else if (patternCl->patternType == PatternType::ASSIGN) {
        return ClauseResult(false);
    }
}