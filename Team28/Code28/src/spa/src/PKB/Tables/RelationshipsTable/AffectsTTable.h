#pragma once

#include "../Reflexive.h"
#include "AffectsTable.h"
#include "StmtToStmtRelationshipsTable.h"

class AffectsTTable : public StmtToStmtRelationshipsTable {
public:
    void initAffectsT(StorageView *storage);

    /*
     * Populate AffectsTTable.
     */
    void populateAffectsT();

private:
    std::map<std::pair<int, int>, bool> matrix;
    int totalLines;
    /*
     * Helper method for computing Affects* from Affects.
     */
    std::map<std::pair<int, int>, bool> computeClosure();
};
