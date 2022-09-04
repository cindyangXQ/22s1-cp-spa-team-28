#pragma once

#include "../PKB/QueryFacade.h"
#include "Parser/QueryParser.h"
#include "Parser/SolvableQuery.h"
#include "Evaluator/QueryEvaluator.h"
#include "Evaluator/SuchThatEvaluator.h"
#include "Evaluator/PatternEvaluator.h"
#include "Evaluator/QueryResult.h"
#include "Evaluator/ClauseResult.h"

/*
* A wrapper class for QPS
* UI system or main executable can call QPS::processQuery() to process a single query
*/
class QPS {
public:
	QueryEvaluator evaluator;

	QPS(QueryFacade* Facade) : evaluator(QueryEvaluator(Facade)) {};

	std::string processQuery(std::string input);
};