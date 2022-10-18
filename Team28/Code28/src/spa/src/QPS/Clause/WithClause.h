#pragma once

#include "../../commons/Reference.h"

/*
 * Class encapsulating the logic of the with clause.
 */
class WithClause {
public:
    WithClause(){};
    WithClause(Reference refLeft, Reference refRight)
        : refLeft(refLeft), refRight(refRight){};
    Reference getRefLeft();
    Reference getRefRight();

private:
    Reference refLeft;
    Reference refRight;
};
