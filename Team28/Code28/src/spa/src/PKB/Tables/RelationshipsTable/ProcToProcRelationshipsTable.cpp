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
    std::vector<std::string> possibleRights =
        getNamesHelper<Procedure>(procedures);

    return solveRightHelper(&possibleRights, leftRef, ValueType::VAR_NAME);
}

std::vector<Value> ProcToProcRelationshipsTable::solveLeft(
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

std::vector<std::pair<Value, Value>> ProcToProcRelationshipsTable::solveBoth(
    EntityName leftSynonym, EntityName rightSynonym, StorageView *storage) {
    // Validate synonyms are both procedures here. TODO: throw error if not
    if (leftSynonym != EntityName::PROCEDURE ||
        rightSynonym != EntityName::PROCEDURE) {
        return std::vector<std::pair<Value, Value>>();
    }
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    std::vector<std::string> possibleNames =
        getNamesHelper<Procedure>(procedures);

    return solveBothHelper(&possibleNames, &possibleNames, ValueType::VAR_NAME,
                           ValueType::VAR_NAME);
}
