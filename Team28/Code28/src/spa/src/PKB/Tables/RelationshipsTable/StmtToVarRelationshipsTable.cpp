#include "StmtToVarRelationshipsTable.h"

bool StmtToVarRelationshipsTable::validate(Reference leftRef,
                                           Reference rightRef) {
    return validateHelper(leftRef, rightRef);
};

std::vector<Value> StmtToVarRelationshipsTable::solveRight(
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
};

std::vector<Value> StmtToVarRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, StorageView *storage) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    if (stmtRefSet.count(leftSynonym) == 0) {
        return std::vector<Value>();
    }
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleLefts =
        getStatementsHelper(statements, leftSynonym);

    return solveLeftHelper(&possibleLefts, rightRef, ValueType::STMT_NUM);
};

std::vector<std::pair<Value, Value>> StmtToVarRelationshipsTable::solveBoth(
    EntityName leftSynonym, EntityName rightSynonym, StorageView *storage) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    if (stmtRefSet.count(leftSynonym) == 0 ||
        rightSynonym != EntityName::VARIABLE) {
        return std::vector<std::pair<Value, Value>>();
    }
    StatementsTable *statements = storage->getTable<StatementsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    std::vector<int> possibleLefts =
        getStatementsHelper(statements, leftSynonym);
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleRightsSet = variables->getAll();
    std::vector<std::string> possibleRights = std::vector<std::string>(
        possibleRightsSet.begin(), possibleRightsSet.end());

    return solveBothHelper(&possibleLefts, &possibleRights, ValueType::STMT_NUM,
                           ValueType::VAR_NAME);
}
