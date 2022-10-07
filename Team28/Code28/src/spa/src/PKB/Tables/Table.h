#pragma once

#include "../../commons/TableValue.h"

/*
 * A superclass for tables which acts as our database.
 */
class Table {
public:
    virtual void store(TableValue *item) = 0;
};
