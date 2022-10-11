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

std::vector<Solvable *> Storage::getModifiesTables() {
    return std::vector<Solvable *>{this->getTable<ModifiesSTable>(),
                                   this->getTable<ModifiesPTable>()};
};

std::vector<Solvable *> Storage::getUsesTables() {
    return std::vector<Solvable *>{this->getTable<UsesSTable>(),
                                   this->getTable<UsesPTable>()};
};

StorageView *Storage::getStorageView() { return this->storageView; }
