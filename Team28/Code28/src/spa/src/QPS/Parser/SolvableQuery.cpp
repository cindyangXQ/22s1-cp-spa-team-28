#include "SolvableQuery.h"

SolvableQuery::SolvableQuery(Declaration decl, SelectType selectType,
                             std::vector<SuchThatClause> suchThatCls,
                             std::vector<PatternClause> patternCls) {
    this->decl = decl;
    this->selectType = selectType;
    this->suchThatCls = suchThatCls;
    this->patternCls = patternCls;
}
