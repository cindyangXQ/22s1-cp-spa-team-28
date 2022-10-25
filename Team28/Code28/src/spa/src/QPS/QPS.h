#pragma once

#include <list>

#include "../PKB/Facades/QueryFacade.h"
#include "Evaluator/QueryEvaluator.h"
#include "Parser/QueryParser.h"
#include "Parser/SolvableQuery.h"

/*
 * A wrapper class for QPS
 * UI system or main executable can call QPS::processQuery() to process a single
 * query
 */
class QPS {
public:
    QPS(QueryFacade *facade) : evaluator(QueryEvaluator(facade)){};

    void evaluate(std::string query, std::list<std::string> &results);

    QueryEvaluator getEvaluator();

private:
    QueryEvaluator evaluator;
};
