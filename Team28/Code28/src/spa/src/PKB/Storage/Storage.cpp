#include "Storage.h"

Storage::Storage() {
    initDesignEntitiesTable();
    initRelationshipsTable();
    initRsTablesMap();
    initStorageView();
};

void Storage::initDesignEntitiesTable() {
    initTable<StatementsTable>(Populate::STMT);
    initTable<AssignmentsTable>(Populate::ASSIGN);
    initTable<ProceduresTable>(Populate::PROC);
    initTable<VariablesTable>(Populate::VAR);
    initTable<ConstantsTable>(Populate::CONST);
};

void Storage::initRelationshipsTable() {
    initTable<ParentTable>(Populate::PARENT);
    initTable<ParentTTable>(Populate::PARENT_T);
    initTable<FollowsTable>(Populate::FOLLOWS);
    initTable<FollowsTTable>(Populate::FOLLOWS_T);
    initTable<ModifiesSTable>(Populate::MOD_S);
    initTable<ModifiesPTable>(Populate::MOD_P);
    initTable<UsesSTable>(Populate::USE_S);
    initTable<UsesPTable>(Populate::USE_P);
    initTable<CallsTable>(Populate::CALL);
    initTable<CallsTTable>(Populate::CALL_T);
    initTable<BranchInTable>(Populate::B_IN);
    initTable<BranchOutTable>(Populate::B_OUT);
    initTable<IfControlVarTable>(Populate::IF_C);
    initTable<WhileControlVarTable>(Populate::WHILE_C);
    initTable<CallProcTable>(Populate::PROC_NAME);
    initTable<NextTable>();
    initTable<NextTTable>();
};

void Storage::initRsTablesMap() {
    this->rsTables[RelationshipReference::FOLLOWS] =
        this->getTable<FollowsTable>();
    this->rsTables[RelationshipReference::FOLLOWS_T] =
        this->getTable<FollowsTTable>();
    this->rsTables[RelationshipReference::PARENT] =
        this->getTable<ParentTable>();
    this->rsTables[RelationshipReference::PARENT_T] =
        this->getTable<ParentTTable>();
    this->rsTables[RelationshipReference::CALLS] = this->getTable<CallsTable>();
    this->rsTables[RelationshipReference::CALLS_T] =
        this->getTable<CallsTTable>();
    this->rsTables[RelationshipReference::NEXT] = this->getTable<NextTable>();
    this->rsTables[RelationshipReference::NEXT_T] =
        this->getTable<NextTTable>();
};

void Storage::initStorageView() {
    this->storageView = new StorageView();
    this->storageView->setTable<StatementsTable>(
        this->getTable<StatementsTable>());
    this->storageView->setTable<ProceduresTable>(
        this->getTable<ProceduresTable>());
    this->storageView->setTable<VariablesTable>(
        this->getTable<VariablesTable>());
    this->storageView->setTable<FollowsTable>(this->getTable<FollowsTable>());
    this->storageView->setTable<BranchInTable>(this->getTable<BranchInTable>());
    this->storageView->setTable<BranchOutTable>(
        this->getTable<BranchOutTable>());
};

Solvable *Storage::getRsTable(RelationshipReference rsRef,
                              ReferenceType leftType) {
    if (rsRef == RelationshipReference::MODIFIES) {
        return this->getOnType<ModifiesSTable, ModifiesPTable>(leftType);
    }
    if (rsRef == RelationshipReference::USES) {
        return this->getOnType<UsesSTable, UsesPTable>(leftType);
    }
    return this->rsTables.at(rsRef);
};

Table *Storage::getStoreTable(Populate popType) {
    return this->popTables.at(popType);
};

std::vector<Solvable *> Storage::getModifiesTables() {
    return std::vector<Solvable *>{this->getTable<ModifiesSTable>(),
                                   this->getTable<ModifiesPTable>()};
};

std::vector<Solvable *> Storage::getUsesTables() {
    return std::vector<Solvable *>{this->getTable<UsesSTable>(),
                                   this->getTable<UsesPTable>()};
};

StorageView *Storage::getStorageView() { return this->storageView; }
