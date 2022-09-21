#pragma once

#include "RelationshipsTable.h"

class StmtToStmtRelationshipsTable : public RelationshipsTable<int, int> {
public:
    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    bool validate(Reference leftRef, Reference rightRef);

    /*
     * Returns list of possible values that the right synonym can be.
     */
    std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym,
                                  StatementsTable *statements);

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 StatementsTable *statements);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym,
                                                   EntityName rightSynonym,
                                                   StatementsTable *statements);
};

typedef StmtToStmtRelationshipsTable ParentTable;
typedef StmtToStmtRelationshipsTable ParentTTable;
typedef StmtToStmtRelationshipsTable FollowsTable;
typedef StmtToStmtRelationshipsTable FollowsTTable;
