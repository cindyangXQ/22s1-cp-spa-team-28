#pragma once

#include "../../commons/Reference.h"
#include "Table.h"
#include "../Storage/Storage.h"

class Solvable {
public:
    virtual bool validate(Reference leftRef, Reference rightRef) = 0;

    virtual std::vector<Value>
    solveRight(Reference leftRef, EntityName rightSynonym, Storage *storage);
};
