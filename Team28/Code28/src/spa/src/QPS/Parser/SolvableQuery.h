#pragma once

#include "../Clause/Declaration.h"
#include "../Clause/PatternClause.h"
#include "../Clause/SelectClause.h"
#include "../Clause/SuchThatClause.h"
#include "../Clause/WithClause.h"
#include <iostream>

/*
 * Class encapsulating the logical representation of the query after parsing.
 */
class SolvableQuery {
public:
    Declaration decl;
    SelectClause selectClause;
    std::vector<SuchThatClause> suchThatCls;
    std::vector<PatternClause> patternCls;
    std::vector<WithClause> withCls;

    SolvableQuery(Declaration decl, SelectClause selectClause,
                  std::vector<SuchThatClause> suchThatCls,
                  std::vector<PatternClause> patternCls,
                  std::vector<WithClause> withCls);
};
