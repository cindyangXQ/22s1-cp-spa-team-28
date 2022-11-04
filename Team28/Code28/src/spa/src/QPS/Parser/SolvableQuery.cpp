#include "SolvableQuery.h"

SolvableQuery::~SolvableQuery() {
    for (QueryClause *clause : clauses) {
        delete clause;
    }
    for (QueryClause *clause : withCls) {
        delete clause;
    }
}
Declaration SolvableQuery::getDecl() { return this->decl; }
SelectClause SolvableQuery::getSelectClause() { return this->selectClause; }
std::vector<QueryClause *> SolvableQuery::getQueryClause() {
    return this->clauses;
}
std::vector<QueryClause *> SolvableQuery::getWithClause() {
    return this->withCls;
}
std::vector<std::vector<QueryClause *>> SolvableQuery::getClauseGroup() {
    return this->clauseGroups;
}
void SolvableQuery::setClauseGroup(
    std::vector<std::vector<QueryClause *>> *clauseGroups) {
    this->clauseGroups = *clauseGroups;
}
void SolvableQuery::replaceWithClause(
    std::vector<QueryClause *> newWithClause) {
    this->withCls = newWithClause;
}