#pragma once

#include "../../commons/Entity.h"
#include "../../commons/TableValue.h"
#include "../../commons/Value.h"

#define UNUSED(x) (void)(x)

/*
 * A superclass for tables which acts as our database.
 */
class Table {
public:
    virtual void store(TableValue *item) = 0;

    virtual std::vector<Value> getValue(std::string value,
                                        EntityName entity) = 0;
};
