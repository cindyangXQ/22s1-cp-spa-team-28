#include "ProcToProcRelationshipsTable.h"

bool ProcToProcRelationshipsTable::validate(Reference leftRef,
                                            Reference rightRef) {
    return validateHelper(leftRef, rightRef);
};

std::vector<Value> ProcToProcRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, StorageView *storage) {
    // Validate rightSynonym is a procedure.
    if (rightSynonym != EntityName::PROCEDURE) {
        return std::vector<Value>();
    }
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleRightsSet = procedures->getAll();
    std::vector<std::string> possibleRights = std::vector<std::string>(
        possibleRightsSet.begin(), possibleRightsSet.end());

    return solveRightHelper(&possibleRights, leftRef, ValueType::VAR_NAME);
}

std::vector<Value> ProcToProcRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, StorageView *storage) {
    // Validate leftSynonym is a procedure.
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

std::vector<std::pair<Value, Value>> ProcToProcRelationshipsTable::solveBoth(
    EntityName leftSynonym, EntityName rightSynonym, StorageView *storage) {
    // Validate synonyms are both procedures here. TODO: throw error if not
    if (leftSynonym != EntityName::PROCEDURE ||
        rightSynonym != EntityName::PROCEDURE) {
        return std::vector<std::pair<Value, Value>>();
    }
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> allProcedures = procedures->getAll();
    std::vector<std::string> possibleLefts =
        std::vector<std::string>(allProcedures.begin(), allProcedures.end());
    std::vector<std::string> possibleRights =
        std::vector<std::string>(allProcedures.begin(), allProcedures.end());

    return solveBothHelper(&possibleLefts, &possibleRights, ValueType::VAR_NAME,
                           ValueType::VAR_NAME);
}
