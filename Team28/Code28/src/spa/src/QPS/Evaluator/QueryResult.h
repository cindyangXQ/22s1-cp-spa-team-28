#pragma once

#include "../Clause/SelectClause.h"
#include "ClauseResult.h"

class QueryResult {
public:
    SelectClause selectClause;
    std::vector<ClauseResult> clauseResultList;
    QueryResult();
    explicit QueryResult(SelectClause selectClause,
                         std::vector<ClauseResult> clauseResultList)
        : selectClause(selectClause), clauseResultList(clauseResultList){};
};
