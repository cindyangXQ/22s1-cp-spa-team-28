#pragma once

#include "../Clause/SelectClause.h"
#include "ClauseResult.h"

class QueryResult {
public:
    QueryResult(){};
    QueryResult(SelectClause selectClause,
                std::vector<std::vector<ClauseResult>> clauseResultList)
        : selectClause(selectClause), clauseResultList(clauseResultList){};
    SelectClause getSelectClause();
    std::vector<std::vector<ClauseResult>> getClauseResultList();

private:
    SelectClause selectClause;
    std::vector<std::vector<ClauseResult>> clauseResultList;
};
