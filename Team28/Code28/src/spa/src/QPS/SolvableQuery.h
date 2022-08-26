#include <iostream>
#include "Declaration.h"
#include "SelectClause.h"
#include "SuchThatClause.h"
#include "PatternClause.h"
class SolvableQuery
{
public:
	Declaration d;
	SelectClause select;
	SuchThatClause such_that;
	PatternClause pattern;

	SolvableQuery();
};