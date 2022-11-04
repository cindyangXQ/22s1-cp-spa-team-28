#pragma once

#include "../Clause/WithClause.h"
#include "../Parser/SolvableQuery.h"

class WithOptimizer {
public:
    static void optimize(SolvableQuery *solvableQ);
};
