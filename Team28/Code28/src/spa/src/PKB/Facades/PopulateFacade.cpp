#include "PopulateFacade.h"

PopulateFacade::PopulateFacade(Storage *storage) { this->storage = storage; };

void PopulateFacade::storeStatements(std::vector<Statement *> *statements) {
    storeHelper<Statement, StatementsTable>(statements);
};

void PopulateFacade::storeAssignments(std::vector<Assignment *> *assignments) {
    storeHelper<Assignment, AssignmentsTable>(assignments);
};

void PopulateFacade::storeVariables(std::vector<Variable *> *variables) {
    storeHelper<Variable, VariablesTable>(variables);
};

void PopulateFacade::storeConstants(std::vector<Constant *> *constants) {
    storeHelper<Constant, ConstantsTable>(constants);
};

void PopulateFacade::storeProcedures(std::vector<Procedure *> *procedures) {
    storeHelper<Procedure, ProceduresTable>(procedures);
};

void PopulateFacade::storeFollows(
    std::vector<Relationship<int, int> *> *follows) {
    storeHelper<Relationship<int, int>, FollowsTable>(follows);
}

void PopulateFacade::storeFollowsT(
    std::vector<Relationship<int, int> *> *followsT) {
    storeHelper<Relationship<int, int>, FollowsTTable>(followsT);
}

void PopulateFacade::storeParent(
    std::vector<Relationship<int, int> *> *parents) {
    storeHelper<Relationship<int, int>, ParentTable>(parents);
}

void PopulateFacade::storeParentT(
    std::vector<Relationship<int, int> *> *parentsT) {
    storeHelper<Relationship<int, int>, ParentTTable>(parentsT);
}

void PopulateFacade::storeModifiesS(
    std::vector<Relationship<int, std::string> *> *modifiesS) {
    storeHelper<Relationship<int, std::string>, ModifiesSTable>(modifiesS);
}

void PopulateFacade::storeModifiesP(
    std::vector<Relationship<std::string, std::string> *> *modifiesP) {
    storeHelper<Relationship<std::string, std::string>, ModifiesPTable>(
        modifiesP);
}

void PopulateFacade::storeUsesS(
    std::vector<Relationship<int, std::string> *> *usesS) {
    storeHelper<Relationship<int, std::string>, UsesSTable>(usesS);
}

void PopulateFacade::storeUsesP(
    std::vector<Relationship<std::string, std::string> *> *usesP) {
    storeHelper<Relationship<std::string, std::string>, UsesPTable>(usesP);
}

void PopulateFacade::storeCalls(
    std::vector<Relationship<std::string, std::string> *> *calls) {
    storeHelper<Relationship<std::string, std::string>, CallsTable>(calls);
}

void PopulateFacade::storeCallsT(
    std::vector<Relationship<std::string, std::string> *> *callsT) {
    storeHelper<Relationship<std::string, std::string>, CallsTTable>(callsT);
}

void PopulateFacade::storeBranchIn(
    std::vector<Relationship<int, int> *> *branchIn) {
    storeHelper<Relationship<int, int>, BranchInTable>(branchIn);
}

void PopulateFacade::storeBranchOut(
    std::vector<Relationship<int, int> *> *branchOut) {
    storeHelper<Relationship<int, int>, BranchOutTable>(branchOut);
}

void PopulateFacade::storeIfControlVar(
    std::vector<Relationship<int, std::string> *> *ifControls) {
    storeHelper<Relationship<int, std::string>, IfControlVarTable>(ifControls);
}

void PopulateFacade::storeWhileControlVar(
    std::vector<Relationship<int, std::string> *> *whileControls) {
    storeHelper<Relationship<int, std::string>, WhileControlVarTable>(
        whileControls);
}
