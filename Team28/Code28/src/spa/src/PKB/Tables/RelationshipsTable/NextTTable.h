#pragma once

#include "../../../commons/Value.h"
#include "../../Storage/StorageView.h"
#include "../Cache.h"
#include "../Reflexive.h"
#include "StmtToStmtRelationshipsTable.h"

class NextTTable : public StmtToStmtRelationshipsTable,
                   public Reflexive,
                   public Cache {
public:
    void initNextT(StorageView *storage);

    void resetCache();

    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    bool validate(Reference leftRef, Reference rightRef);

    /*
     * Returns list of possible values that the right synonym can be.
     */
    std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym,
                                  StorageView *storage);

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 StorageView *storage);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym,
                                                   EntityName rightSynonym,
                                                   StorageView *storage);

    /*
     * Returns list of possible values that has a reflexive relationship.
     */
    std::vector<Value> solveBothReflexive(EntityName stmtEntity,
                                          StorageView *storage);

    int getTableSize();

private:
    NextTable *next;

    int totalLines;
    std::map<std::pair<int, int>, bool> matrix;
    std::map<int, bool> isDFSComputed;

    void initMatrix();
    void DFSUtil(int s, int v);
    bool checkNextT(int left, int right);
    bool verifyDoubleWildcards();
    bool verifyLeftWildcard(int right);
    bool verifyRightWildcard(int left);
    std::vector<Value> solveRightHelper(std::vector<int> *possibleRights,
                                        Reference leftRef);
    std::vector<Value> solveLeftHelper(std::vector<int> *possibleLefts,
                                       Reference rightRef);
};
