#pragma once

#include "../../commons/Reference.h"
#include "../Storage/StorageView.h"
#include "Table.h"

/*
 * Abstract class encapsulating methods used for resolving queries.
 */
class Solvable {
public:
    virtual bool validate(Reference leftRef, Reference rightRef) = 0;

    virtual std::vector<Value> solveRight(Reference leftRef,
                                          EntityName rightSynonym,
                                          StorageView *storage) = 0;

    virtual std::vector<Value> solveLeft(Reference rightRef,
                                         EntityName leftSynonym,
                                         StorageView *storage) = 0;

    virtual std::vector<std::pair<Value, Value>>
    solveBoth(EntityName leftSynonym, EntityName rightSynonym,
              StorageView *storage) = 0;
};
