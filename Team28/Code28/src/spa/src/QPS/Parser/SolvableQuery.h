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
    SuchThatClause suchThatCl;
    PatternClause patternCl;

    SolvableQuery(Declaration decl, SelectType selectType,
                  SuchThatClause suchThatCl, PatternClause patternCl);
};
