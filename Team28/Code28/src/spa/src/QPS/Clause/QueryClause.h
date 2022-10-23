#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Evaluator/ClauseResult.h"
#include <regex>

class QueryClause {
public:
    virtual void parse(std::smatch matches, std::vector<Synonym> syns) = 0;
    virtual bool validate() = 0;
    virtual ClauseResult evaluate(QueryFacade *queryFacade) = 0;
};