#pragma once

#include "../Clause/Declaration.h"
#include "../Clause/PatternClause.h"
#include "../Clause/SelectClause.h"
#include "../Clause/SuchThatClause.h"
#include <iostream>

/*
 * Class encapsulating the logical representation of the query after parsing.
 */
class SolvableQuery {
public:
    Declaration decl;
    SelectType selectType = Synonym();
    std::vector<SuchThatClause> suchThatCls;
    std::vector<PatternClause> patternCls;

    SolvableQuery(Declaration decl, SelectType selectType,
                  std::vector<SuchThatClause> suchThatCls,
                  std::vector<PatternClause> patternCls);
};
