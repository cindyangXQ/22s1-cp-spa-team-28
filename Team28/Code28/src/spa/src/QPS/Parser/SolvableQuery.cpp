#include "SolvableQuery.h"

SolvableQuery::SolvableQuery(Declaration decl, SelectClause selectClause,
                             std::vector<SuchThatClause> suchThatCls,
                             std::vector<PatternClause> patternCls,
                             std::vector<WithClause> withCls) {
    this->decl = decl;
    this->selectClause = selectClause;
    this->suchThatCls = suchThatCls;
    this->patternCls = patternCls;
    this->withCls = withCls;
}
