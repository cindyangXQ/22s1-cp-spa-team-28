#pragma once

#include "../Clause/WithClause.h"
#include "../Parser/SolvableQuery.h"

class WithOptimizer {
public:
    static void optimize(SolvableQuery *solvableQ);

private:
    static bool
    performReplacement(Reference synRef, Reference valRef,
                       std::vector<QueryClause *> *clauses,
                       std::vector<QueryClause *> *withClauses, int i,
                       std::unordered_set<std::string> selectedSynName);
};
