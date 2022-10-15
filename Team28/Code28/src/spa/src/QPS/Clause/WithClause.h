#pragma once

#include "../../commons/Reference.h"

/*
 * Class encapsulating the logic of the with clause.
 */
class WithClause {
public:
    Reference refLeft;
    Reference refRight;

    WithClause();
    WithClause(Reference refLeft, Reference refRight);
};
