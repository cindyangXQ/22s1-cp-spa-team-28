#pragma once

#include "../Reflexive.h"
#include "AffectsTable.h"
#include "StmtToStmtRelationshipsTable.h"

class AffectsTTable : public StmtToStmtRelationshipsTable, public Reflexive {
public:
    void initAffectsT(StorageView *storage);

    /*
     * Populate AffectsTTable.
     */
    void populateAffectsT();

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
    std::vector<Value> solveBothReflexive(EntityName synonym,
                                          StorageView *storage);

private:
    std::map<std::pair<int, int>, bool> matrix;
    std::unordered_set<int> assignments;
    int totalLines;
    /*
     * Helper method for computing Affects* from Affects.
     */
    std::map<std::pair<int, int>, bool> computeClosure();
    // TODO: Abstract these common methods into a common utils file, duplicated
    // form AffectsTable
    bool isAssignment(int stmt);
    bool areAssignments(int left, int right);
    bool isAssignmentEntity(EntityName entity);
};
