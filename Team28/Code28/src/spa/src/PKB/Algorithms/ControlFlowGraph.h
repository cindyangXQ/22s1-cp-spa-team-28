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
    /*
     * Explicit constructor for Storage.
     */
    ControlFlowGraph(NextTable* nextTable, StorageView* storage);

    /*
     * Populates NextTable based on relationships currently found in Storage.
     * Traversal of CFG is done through DFS.
     */
    void populateNext();

private:
    NextTable* next;
    FollowsTable* follows;
    BranchInTable* branchIn;
    BranchOutTable* branchOut;

    std::map<int, bool> visited;

    /*
     * Depth First Search of CFG based on given integer.
     */
    void DFS(int i);

    /*
     * Helper method for DFS to find neighbouring statements in given table.
     */
    template <typename Subclass> void DFSHelper(int i, Subclass* table) {
        for (int j : table->retrieveLeft(i)) {
            Relationship<int, int> rs =
                Relationship(RelationshipReference::NEXT, i, j);
            this->next->store(&rs);
            if (!this->visited[j]) {
                DFS(j);
            }
        }
    };
};