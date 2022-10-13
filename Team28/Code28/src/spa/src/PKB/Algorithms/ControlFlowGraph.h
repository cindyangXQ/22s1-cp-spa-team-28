#pragma once

#include "../Storage/StorageView.h"
#include "../Tables/RelationshipsTable/StmtToStmtRelationshipsTable.h"
#include "../Tables/Table.h"

/*
 * Encapsulates a CFG class which is responsible for extracting the
 * Next relationship from the given source program.
 */
class ControlFlowGraph {
public:
    /*
     * Explicit constructor for Storage.
     */
    ControlFlowGraph(NextTable *nextTable, NextTTable *nextTTable,
                     StorageView *storage);

    /*
     * Populates NextTable based on relationships currently found in Storage.
     * Traversal of CFG is done through DFS.
     */
    void populateNext();

    /*
     * Populates NextTTable by iterating through current Next relationships.
     */
    void populateNextT();

private:
    NextTable *next;
    NextTTable *nextT;
    FollowsTable *follows;
    BranchInTable *branchIn;
    BranchOutTable *branchOut;
    ProceduresTable *procedures;

    std::map<std::pair<int, int>, bool> visited;

    /*
     * Depth First Search of CFG based on given integer.
     */
    void DFS(int i);

    /*
     * Helper method for DFS to find neighbouring statements in given table.
     */
    template <typename Subclass> void DFSHelper(int i, Subclass *table) {
        for (int j : table->retrieveLeft(i)) {
            Relationship<int, int> nextRs =
                Relationship(RelationshipReference::NEXT, i, j);
            Relationship<int, int> nextTRs =
                Relationship(RelationshipReference::NEXT_T, i, j);
            this->next->store(&nextRs);
            this->nextT->store(&nextTRs);
            std::pair<int, int> curr = std::make_pair(i, j);
            bool isVisited = this->visited.find(curr) != this->visited.end();
            if (!isVisited) {
                this->visited[curr] = true;
                DFS(j);
            }
        }
    };
};
