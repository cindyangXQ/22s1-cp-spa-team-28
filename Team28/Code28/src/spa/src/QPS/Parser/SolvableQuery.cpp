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

Declaration SolvableQuery::getDecl() { return this->decl; }
SelectClause SolvableQuery::getSelectClause() { return this->selectClause; }
std::vector<SuchThatClause> SolvableQuery::getSuchThatCls() {
    return this->suchThatCls;
}
std::vector<PatternClause> SolvableQuery::getPatternCls() {
    return this->patternCls;
}
std::vector<WithClause> SolvableQuery::getWithCls() { return this->withCls; }
