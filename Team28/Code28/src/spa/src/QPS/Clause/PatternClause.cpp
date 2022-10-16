#include "PatternClause.h"

PatternClause::PatternClause(Synonym syn, Reference entRef,
                             Expression expression, bool isExact) {
    this->patternType = entityToPatternTypeMap.find(syn.entity)->second;
    this->syn = syn;
    this->entRef = entRef;
    this->expression = expression;
    this->isExact = isExact;
}
