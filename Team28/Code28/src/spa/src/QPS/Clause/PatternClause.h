#pragma once

#include "../Reference/Synonym.h"
#include "../Reference/Reference.h"

/*
 * Class encapsulating the logic of the pattern clause.
 */
class PatternClause
{
public:
    Synonym syn;
    Reference entRef;
    Reference stmtRef;
	PatternClause();
    PatternClause(Synonym syn, Reference entRef, Reference stmtRef);
};