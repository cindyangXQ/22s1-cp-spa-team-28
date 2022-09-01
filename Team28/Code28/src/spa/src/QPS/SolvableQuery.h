#pragma once

#include <iostream>
#include "Clause/Declaration.h"
#include "Clause/SelectClause.h"
#include "Clause/SuchThatClause.h"
#include "Clause/PatternClause.h"
class SolvableQuery
{
public:
	Declaration decl;
	SelectClause selectCl;
	SuchThatClause suchThatCl;
	PatternClause patternCl;

    SolvableQuery();
	SolvableQuery(Declaration decl, SelectClause selectCl, SuchThatClause suchThatCl, PatternClause patternCl);
    void evaluate();
};