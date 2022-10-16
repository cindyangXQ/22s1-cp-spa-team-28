#pragma once

#include "../../commons/Reference.h"
#include "../../commons/Relationship.h"
#include <iostream>

/*
 * Class encapsulating the logic of the such that clause.
 */
class SuchThatClause {
public:
    RelationshipReference relationship;
    Reference refLeft;
    Reference refRight;

    SuchThatClause() : relationship(RelationshipReference::EMPTY){};
    SuchThatClause(RelationshipReference relationship, Reference refLeft,
                   Reference refRight)
        : relationship(relationship), refLeft(refLeft), refRight(refRight){};
};
