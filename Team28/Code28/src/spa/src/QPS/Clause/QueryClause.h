#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Evaluator/ClauseResult.h"
#include <regex>

class QueryClause {
public:
    virtual ~QueryClause(){};
    virtual void parse(std::smatch matches, std::vector<Synonym> syns) = 0;
    virtual bool validate() = 0;
    virtual ClauseResult evaluate(QueryFacade *queryFacade) = 0;
    virtual std::unordered_set<std::string> getSynonymsUsed() = 0;
    virtual void populateOptimizeScore(QueryFacade *queryFacade) = 0;
    virtual double getOptimizeScore() = 0;
    virtual bool replace(Reference synRef, Reference valRef) = 0;
};