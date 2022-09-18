#pragma once

#include <list>

#include "../PKB/QueryFacade.h"
#include "Evaluator/ClauseResult.h"
#include "Evaluator/PatternEvaluator.h"
#include "Evaluator/QueryEvaluator.h"
#include "Evaluator/QueryResult.h"
#include "Evaluator/SuchThatEvaluator.h"
#include "Parser/QueryParser.h"
#include "Parser/SolvableQuery.h"

/*
 * A wrapper class for QPS
 * UI system or main executable can call QPS::processQuery() to process a single
 * query
 */
class QPS {
public:
    QueryEvaluator evaluator;

    QPS(QueryFacade *Facade) : evaluator(QueryEvaluator(Facade)){};

    std::string processQuery(std::string input);
    void evaluate(std::string query, std::list<std::string> &results);
};
