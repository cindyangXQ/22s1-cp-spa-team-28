#include <iostream>
#include "../Reference/Reference.h"

class SuchThatClause
{
public:
    RelRef relationshipType;
    Reference refLeft;
    Reference refRight;

    SuchThatClause();
    SuchThatClause(RelRef relationshipType, Reference refLeft, Reference refRight);
};