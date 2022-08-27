#include "PatternClause.h"

PatternClause::PatternClause() {}
PatternClause::PatternClause(Synonym syn, EntityReference entRef, StatementReference stmtRef) {
    this->syn = syn;
    this->entRef = entRef;
    this->stmtRef = stmtRef;
}