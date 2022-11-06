#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Parser/SolvableQuery.h"

class OrderOptimizer {
public:
    static void optimize(SolvableQuery *solvableQ, QueryFacade *queryFacade);

private:
    static double getTotalScore(std::vector<QueryClause *> group);
};
