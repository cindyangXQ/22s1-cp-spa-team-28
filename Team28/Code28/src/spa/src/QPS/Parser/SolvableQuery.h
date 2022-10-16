#pragma once

#include "../Clause/Declaration.h"
#include "../Clause/PatternClause.h"
#include "../Clause/SelectClause.h"
#include "../Clause/SuchThatClause.h"
#include "../Clause/WithClause.h"
#include <iostream>

// TOFIX: public fields to private

/*
 * Class encapsulating the logical representation of the query after parsing.
 */
class SolvableQuery {
public:
    SolvableQuery(Declaration decl, SelectClause selectClause,
                  std::vector<SuchThatClause> suchThatCls,
                  std::vector<PatternClause> patternCls,
                  std::vector<WithClause> withCls);

    Declaration getDecl();
    SelectClause getSelectClause();
    std::vector<SuchThatClause> getSuchThatCls();
    std::vector<PatternClause> getPatternCls();
    std::vector<WithClause> getWithCls();

private:
    Declaration decl;
    SelectClause selectClause;
    std::vector<SuchThatClause> suchThatCls;
    std::vector<PatternClause> patternCls;
    std::vector<WithClause> withCls;
};
