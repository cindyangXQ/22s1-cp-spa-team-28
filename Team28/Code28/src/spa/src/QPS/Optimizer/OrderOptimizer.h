#pragma once

#include "../Parser/SolvableQuery.h"

class OrderOptimizer {
public:
    static void optimize(SolvableQuery *solvableQ);

private:
    static double getTotalScore(std::vector<QueryClause *> group);
};
