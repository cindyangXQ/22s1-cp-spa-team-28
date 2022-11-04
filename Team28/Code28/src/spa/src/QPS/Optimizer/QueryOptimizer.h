#pragma once

#include "GroupOptimizer.h"
#include "OrderOptimizer.h"
#include "WithOptimizer.h"

class QueryOptimizer {
public:
    static void optimize(SolvableQuery *solvableQ);
};
