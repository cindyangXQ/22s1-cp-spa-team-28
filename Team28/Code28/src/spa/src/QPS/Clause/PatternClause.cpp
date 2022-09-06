#include "PatternClause.h"

PatternClause::PatternClause() {}
PatternClause::PatternClause(Synonym syn, Reference entRef, Expression expression) {
    this->syn = syn;
    this->entRef = entRef;
    this->expression = expression;
}