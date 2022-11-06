#pragma once

#include <list>

#include "../PKB/Facades/QueryFacade.h"
#include "Evaluator/QueryEvaluator.h"
#include "Optimizer/QueryOptimizer.h"
#include "Parser/QueryParser.h"

/*
 * A wrapper class for QPS
 * UI system or main executable can call QPS::processQuery() to process a single
 * query
 */
class QPS {
public:
    QPS(QueryFacade *facade)
        : evaluator(QueryEvaluator(facade)),
          optimizer(QueryOptimizer(facade)){};

    void evaluate(std::string query, std::list<std::string> &results);

    QueryEvaluator getEvaluator();

private:
    QueryEvaluator evaluator;
    QueryOptimizer optimizer;
};
