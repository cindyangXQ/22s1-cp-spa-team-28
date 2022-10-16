#include "PatternClause.h"

PatternClause::PatternClause(Synonym syn, Reference entRef,
                             Expression expression, bool isExact) {
    if (syn.getEntityName() == EntityName::ASSIGN) {
        this->patternType = PatternType::ASSIGN;
    } else if (syn.getEntityName() == EntityName::WHILE) {
        this->patternType = PatternType::WHILE;
    } else if (syn.getEntityName() == EntityName::IF) {
        this->patternType = PatternType::IF;
    }
    this->syn = syn;
    this->entRef = entRef;
    this->expression = expression;
    this->isExact = isExact;
}
