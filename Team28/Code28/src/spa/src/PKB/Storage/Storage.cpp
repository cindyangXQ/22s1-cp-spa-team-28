#include "Storage.h"

Storage::Storage() {
    initDesignEntitiesTable();
    initRelationshipsTable();
    initRsTablesMap();
    initStorageView();
};

void Storage::initDesignEntitiesTable() {
    initTable<StatementsTable>(Designation::STMT);
    initTable<AssignmentsTable>(Designation::ASSIGN);
    initTable<ProceduresTable>(Designation::PROC);
    initTable<VariablesTable>(Designation::VAR);
    initTable<ConstantsTable>(Designation::CONST);
};

void Storage::initRelationshipsTable() {
    initTable<ParentTable>(Designation::PARENT);
    initTable<ParentTTable>(Designation::PARENT_T);
    initTable<FollowsTable>(Designation::FOLLOWS);
    initTable<FollowsTTable>(Designation::FOLLOWS_T);
    initTable<ModifiesSTable>(Designation::MOD_S);
    initTable<ModifiesPTable>(Designation::MOD_P);
    initTable<UsesSTable>(Designation::USE_S);
    initTable<UsesPTable>(Designation::USE_P);
    initTable<CallsTable>(Designation::CALL);
    initTable<CallsTTable>(Designation::CALL_T);
    initTable<BranchInTable>(Designation::B_IN);
    initTable<BranchOutTable>(Designation::B_OUT);
    initTable<IfControlVarTable>(Designation::IF_C);
    initTable<WhileControlVarTable>(Designation::WHILE_C);
    initTable<CallProcTable>(Designation::PROC_NAME);
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

UsesControlVarTable *Storage::getControlVarTable(Designation designType) {
    if (designType == Designation::IF_C) {
        return this->getTable<IfControlVarTable>();
    }
    if (designType == Designation::WHILE_C) {
        return this->getTable<WhileControlVarTable>();
    }
    return nullptr;
};

Table *Storage::getDesignationTable(Designation designType) {
    return this->designTables.at(designType);
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
