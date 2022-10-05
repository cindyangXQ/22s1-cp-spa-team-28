#pragma once

#include "../../commons/TableValue.h"

/*
 * An interface of tables which acts as our database.
 */
class Table {
public:
    virtual void store(TableValue *item) = 0;

    virtual ~Table(){};
};
