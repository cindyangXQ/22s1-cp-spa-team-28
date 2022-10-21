#include "ControlFlowGraph.h"

#include <map>

ControlFlowGraph::ControlFlowGraph(NextTable *nextTable, NextTTable *nextTTable,
                                   StorageView *storage) {
    this->next = nextTable;
    this->nextT = nextTTable;
    this->statements = storage->getTable<StatementsTable>();
    this->follows = storage->getTable<FollowsTable>();
    this->branchIn = storage->getTable<BranchInTable>();
    this->branchOut = storage->getTable<BranchOutTable>();
    this->procedures = storage->getTable<ProceduresTable>();
    // TODO: fix LoD
    this->totalLines = storage->getTable<StatementsTable>()->getTableSize();
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
    std::map<std::pair<int, int>, bool> matrix = computeClosure(this->visited);

    for (const auto &elem : matrix) {
        if (elem.second) {
            Relationship<int, int> nextTRs =
                Relationship(RelationshipReference::NEXT_T, elem.first.first,
                             elem.first.second);

            this->nextT->store(&nextTRs);
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
    if (!this->statements->isIfStatement(i)) {
        DFSHelper(i, this->follows);
    }
};

std::map<std::pair<int, int>, bool>
ControlFlowGraph::computeClosure(std::map<std::pair<int, int>, bool> map) {
    std::map<std::pair<int, int>, bool> matrix;

    for (int i = 1; i <= this->totalLines; i++) {
        for (int j = 1; j <= this->totalLines; j++) {
            std::pair<int, int> curr = std::make_pair(i, j);
            matrix[curr] = false;
        }
    }

    for (const auto &elem : map) {
        matrix[elem.first] = elem.second;
    }

    for (int k = 1; k <= this->totalLines; k++) {
        for (int i = 1; i <= this->totalLines; i++) {
            for (int j = 1; j <= this->totalLines; j++) {
                std::pair<int, int> curr = std::make_pair(i, j);
                std::pair<int, int> left = std::make_pair(i, k);
                std::pair<int, int> right = std::make_pair(k, j);
                matrix[curr] = matrix[curr] || (matrix[left] && matrix[right]);
            }
        }
    }

    return matrix;
}
