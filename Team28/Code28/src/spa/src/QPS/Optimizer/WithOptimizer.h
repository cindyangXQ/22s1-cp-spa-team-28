#pragma once

#include "../Parser/SolvableQuery.h"
#include "../Clause/WithClause.h"

class WithOptimizer {
public:
    static void optimize(SolvableQuery *solvableQ);
};
