#include "PatternClause.h"

PatternClause::PatternClause(Synonym syn, Reference entRef,
                             Expression expression, bool isExact) {
    EntityName entity = syn.getEntityName();
    this->patternType = entityToPatternTypeMap.find(entity)->second;
    this->syn = syn;
    this->entRef = entRef;
    this->expression = expression;
    this->isExact = isExact;
}

Synonym PatternClause::getSyn() { return this->syn; }
PatternType PatternClause::getPatternType() { return this->patternType; }
Reference PatternClause::getEntRef() { return this->entRef; }
Expression PatternClause::getExpression() { return this->expression; }
bool PatternClause::getIsExact() { return this->isExact; }
