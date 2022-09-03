#pragma once

#include <iostream>
#include "../../commons/Reference.h"

/*
 * Class encapsulating the logic of the such that clause.
 */
class SuchThatClause
{
public:
    RelationshipReference relationshipType;
    Reference refLeft;
    Reference refRight;

    SuchThatClause();
    SuchThatClause(RelationshipReference relationshipType, Reference refLeft, Reference refRight);
};