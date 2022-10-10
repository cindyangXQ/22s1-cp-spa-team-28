#include "SolvableQuery.h"

SolvableQuery::SolvableQuery(Declaration decl, SelectClause selectClause,
                             std::vector<SuchThatClause> suchThatCls,
                             std::vector<PatternClause> patternCls) {
    this->decl = decl;
    this->selectClause = selectClause;
    this->suchThatCls = suchThatCls;
    this->patternCls = patternCls;
}
