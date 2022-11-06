#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "GroupOptimizer.h"
#include "OrderOptimizer.h"
#include "WithOptimizer.h"

class QueryOptimizer {
private:
    QueryFacade *queryFacade;

public:
    explicit QueryOptimizer(QueryFacade *queryFacade)
        : queryFacade(queryFacade){};
    void optimize(SolvableQuery *solvableQ);
};
