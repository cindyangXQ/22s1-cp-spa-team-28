#include "QueryResult.h"

SelectClause QueryResult::getSelectClause() { return this->selectClause; }
std::vector<std::vector<ClauseResult>> QueryResult::getClauseResultList() {
    return this->clauseResultList;
}
