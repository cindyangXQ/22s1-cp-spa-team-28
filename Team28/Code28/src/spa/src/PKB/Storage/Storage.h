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
#include "../../commons/Relationship.h"
#include "../../commons/TableValue.h"
#include "../../commons/Variable.h"

#include "../Tables/AssignmentsTable/AssignmentsTable.h"
#include "../Tables/NamesTable/NamesTable.h"
#include "../Tables/RelationshipsTable/ProcToProcRelationshipsTable.h"
#include "../Tables/RelationshipsTable/ProcToVarRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToProcRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToStmtRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToVarRelationshipsTable.h"
#include "../Tables/RelationshipsTable/UsesControlVarTable.h"
#include "../Tables/StatementsTable/StatementsTable.h"
#include "../Tables/Table.h"

#include "StorageView.h"

/*
 * Encapsulates a Storage class responsible for storing information to
 * tables in PKB.
 */
class Storage {
public:
    /*
     * Constructor for Storage.
     */
    Storage();

    /*
     * Retrieves a table by the templated class given.
     */
    template <typename Subclass> Subclass *getTable() {
        Table *table = this->tables.at(typeid(Subclass));
        return dynamic_cast<Subclass *>(table);
    };

    /*
     * Retrieves a table by the provided RelationshipReference.
     */
    Solvable *getRsTable(RelationshipReference rsRef, ReferenceType leftType);

    /*
     * Retrieves a table by the provided Designation.
     */
    Table *getDesignationTable(Designation designType);

    /*
     * Retrieves a UsesControlVarTable by the provided Designation.
     */
    UsesControlVarTable *getControlVarTable(Designation designType);

    /*
     * Retrieves Modifies Tables.
     */
    std::vector<Solvable *> getModifiesTables();

    /*
     * Retrieves Uses Tables.
     */
    std::vector<Solvable *> getUsesTables();

    /*
     * Returns the StorageView.
     */
    StorageView *getStorageView();

    Table *getAttributesTable(EntityName entity, EntityAttribute attribute);

private:
    std::map<std::type_index, Table *> tables;
    /*
     * Mapping of RelationshipReference to Solvable tables.
     * NOTE: Modifies and Uses are complex and won't be mapped here.
     */
    std::map<RelationshipReference, Solvable *> rsTables;
    std::map<std::pair<EntityName, EntityAttribute>, Table *> attributesTables;

    /*
     * Mapping of Designation to Table for external facing APIs in
     * PopulateFacade/QueryFacade to use.
     */
    std::map<Designation, Table *> designTables;
    StorageView *storageView;

    /*
     * Template method init and store Table that can be populated externally.
     */
    template <typename TableClass> void initTable(Designation designType) {
        TableClass *table = new TableClass();
        this->tables[typeid(TableClass)] = table;
        this->designTables[designType] = table;
    }

    /*
     * Template method init and store Tables that are populated internally.
     */
    template <typename TableClass> void initTable() {
        TableClass *table = new TableClass();
        this->tables[typeid(TableClass)] = table;
    }

    /*
     * Template method for getting tables used for Uses and Modifies.
     */
    template <typename STable, typename PTable>
    Solvable *getOnType(ReferenceType leftType) {
        if (leftType == ReferenceType::STMT_REF) {
            return this->getTable<STable>();
        }
        if (leftType == ReferenceType::ENT_REF) {
            return this->getTable<PTable>();
        }
        return nullptr;
    }

    /*
     * Helper functions to initialise tables and maps in storage
     */
    void initDesignEntitiesTable();
    void initRelationshipsTable();
    void initRsTablesMap();
    void initStorageView();
    void initAttributesTableMap();
};
