#pragma once

#include <iostream>
#include "../Reference/Reference.h"

/*
 * Class encapsulating the logic of the such that clause.
 */
class SuchThatClause
{
public:
    RelRef relationshipType;
    Reference refLeft;
    Reference refRight;

    SuchThatClause();
    SuchThatClause(RelRef relationshipType, Reference refLeft, Reference refRight);
};