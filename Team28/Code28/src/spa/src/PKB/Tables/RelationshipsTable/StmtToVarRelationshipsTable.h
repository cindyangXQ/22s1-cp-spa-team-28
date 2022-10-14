#pragma once

#include "../Solvable.h"
#include "RelationshipsTable.h"

class StmtToVarRelationshipsTable : public RelationshipsTable<int, std::string>,
                                    public virtual Solvable {
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

    virtual std::vector<Value> getMatchingValue(std::string value,
                                                EntityName entity);
    virtual std::map<Value, std::vector<Value>> getAllValues(EntityName entity);
};

class ModifiesSTable : public StmtToVarRelationshipsTable {};
class UsesSTable : public StmtToVarRelationshipsTable {};
class CallProcTable : public StmtToVarRelationshipsTable {};
