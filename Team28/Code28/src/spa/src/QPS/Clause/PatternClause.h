#pragma once

#include "../../commons/Synonym.h"
#include "../../commons/Reference.h"

enum class PatternType {
    ASSIGN,
    EMPTY
};
typedef std::string Expression;
/*
 * Class encapsulating the logic of the pattern clause.
 */
class PatternClause
{
public:
    Synonym syn = Synonym();
    PatternType patternType;
    Reference entRef;
    Expression expression;
	PatternClause();
    PatternClause(Synonym syn, Reference entRef, Expression expression);
};