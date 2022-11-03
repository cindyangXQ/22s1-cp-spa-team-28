#include "ControlFlowGraph.h"

#include <map>

ControlFlowGraph::ControlFlowGraph(NextTable *nextTable, StorageView *storage) {
    this->next = nextTable;
    this->statements = storage->getTable<StatementsTable>();
    this->follows = storage->getTable<FollowsTable>();
    this->branchIn = storage->getTable<BranchInTable>();
    this->branchOut = storage->getTable<BranchOutTable>();
    this->procedures = storage->getTable<ProceduresTable>();
};

void ControlFlowGraph::populateNext() {
    this->visited = std::map<std::pair<int, int>, bool>();

    for (int stmtNum : this->procedures->getAllStmtNum()) {
        if (!(follows->isLeftValueExist(stmtNum) ||
              branchIn->isLeftValueExist(stmtNum))) {
            continue;
        }
        DFS(stmtNum);
    }
};

void ControlFlowGraph::DFS(int i) {
    if (!(this->follows->isLeftValueExist(i) ||
          this->branchIn->isLeftValueExist(i) ||
          this->branchOut->isLeftValueExist(i))) {
        return;
    }

    DFSHelper(i, this->branchIn);
    DFSHelper(i, this->branchOut);
    if (!this->statements->isIfStatement(i)) {
        DFSHelper(i, this->follows);
    }
};
