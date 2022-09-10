#pragma once

#include <iostream>
#include "../../commons/Reference.h"
#include "../../commons/Relationship/Relationship.h"

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