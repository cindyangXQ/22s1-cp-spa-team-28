#include "PopulateFacade.h"

PopulateFacade::PopulateFacade(Storage *storage) { this->storage = storage; };

void PopulateFacade::storeStatements(std::vector<Statement *> *statements) {
    StatementsTable *statementsTable =
        this->storage->getTable<StatementsTable>(TableName::STATEMENTS);

    for (Statement *statement : *statements) {
        statementsTable->store(statement);
    }
};

void PopulateFacade::storeAssignments(std::vector<Assignment *> *assignments) {
    AssignmentsTable *assignmentsTable =
        this->storage->getTable<AssignmentsTable>(TableName::ASSIGNMENTS);

    for (Assignment *assignment : *assignments) {
        assignmentsTable->store(assignment);
    }
};

void PopulateFacade::storeVariables(std::vector<Variable *> *variables) {
    VariablesTable *variableTable =
        this->storage->getTable<VariablesTable>(TableName::VARIABLES);
    for (Variable *variable : *variables) {
        variableTable->store(variable);
    }
};

void PopulateFacade::storeConstants(std::vector<Constant *> *constants) {
    ConstantsTable *constantsTable =
        this->storage->getTable<ConstantsTable>(TableName::CONSTANTS);
    for (Constant *constant : *constants) {
        constantsTable->store(constant);
    }
};

void PopulateFacade::storeProcedures(std::vector<Procedure *> *procedures) {
    ProceduresTable *proceduresTable =
        this->storage->getTable<ProceduresTable>(TableName::PROCEDURES);
    for (Procedure *procedure : *procedures) {
        proceduresTable->store(procedure);
    }
};

void PopulateFacade::storeFollows(
    std::vector<Relationship<int, int> *> *follows) {
    FollowsTable *followsTable =
        this->storage->getTable<FollowsTable>(TableName::FOLLOWS);
    for (Relationship<int, int> *follow : *follows) {
        followsTable->store(follow);
    }
}

void PopulateFacade::storeFollowsT(
    std::vector<Relationship<int, int> *> *followsT) {
    FollowsTTable *followsTTable =
        this->storage->getTable<FollowsTTable>(TableName::FOLLOWS_T);
    for (Relationship<int, int> *follow : *followsT) {
        followsTTable->store(follow);
    }
}

void PopulateFacade::storeParent(
    std::vector<Relationship<int, int> *> *parents) {
    ParentTable *parentTable =
        this->storage->getTable<ParentTable>(TableName::PARENT);
    for (Relationship<int, int> *parent : *parents) {
        parentTable->store(parent);
    }
}

void PopulateFacade::storeParentT(
    std::vector<Relationship<int, int> *> *parentsT) {
    ParentTTable *parentTTable =
        this->storage->getTable<ParentTTable>(TableName::PARENT_T);
    for (Relationship<int, int> *parent : *parentsT) {
        parentTTable->store(parent);
    }
}

void PopulateFacade::storeModifiesS(
    std::vector<Relationship<int, std::string> *> *modifiesS) {
    ModifiesSTable *modifiesSTable =
        this->storage->getTable<ModifiesSTable>(TableName::MODIFIES_S);
    for (Relationship<int, std::string> *modify : *modifiesS) {
        modifiesSTable->store(modify);
    }
}

void PopulateFacade::storeModifiesP(
    std::vector<Relationship<std::string, std::string> *> *modifiesP) {
    ModifiesPTable *modifiesPTable =
        this->storage->getTable<ModifiesPTable>(TableName::MODIFIES_P);
    for (Relationship<std::string, std::string> *modify : *modifiesP) {
        modifiesPTable->store(modify);
    }
}

void PopulateFacade::storeUsesS(
    std::vector<Relationship<int, std::string> *> *usesS) {
    UsesSTable *usesSTable =
        this->storage->getTable<UsesSTable>(TableName::USES_S);
    for (Relationship<int, std::string> *use : *usesS) {
        usesSTable->store(use);
    }
}

void PopulateFacade::storeUsesP(
    std::vector<Relationship<std::string, std::string> *> *usesP) {
    UsesPTable *modifiesPTable =
        this->storage->getTable<UsesPTable>(TableName::USES_P);
    for (Relationship<std::string, std::string> *use : *usesP) {
        modifiesPTable->store(use);
    }
}

void PopulateFacade::storeCalls(
    std::vector<Relationship<std::string, std::string> *> *calls) {
    CallsTable *callsTable =
        this->storage->getTable<CallsTable>(TableName::CALLS);
    for (Relationship<std::string, std::string> *call : *calls) {
        callsTable->store(call);
    }
}

void PopulateFacade::storeCallsT(
    std::vector<Relationship<std::string, std::string> *> *callsT) {
    CallsTTable *callsTTable =
        this->storage->getTable<CallsTTable>(TableName::CALLS_T);
    for (Relationship<std::string, std::string> *call : *callsT) {
        callsTTable->store(call);
    }
}

void PopulateFacade::storeBranchIn(
    std::vector<Relationship<int, int> *> *branchIn) {
    BranchInTable *branchinTable =
        this->storage->getTable<BranchInTable>(TableName::BRANCH_IN);
    for (Relationship<int, int> *branchin : *branchIn) {
        branchinTable->store(branchin);
    }
}

void PopulateFacade::storeBranchOut(
    std::vector<Relationship<int, int> *> *branchOut) {
    BranchOutTable *branchoutTable =
        this->storage->getTable<BranchOutTable>(TableName::BRANCH_OUT);
    for (Relationship<int, int> *branchout : *branchOut) {
        branchoutTable->store(branchout);
    }
}

void PopulateFacade::storeIfControlVar(
    std::vector<Relationship<int, std::string> *> *usesS) {
    IfControlVarTable *ifControlVarTable =
        this->storage->getTable<IfControlVarTable>(TableName::I_CONTROL);
    for (Relationship<int, std::string> *use : *usesS) {
        ifControlVarTable->store(use);
    }
}

void PopulateFacade::storeWhileControlVar(
    std::vector<Relationship<int, std::string> *> *usesS) {
    WhileControlVarTable *whileControlVarTable =
        this->storage->getTable<WhileControlVarTable>(TableName::W_CONTROL);
    for (Relationship<int, std::string> *use : *usesS) {
        whileControlVarTable->store(use);
    }
}
