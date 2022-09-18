#include "SolvableQuery.h"

SolvableQuery::SolvableQuery(Declaration decl, SelectType selectType,
                             SuchThatClause suchThatCl,
                             PatternClause patternCl) {
    this->decl = decl;
    this->selectType = selectType;
    this->suchThatCl = suchThatCl;
    this->patternCl = patternCl;
}
