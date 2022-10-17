#include "ProcToVarRelationshipsTable.h"

bool ProcToVarRelationshipsTable::validate(Reference leftRef,
                                           Reference rightRef) {
    return validateHelper(leftRef, rightRef);
};

std::vector<Value> ProcToVarRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, StorageView *storage) {
    // Validate rightSynonym is a variable.
    if (rightSynonym != EntityName::VARIABLE) {
        return std::vector<Value>();
    }
    VariablesTable *variables = storage->getTable<VariablesTable>();
    std::vector<std::string> possibleRights =
        getNamesHelper<Variable>(variables);

    return solveRightHelper(&possibleRights, leftRef, ValueType::VAR_NAME);
}

std::vector<Value> ProcToVarRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, StorageView *storage) {
    // Validate leftSynonym is a procedure.
    if (leftSynonym != EntityName::PROCEDURE) {
        return std::vector<Value>();
    }
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    std::vector<std::string> possibleLefts =
        getNamesHelper<Procedure>(procedures);

    return solveLeftHelper(&possibleLefts, rightRef, ValueType::VAR_NAME);
};

std::vector<std::pair<Value, Value>> ProcToVarRelationshipsTable::solveBoth(
    EntityName leftSynonym, EntityName rightSynonym, StorageView *storage) {
    // Validate leftSynonym is a statement.
    if (leftSynonym != EntityName::PROCEDURE ||
        rightSynonym != EntityName::VARIABLE) {
        return std::vector<std::pair<Value, Value>>();
    }
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    std::vector<std::string> possibleLefts =
        getNamesHelper<Procedure>(procedures);
    std::vector<std::string> possibleRights =
        getNamesHelper<Variable>(variables);

    return solveBothHelper(&possibleLefts, &possibleRights, ValueType::VAR_NAME,
                           ValueType::VAR_NAME);
}
