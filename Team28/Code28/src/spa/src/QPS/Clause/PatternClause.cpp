#include "PatternClause.h"

// TOFIX: maybe use map to handle if-else. Even better would be to polymorph
// PatternClause and avoid switching on EntityName at all
PatternClause::PatternClause(Synonym syn, Reference entRef,
                             Expression expression, bool isExact) {
    if (syn.entity == EntityName::ASSIGN) {
        this->patternType = PatternType::ASSIGN;
    } else if (syn.entity == EntityName::WHILE) {
        this->patternType = PatternType::WHILE;
    } else if (syn.entity == EntityName::IF) {
        this->patternType = PatternType::IF;
    }
    this->syn = syn;
    this->entRef = entRef;
    this->expression = expression;
    this->isExact = isExact;
}
