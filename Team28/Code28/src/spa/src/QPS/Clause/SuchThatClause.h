#pragma once

#include <iostream>
#include "../../commons/Relationship.h"
#include "../../commons/Reference.h"

/*
 * Class encapsulating the logic of the such that clause.
 */
class SuchThatClause
{
public:
    RelationshipReference relationship;
    Reference refLeft;
    Reference refRight;

    SuchThatClause();
    SuchThatClause(RelationshipReference relationship, Reference refLeft, Reference refRight);
};