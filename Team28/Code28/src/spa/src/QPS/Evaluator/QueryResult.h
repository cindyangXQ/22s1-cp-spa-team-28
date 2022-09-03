#pragma once

#include "../Clause/SelectClause.h"
#include "ClauseResult.h"

class QueryResult {
public:
    SelectType selectType;
    std::vector<ClauseResult> clauseResultList;
    QueryResult();
    explicit QueryResult(SelectType selectType, std::vector<ClauseResult> clauseResultList);
};