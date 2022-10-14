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

    virtual std::vector<Value> getMatchingValue(std::string value,
                                                EntityName entity) = 0;

    /*
     * Maps the secondary value to the first value(s) in a relationship. For
     * non-RelationshipTables, maps the same value back to itself.
     */
    virtual std::map<Value, std::vector<Value>>
    getAllValues(EntityName entity) = 0;
};
