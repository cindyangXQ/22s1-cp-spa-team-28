#include "SolvableQuery.h"

SolvableQuery::SolvableQuery() {}
SolvableQuery::SolvableQuery(Declaration decl, SelectClause selectCl, SuchThatClause suchThatCl, PatternClause patternCl) {
    this->decl = decl;
    this->selectCl = selectCl;
    this->suchThatCl = suchThatCl;
    this->patternCl = patternCl;
}