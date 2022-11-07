#pragma once

#include "../Cache.h"
#include "../Reflexive.h"
#include "StmtToStmtRelationshipsTable.h"
#include <functional>

enum class Position { LEFT, RIGHT };

class AffectsBaseTable : public StmtToStmtRelationshipsTable,
                         public Reflexive,
                         public Cache {
public:
    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    virtual bool validate(Reference leftRef, Reference rightRef) = 0;

    /*
     * Returns list of possible values that the right synonym can be.
     */
    virtual std::vector<Value> solveRight(Reference leftRef,
                                          EntityName rightSynonym,
                                          StorageView *storage) = 0;

    /*
     * Returns list of possible values that the left synonym can be.
     */
    virtual std::vector<Value> solveLeft(Reference rightRef,
                                         EntityName leftSynonym,
                                         StorageView *storage) = 0;

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    virtual std::vector<std::pair<Value, Value>>
    solveBoth(EntityName leftSynonym, EntityName rightSynonym,
              StorageView *storage) = 0;

    /*
     * Returns list of possible (Value, Value) that a reflexive pair of synonyms
     * can be.
     */
    virtual std::vector<Value> solveBothReflexive(EntityName synonym,
                                                  StorageView *storage) = 0;

    int getTableSize();

protected:
    std::unordered_set<int> assignments;

    bool validateHelper(
        Reference leftRef, Reference rightRef,
        const std::function<bool()> &verifyDoubleWildcards,
        const std::function<bool(int, Position)> &verifySingleWildcard,
        const std::function<bool(int, int)> &checkRs);

    bool isAssignment(int stmt);
    bool areAssignments(int left, int right);
    bool isAssignmentEntity(EntityName entity);
    int chooseStmt(int left, int right, Position pos);
};
