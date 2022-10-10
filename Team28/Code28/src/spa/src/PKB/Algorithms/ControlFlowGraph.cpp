#include "ControlFlowGraph.h"

#include <map>

ControlFlowGraph::ControlFlowGraph(NextTable* nextTable, StorageView* storage) {
    this->next = nextTable;
    this->follows = storage->getTable<FollowsTable>();
    this->branchIn = storage->getTable<BranchInTable>();
    this->branchOut = storage->getTable<BranchOutTable>();
};

void ControlFlowGraph::populateNext() {
    // Assumption: Source program always has line num 1 and should always start from 1.
    if (!(follows->isValueExist(1) || branchIn->isValueExist(1))) {
        return;
    }

    this->visited = std::map<int, bool>();

    DFS(1);
};

void ControlFlowGraph::DFS(int i) {
    if (!(follows->isValueExist(i) || branchIn->isValueExist(i) || branchOut->isValueExist(i))) {
        return;
    }

    this->visited[i] = true;

    DFSHelper(i, this->branchIn);
    DFSHelper(i, this->branchOut);
    DFSHelper(i, this->follows);
};

