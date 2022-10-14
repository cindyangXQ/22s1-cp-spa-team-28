#include "Storage.h"

Storage::Storage() {
    StatementsTable *statements = new StatementsTable();
    AssignmentsTable *assignments = new AssignmentsTable();
    ProceduresTable *procedures = new ProceduresTable();
    VariablesTable *variables = new VariablesTable();
    ConstantsTable *constants = new ConstantsTable();
    ParentTable *parents = new ParentTable();
    ParentTTable *parentsT = new ParentTTable();
    FollowsTable *follows = new FollowsTable();
    FollowsTTable *followsT = new FollowsTTable();
    ModifiesSTable *modifiesS = new ModifiesSTable();
    ModifiesPTable *modifiesP = new ModifiesPTable();
    UsesSTable *usesS = new UsesSTable();
    UsesPTable *usesP = new UsesPTable();
    CallsTable *calls = new CallsTable();
    CallsTTable *callsT = new CallsTTable();
    BranchInTable *branchIn = new BranchInTable();
    BranchOutTable *branchOut = new BranchOutTable();
    NextTable *next = new NextTable();
    NextTTable *nextT = new NextTTable();
    IfControlVarTable *ifControl = new IfControlVarTable();
    WhileControlVarTable *whileControl = new WhileControlVarTable();
    CallProcTable *callProcs = new CallProcTable();

    this->tables[typeid(StatementsTable)] = statements;
    this->tables[typeid(AssignmentsTable)] = assignments;
    this->tables[typeid(ProceduresTable)] = procedures;
    this->tables[typeid(VariablesTable)] = variables;
    this->tables[typeid(ConstantsTable)] = constants;
    this->tables[typeid(ParentTable)] = parents;
    this->tables[typeid(ParentTTable)] = parentsT;
    this->tables[typeid(FollowsTable)] = follows;
    this->tables[typeid(FollowsTTable)] = followsT;
    this->tables[typeid(ModifiesSTable)] = modifiesS;
    this->tables[typeid(ModifiesPTable)] = modifiesP;
    this->tables[typeid(UsesSTable)] = usesS;
    this->tables[typeid(UsesPTable)] = usesP;
    this->tables[typeid(CallsTable)] = calls;
    this->tables[typeid(CallsTTable)] = callsT;
    this->tables[typeid(BranchInTable)] = branchIn;
    this->tables[typeid(BranchOutTable)] = branchOut;
    this->tables[typeid(NextTable)] = next;
    this->tables[typeid(NextTTable)] = nextT;
    this->tables[typeid(IfControlVarTable)] = ifControl;
    this->tables[typeid(WhileControlVarTable)] = whileControl;
    this->tables[typeid(CallProcTable)] = callProcs;

    this->popTables[Populate::STMT] = statements;
    this->popTables[Populate::ASSIGN] = assignments;
    this->popTables[Populate::PROC] = procedures;
    this->popTables[Populate::VAR] = variables;
    this->popTables[Populate::CONST] = constants;
    this->popTables[Populate::PARENT] = parents;
    this->popTables[Populate::PARENT_T] = parentsT;
    this->popTables[Populate::FOLLOWS] = follows;
    this->popTables[Populate::FOLLOWS_T] = followsT;
    this->popTables[Populate::MOD_S] = modifiesS;
    this->popTables[Populate::MOD_P] = modifiesP;
    this->popTables[Populate::USE_S] = usesS;
    this->popTables[Populate::USE_P] = usesP;
    this->popTables[Populate::CALL] = calls;
    this->popTables[Populate::CALL_T] = callsT;
    this->popTables[Populate::B_IN] = branchIn;
    this->popTables[Populate::B_OUT] = branchOut;
    this->popTables[Populate::IF_C] = ifControl;
    this->popTables[Populate::WHILE_C] = whileControl;
    this->popTables[Populate::PROC_NAME] = callProcs;

    this->rsTables[RelationshipReference::FOLLOWS] = follows;
    this->rsTables[RelationshipReference::FOLLOWS_T] = followsT;
    this->rsTables[RelationshipReference::PARENT] = parents;
    this->rsTables[RelationshipReference::PARENT_T] = parentsT;
    this->rsTables[RelationshipReference::CALLS] = calls;
    this->rsTables[RelationshipReference::CALLS_T] = callsT;
    this->rsTables[RelationshipReference::NEXT] = next;
    this->rsTables[RelationshipReference::NEXT_T] = nextT;

    this->storageView = new StorageView();
    this->storageView->setTable<StatementsTable>(statements);
    this->storageView->setTable<ProceduresTable>(procedures);
    this->storageView->setTable<VariablesTable>(variables);
    this->storageView->setTable<FollowsTable>(follows);
    this->storageView->setTable<BranchInTable>(branchIn);
    this->storageView->setTable<BranchOutTable>(branchOut);

    initAttributesTableMap();
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
}

std::vector<Solvable *> Storage::getModifiesTables() {
    return std::vector<Solvable *>{this->getTable<ModifiesSTable>(),
                                   this->getTable<ModifiesPTable>()};
};

std::vector<Solvable *> Storage::getUsesTables() {
    return std::vector<Solvable *>{this->getTable<UsesSTable>(),
                                   this->getTable<UsesPTable>()};
};

StorageView *Storage::getStorageView() { return this->storageView; };

void Storage::initAttributesTableMap() {
    this->attributesTables = {
        {std::make_pair(EntityName::STMT, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::READ, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::PRINT, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::CALL, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::WHILE, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::IF, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::ASSIGN, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::CONSTANT, EntityAttribute::VALUE),
         this->getTable<ConstantsTable>()},
        {std::make_pair(EntityName::PROCEDURE, EntityAttribute::PROC_NAME),
         this->getTable<ProceduresTable>()},
        {std::make_pair(EntityName::CALL, EntityAttribute::PROC_NAME),
         this->getTable<CallProcTable>()},
        {std::make_pair(EntityName::VARIABLE, EntityAttribute::VAR_NAME),
         this->getTable<VariablesTable>()},
        {std::make_pair(EntityName::READ, EntityAttribute::VAR_NAME),
         this->getTable<ModifiesSTable>()},
        {std::make_pair(EntityName::PRINT, EntityAttribute::VAR_NAME),
         this->getTable<UsesSTable>()}};
};