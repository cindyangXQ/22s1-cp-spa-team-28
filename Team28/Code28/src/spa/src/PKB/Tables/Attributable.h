#pragma once

#include "../../commons/Entity.h"
#include "../../commons/Value.h"
#include "../Storage/StorageView.h"

#define UNUSED(x) (void)(x)

/*
 * Abstract class to encapsulate tables that contain entities/abstractions that
 * are attributes.
 */
class Attributable {
public:
    /*
     * Gets all values of the given attribute.
     */
    virtual std::vector<Value> getMatchingValue(std::string value,
                                                EntityName entity,
                                                StorageView *storage) = 0;

    /*
     * Maps the secondary value to the first value(s) in a relationship. For
     * non-RelationshipTables, maps the same value back to itself.
     */
    virtual std::map<Value, std::vector<Value>>
    getAllValues(EntityName entity, StorageView *storage) = 0;
};
