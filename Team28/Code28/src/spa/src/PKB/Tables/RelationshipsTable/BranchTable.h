#pragma once

#include "RelationshipsTable.h"

class BranchTable : public RelationshipsTable<int, int> {
    // add helper methods for DFS?
};

class BranchInTable : public BranchTable {};
class BranchOutTable : public BranchTable {};
