#pragma once

#include "../../../commons/Value.h"
#include "../../Storage/StorageView.h"
#include "../Reflexive.h"
#include "StmtToStmtRelationshipsTable.h"

class NextTTable : public StmtToStmtRelationshipsTable, public Reflexive {
public:
    std::vector<Value> solveBothReflexive(EntityName stmtEntity,
                                          StorageView *storage);
};
