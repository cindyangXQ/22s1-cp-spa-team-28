#include "QueryOptimizer.h"

void QueryOptimizer::optimize(SolvableQuery *solvableQ) {
    WithOptimizer::optimize(solvableQ);
    GroupOptimizer::optimize(solvableQ);
    OrderOptimizer::optimize(solvableQ);
}