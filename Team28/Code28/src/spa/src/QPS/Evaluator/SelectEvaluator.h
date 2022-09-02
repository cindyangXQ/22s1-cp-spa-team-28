#pragma once

#include "../Clause/SelectClause.h"

/*
 * Evaluator class for the select clause.
 */
class SelectEvaluator {
public:
    static void evaluate(SelectClause selectCl);
};