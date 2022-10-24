#include "SolvableQuery.h"

SolvableQuery::~SolvableQuery() {
    for (QueryClause *clause : clauses) {
        delete clause;
    }
}
Declaration SolvableQuery::getDecl() { return this->decl; }
SelectClause SolvableQuery::getSelectClause() { return this->selectClause; }
std::vector<QueryClause *> SolvableQuery::getQueryClause() {
    return this->clauses;
}