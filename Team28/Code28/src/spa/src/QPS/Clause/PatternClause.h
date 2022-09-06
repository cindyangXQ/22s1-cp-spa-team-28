#pragma once

#include "../Reference/Synonym.h"
#include "../../commons/Reference.h"

typedef std::string Expression;
/*
 * Class encapsulating the logic of the pattern clause.
 */
class PatternClause
{
public:
    Synonym syn;
    Reference entRef;
    Expression expression;
	PatternClause();
    PatternClause(Synonym syn, Reference entRef, Expression expression);
};