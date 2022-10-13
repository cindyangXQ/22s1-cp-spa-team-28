#include "ControlFlowGraph.h"

#include <map>

ControlFlowGraph::ControlFlowGraph(NextTable *nextTable, NextTTable *nextTTable,
                                   StorageView *storage) {
    this->next = nextTable;
    this->nextT = nextTTable;
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

void ControlFlowGraph::populateNextT() {
    std::cout << "ORIGINAL" << std::endl;
    for (const auto &elem : this->visited) {
        std::cout << "(" << elem.first.first << ", " << elem.first.second
                  << ") " << elem.second << "\n";
    }
    for (const auto &elem1 : this->visited) {
        std::cout << "ENTERING " << std::endl;
        for (const auto &elem2 : this->visited) {
            if (elem1.first.second == elem2.first.first) {
                Relationship<int, int> nextTRs =
                    Relationship(RelationshipReference::NEXT_T,
                                 elem1.first.first, elem2.first.second);
                std::cout << "(" << elem1.first.first << ", "
                          << elem2.first.second << ") " << elem1.second << "\n";
                this->nextT->store(&nextTRs);
            }
        }
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
    DFSHelper(i, this->follows);
};
