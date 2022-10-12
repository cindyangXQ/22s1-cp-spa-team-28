#include "ControlFlowGraph.h"

#include <map>

ControlFlowGraph::ControlFlowGraph(NextTable *nextTable, StorageView *storage) {
    this->next = nextTable;
    this->follows = storage->getTable<FollowsTable>();
    this->branchIn = storage->getTable<BranchInTable>();
    this->branchOut = storage->getTable<BranchOutTable>();
    this->procedures = storage->getTable<ProceduresTable>();
};

void ControlFlowGraph::populateNext() {
    this->visited = std::map<int, bool>();

    for (int stmtNum : this->procedures->getAllStmtNum()) {
        if (!(follows->isLeftValueExist(stmtNum) ||
              branchIn->isLeftValueExist(stmtNum))) {
            continue;
        }
        DFS(stmtNum);
    }
};

void ControlFlowGraph::DFS(int i) {
    if (!(follows->isLeftValueExist(i) || branchIn->isLeftValueExist(i) ||
          branchOut->isLeftValueExist(i))) {
        return;
    }

    this->visited[i] = true;

    DFSHelper(i, this->branchIn);
    DFSHelper(i, this->branchOut);
    DFSHelper(i, this->follows);
};
