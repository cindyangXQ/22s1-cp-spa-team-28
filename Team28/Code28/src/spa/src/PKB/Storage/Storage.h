#pragma once

#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "../../commons/Constant.h"
#include "../../commons/Entity.h"
#include "../../commons/Procedure.h"
#include "../../commons/TableValue.h"
#include "../../commons/Variable.h"

#include "../Tables/AssignmentsTable/AssignmentsTable.h"
#include "../Tables/NamesTable/NamesTable.h"
#include "../Tables/RelationshipsTable/ProcToVarRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToStmtRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToVarRelationshipsTable.h"
#include "../Tables/RelationshipsTable/ProcToProcRelationshipsTable.h" 
#include "../Tables/RelationshipsTable/BranchTable.h" 
#include "../Tables/StatementsTable/StatementsTable.h"
#include "../Tables/Table.h"

/*
 * Enumerates the different kinds of tables to instantiate.
 */
enum class TableName {
    STATEMENTS,
    ASSIGNMENTS,
    PROCEDURES,
    VARIABLES,
    CONSTANTS,
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    MODIFIES_S,
    MODIFIES_P,
    USES_S,
    USES_P,
    CALLS,
    CALLS_T,
    BRANCH_IN,
    BRANCH_OUT,
    NEXT,
    NEXT_T
};

/*
 * Encapsulates a Storage class which is responsible for storing information to
 * tables in PKB.
 */
class Storage {
public:
    /*
    * Explicit constructor for Storage.
    */
    explicit Storage();

    /*
    * Retrieve a table by TableName.
    */
    Table<TableValue> *getTable(TableName name);

private:
    std::map<TableName, Table<TableValue> *> tables;
};
