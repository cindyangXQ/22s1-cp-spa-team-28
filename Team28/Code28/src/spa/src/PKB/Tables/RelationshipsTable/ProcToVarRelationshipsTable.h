#pragma once

#include "../Solvable.h"
#include "RelationshipsTable.h"

class ProcToVarRelationshipsTable
    : public RelationshipsTable<std::string, std::string>,
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
                                  Storage *storage);

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 ProceduresTable *procedures);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym,
                                                   EntityName rightSynonym,
                                                   ProceduresTable *procedures,
                                                   VariablesTable *variables);
};

class ModifiesPTable : public ProcToVarRelationshipsTable {};
class UsesPTable : public ProcToVarRelationshipsTable {};
