#pragma once

#include <iostream>
#include "../../commons/Reference.h"
#include "../../commons/Relationship.h"

/*
 * Class encapsulating the logic of the such that clause.
 */
class SuchThatClause
{
public:
    Relationship relationship;
    Reference refLeft;
    Reference refRight;

    SuchThatClause();
    SuchThatClause(Relationship relationship, Reference refLeft, Reference refRight);
};