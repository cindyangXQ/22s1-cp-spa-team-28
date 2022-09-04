#pragma once

#include <iostream>
#include "../Clause/Declaration.h"
#include "../Clause/SelectClause.h"
#include "../Clause/SuchThatClause.h"
#include "../Clause/PatternClause.h"

/*
 * Class encapsulating the logical representation of the query after parsing.
 */
class SolvableQuery
{
public:
	Declaration decl;
	SelectType selectType;
	SuchThatClause suchThatCl;
	PatternClause patternCl;

    SolvableQuery();
	SolvableQuery(Declaration decl, SelectType selectType, SuchThatClause suchThatCl, PatternClause patternCl);
};