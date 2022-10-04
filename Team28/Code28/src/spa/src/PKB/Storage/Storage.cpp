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

    /*
     * TODO relook typecasting
     */
    this->tables[TableName::STATEMENTS] = statements;
    this->tables[TableName::ASSIGNMENTS] = assignments;
    this->tables[TableName::PROCEDURES] = procedures;
    this->tables[TableName::VARIABLES] = variables;
    this->tables[TableName::CONSTANTS] = constants;
    this->tables[TableName::PARENT] = parents;
    this->tables[TableName::PARENT_T] = parentsT;
    this->tables[TableName::FOLLOWS] = follows;
    this->tables[TableName::FOLLOWS_T] = followsT;
    this->tables[TableName::MODIFIES_S] = modifiesS;
    this->tables[TableName::MODIFIES_P] = modifiesP;
    this->tables[TableName::USES_S] = usesS;
    this->tables[TableName::USES_P] = usesP;
    this->tables[TableName::CALLS] = calls;
    this->tables[TableName::CALLS_T] = callsT;
    this->tables[TableName::BRANCH_IN] = branchIn;
    this->tables[TableName::BRANCH_OUT] = branchOut;
    this->tables[TableName::NEXT] = next;
    this->tables[TableName::NEXT_T] = nextT;
    this->tables[TableName::I_CONTROL] = ifControl;
    this->tables[TableName::W_CONTROL] = whileControl;
};
