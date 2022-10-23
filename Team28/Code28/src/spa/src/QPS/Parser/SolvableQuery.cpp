#include "SolvableQuery.h"

Declaration SolvableQuery::getDecl() { return this->decl; }
SelectClause SolvableQuery::getSelectClause() { return this->selectClause; }
std::unordered_map<std::type_index, std::vector<QueryClause *>>
SolvableQuery::getQueryClause() {
    return this->clauses;
}