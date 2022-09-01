#include "SolvableQuery.h"
#include "Evaluator/SelectEvaluator.h"
#include "Evaluator/SuchThatEvaluator.h"
#include "Evaluator/PatternEvaluator.h"

SolvableQuery::SolvableQuery() {}
SolvableQuery::SolvableQuery(Declaration decl, SelectClause selectCl, SuchThatClause suchThatCl, PatternClause patternCl) {
    this->decl = decl;
    this->selectCl = selectCl;
    this->suchThatCl = suchThatCl;
    this->patternCl = patternCl;
}

void SolvableQuery::evaluate() {
    SuchThatEvaluator::evaluate(this->suchThatCl);
    PatternEvaluator::evaluate(this->patternCl);
    SelectEvaluator::evaluate(this->selectCl);
}