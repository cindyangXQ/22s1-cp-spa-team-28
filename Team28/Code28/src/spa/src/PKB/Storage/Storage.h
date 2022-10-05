#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include "../../commons/Constant.h"
#include "../../commons/Entity.h"
#include "../../commons/Procedure.h"
#include "../../commons/TableValue.h"
#include "../../commons/Variable.h"

#include "../Tables/AssignmentsTable/AssignmentsTable.h"
#include "../Tables/NamesTable/NamesTable.h"
#include "../Tables/RelationshipsTable/BranchTable.h"
#include "../Tables/RelationshipsTable/ProcToProcRelationshipsTable.h"
#include "../Tables/RelationshipsTable/ProcToVarRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToStmtRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToVarRelationshipsTable.h"
#include "../Tables/RelationshipsTable/UsesControlVarTable.h"
#include "../Tables/StatementsTable/StatementsTable.h"
#include "../Tables/Table.h"

/*
 * Encapsulates a Storage class which is responsible for storing information to
 * tables in PKB.
 */
class Storage {
public:
    /*
     * Explicit constructor for Storage.
     */
    Storage();

    /*
     * Retrieve a table by the templated class givenn
     */
    template <typename Subclass> Subclass *getTable() {
        Table *table = this->tables.at(typeid(Subclass));
        return dynamic_cast<Subclass *>(table);
    };

private:
    std::map<std::type_index, Table *> tables;
};
