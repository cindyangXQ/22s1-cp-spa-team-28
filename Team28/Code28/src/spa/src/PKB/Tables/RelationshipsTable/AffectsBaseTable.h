#pragma once

#include "../Cache.h"
#include "../Reflexive.h"
#include "StmtToStmtRelationshipsTable.h"

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

    bool isAssignment(int stmt);
    bool areAssignments(int left, int right);
    bool isAssignmentEntity(EntityName entity);
    int chooseStmt(int left, int right, Position pos);

    template <class object>
    bool validateHelper(Reference *leftRef, Reference *rightRef,
                        object *affectsTable,
                        bool (object::*verifyDoubleWildcards)(),
                        bool (object::*verifySingleWildcard)(int, Position),
                        bool (object::*checkRs)(int, int)) {
        if (leftRef->isWildcard() && rightRef->isWildcard()) {
            return (affectsTable->*(verifyDoubleWildcards))();
        }
        if (leftRef->isWildcard()) {
            int right = convertToType<int>(rightRef->getValueString());
            return (affectsTable->*(verifySingleWildcard))(right,
                                                           Position::RIGHT);
        }
        if (rightRef->isWildcard()) {
            int left = convertToType<int>(leftRef->getValueString());
            return (affectsTable->*(verifySingleWildcard))(left,
                                                           Position::LEFT);
        }
        int left = convertToType<int>(leftRef->getValueString());
        int right = convertToType<int>(rightRef->getValueString());
        if (!areAssignments(left, right)) {
            return false;
        }
        return (affectsTable->*(checkRs))(left, right);
    };
};
