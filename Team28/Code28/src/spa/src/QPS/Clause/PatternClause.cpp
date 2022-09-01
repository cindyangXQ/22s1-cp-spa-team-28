#include "PatternClause.h"

PatternClause::PatternClause() {}
PatternClause::PatternClause(std::string desc) {}
PatternClause::PatternClause(Synonym syn, Reference entRef, Reference stmtRef) {
    this->syn = syn;
    this->entRef = entRef;
    this->stmtRef = stmtRef;
}