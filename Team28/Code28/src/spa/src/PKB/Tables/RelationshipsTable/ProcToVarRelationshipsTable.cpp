#include "ProcToVarRelationshipsTable.h"

bool ProcToVarRelationshipsTable::validate(Reference leftRef,
                                           Reference rightRef) {
    return validateHelper(leftRef, rightRef);
};

std::vector<Value> ProcToVarRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, StorageView *storage) {
    // Validate rightSynonym is a variable. TODO: throw error if not
    if (rightSynonym != EntityName::VARIABLE) {
        return std::vector<Value>();
    }
    VariablesTable *variables = storage->getTable<VariablesTable>();
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleRightsSet = variables->getAll();
    std::vector<std::string> possibleRights = std::vector<std::string>(
        possibleRightsSet.begin(), possibleRightsSet.end());

    return solveRightHelper(&possibleRights, leftRef, ValueType::VAR_NAME);
}

std::vector<Value> ProcToVarRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, StorageView *storage) {
    // Validate leftSynonym is a procedure. TODO: throw error if not
    if (leftSynonym != EntityName::PROCEDURE) {
        return std::vector<Value>();
    }
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleLeftsSet = procedures->getAll();
    std::vector<std::string> possibleLefts = std::vector<std::string>(
        possibleLeftsSet.begin(), possibleLeftsSet.end());

    return solveLeftHelper(&possibleLefts, rightRef, ValueType::VAR_NAME);
};

std::vector<std::pair<Value, Value>> ProcToVarRelationshipsTable::solveBoth(
    EntityName leftSynonym, EntityName rightSynonym, StorageView *storage) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    if (leftSynonym != EntityName::PROCEDURE ||
        rightSynonym != EntityName::VARIABLE) {
        return std::vector<std::pair<Value, Value>>();
    }
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleLeftsSet = procedures->getAll();
    std::vector<std::string> possibleLefts = std::vector<std::string>(
        possibleLeftsSet.begin(), possibleLeftsSet.end());
    std::unordered_set<std::string> possibleRightsSet = variables->getAll();
    std::vector<std::string> possibleRights = std::vector<std::string>(
        possibleRightsSet.begin(), possibleRightsSet.end());

    return solveBothHelper(&possibleLefts, &possibleRights, ValueType::VAR_NAME,
                           ValueType::VAR_NAME);
}
