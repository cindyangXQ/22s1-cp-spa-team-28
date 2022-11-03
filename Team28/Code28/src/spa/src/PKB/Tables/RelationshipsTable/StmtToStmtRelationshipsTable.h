#pragma once

#include "../Reflexive.h"
#include "../Solvable.h"
#include "RelationshipsTable.h"

class StmtToStmtRelationshipsTable : public RelationshipsTable<int, int>,
                                     public Solvable {
public:
    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    virtual bool validate(Reference leftRef, Reference rightRef);

    /*
     * Returns list of possible values that the right synonym can be.
     */
    virtual std::vector<Value> solveRight(Reference leftRef,
                                          EntityName rightSynonym,
                                          StorageView *storage);

    /*
     * Returns list of possible values that the left synonym can be.
     */
    virtual std::vector<Value>
    solveLeft(Reference rightRef, EntityName leftSynonym, StorageView *storage);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    virtual std::vector<std::pair<Value, Value>>
    solveBoth(EntityName leftSynonym, EntityName rightSynonym,
              StorageView *storage);
};

class ParentTable : public StmtToStmtRelationshipsTable {};
class ParentTTable : public StmtToStmtRelationshipsTable {};
class FollowsTable : public StmtToStmtRelationshipsTable {};
class FollowsTTable : public StmtToStmtRelationshipsTable {};
class BranchInTable : public StmtToStmtRelationshipsTable {};
class BranchOutTable : public StmtToStmtRelationshipsTable {};
class NextTable : public StmtToStmtRelationshipsTable {};
