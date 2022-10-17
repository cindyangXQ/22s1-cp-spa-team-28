#include "StmtToVarRelationshipsTable.h"

bool StmtToVarRelationshipsTable::validate(Reference leftRef,
                                           Reference rightRef) {
    return validateHelper(leftRef, rightRef);
};

std::vector<Value> StmtToVarRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, StorageView *storage) {
    // Validate rightSynonym is a variable.
    if (rightSynonym != EntityName::VARIABLE) {
        return std::vector<Value>();
    }
    VariablesTable *variables = storage->getTable<VariablesTable>();
    std::vector<std::string> possibleRights =
        getNamesHelper<Variable>(variables);

    return solveRightHelper(&possibleRights, leftRef, ValueType::VAR_NAME);
};

std::vector<Value> StmtToVarRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, StorageView *storage) {
    // Validate leftSynonym is a statement.
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
    // Validate leftSynonym is a statement.
    if (stmtRefSet.count(leftSynonym) == 0 ||
        rightSynonym != EntityName::VARIABLE) {
        return std::vector<std::pair<Value, Value>>();
    }
    StatementsTable *statements = storage->getTable<StatementsTable>();
    VariablesTable *variables = storage->getTable<VariablesTable>();
    std::vector<int> possibleLefts =
        getStatementsHelper(statements, leftSynonym);
    std::vector<std::string> possibleRights =
        getNamesHelper<Variable>(variables);

    return solveBothHelper(&possibleLefts, &possibleRights, ValueType::STMT_NUM,
                           ValueType::VAR_NAME);
}
