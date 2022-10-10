#pragma once

#include "../Storage/StorageView.h"
#include "../Tables/Table.h"
#include "../Tables/RelationshipsTable/StmtToStmtRelationshipsTable.h"

/*
 * Encapsulates a CFG class which is responsible for extracting the
 * Next relationship from the given source program.
 */
class ControlFlowGraph {
public:
    static void populateNext(NextTable* nextTable, StorageView* storage);
};