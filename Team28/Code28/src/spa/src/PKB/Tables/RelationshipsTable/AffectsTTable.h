#pragma once

#include "AffectsBaseTable.h"
#include "AffectsTable.h"

class AffectsTTable : public AffectsBaseTable {
public:
    void initAffectsT(StorageView *storage);

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
     * Returns list of possible (Value, Value) that a reflexive pair of synonyms
     * can be.
     */
    std::vector<Value> solveBothReflexive(EntityName synonym,
                                          StorageView *storage);

private:
    AffectsTable *affects;
    std::map<std::pair<int, int>, bool> matrix;
    bool isComputed = false;

    /*
     * Helper method for computing Affects* from Affects.
     */
    std::map<std::pair<int, int>, bool> computeClosure();

    bool verifyDoubleWildcards();
    bool verifyLeftWildcard(int right);
    bool verifyRightWildcard(int left);
    void solveSingleWildcard(std::unordered_set<Value> *intermediateResult,
                             Position pos);
    void solveHelper(int stmt, std::unordered_set<Value> *intermediateResult,
                     Position stmtPos);
};
