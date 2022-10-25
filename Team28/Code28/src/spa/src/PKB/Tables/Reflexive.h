#pragma once

#include "../../commons/Value.h"
#include "../Storage/StorageView.h"
#include "Table.h"

class Reflexive {
public:
    virtual std::vector<Value> solveBothReflexive(EntityName stmtEntity,
                                                  StorageView *storage) = 0;
};
