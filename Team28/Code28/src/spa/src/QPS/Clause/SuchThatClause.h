#pragma once

#include <iostream>
#include "../Reference/Reference.h"

class SuchThatClause
{
public:
    RelationshipReference relationshipType;
    Reference refLeft;
    Reference refRight;

    SuchThatClause();
    SuchThatClause(RelationshipReference relationshipType, Reference refLeft, Reference refRight);
};