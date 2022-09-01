#pragma once

#include "../Reference/Synonym.h"
#include "../Reference/Reference.h"

class PatternClause
{
public:
    Synonym syn;
    Reference entRef;
    Reference stmtRef;
	PatternClause();
    PatternClause(Synonym syn, Reference entRef, Reference stmtRef);
};