#pragma once

#include "../../commons/TableValue.h"

/*
 * A superclass for tables which acts as our database.
 */
class Table {
public:
    virtual void store(TableValue *item) = 0;

    virtual std::vector<std::string> getAllAsString() = 0;
};
