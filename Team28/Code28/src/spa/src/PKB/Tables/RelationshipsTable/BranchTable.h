#pragma once

#include "RelationshipsTable.h"

class BranchTable : public RelationshipsTable<int, int> {
    // add helper methods for DFS?
};

typedef BranchTable BranchInTable;
typedef BranchTable BranchOutTable;
