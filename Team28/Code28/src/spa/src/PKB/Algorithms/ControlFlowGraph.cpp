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
    this->visited = std::map<std::pair<int, int>, bool>();

    for (int stmtNum : this->procedures->getAllStmtNum()) {
        if (!(follows->isLeftValueExist(stmtNum) ||
              branchIn->isLeftValueExist(stmtNum))) {
            continue;
        }
        DFS(stmtNum);
    }

    for (auto it = this->visited.cbegin(); it != this->visited.cend(); ++it) {
        std::cout << "(" << it->first.first << ", " << it->first.second << ")"
                  << " " << it->second << "\n";
    }
    std::cout << "FINISHED" << std::endl;
};

void ControlFlowGraph::DFS(int i) {
    std::pair<int, int> curr;

    std::cout << "DFS called with i: " << i << std::endl;

    if (!(this->follows->isLeftValueExist(i) ||
          this->branchIn->isLeftValueExist(i) ||
          this->branchOut->isLeftValueExist(i))) {
        return;
    }

    if (this->branchIn->isLeftValueExist(i)) {
        for (const auto &j : this->branchIn->retrieveLeft(i)) {
            curr = std::make_pair(i, j);
            this->visited[curr] = true;
        }
    }

    if (this->branchOut->isLeftValueExist(i)) {
        for (const auto &j : this->branchIn->retrieveLeft(i)) {
            curr = std::make_pair(i, j);
            this->visited[curr] = true;
        }
    }

    if (this->follows->isLeftValueExist(i)) {
        for (const auto &j : this->branchIn->retrieveLeft(i)) {
            curr = std::make_pair(i, j);
            this->visited[curr] = true;
        }
    }
    DFSHelper(i, this->branchIn);
    DFSHelper(i, this->branchOut);
    DFSHelper(i, this->follows);
};
