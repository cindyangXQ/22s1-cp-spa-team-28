#include "StmtToStmtRelationshipsTable.h"

bool StmtToStmtRelationshipsTable::validate(Reference leftRef,
                                            Reference rightRef) {
    return validateHelper(leftRef, rightRef);
};

std::vector<Value> StmtToStmtRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, StorageView *storage) {
    // Validate rightSynonym is a statement. TODO: throw error if not
    if (stmtRefSet.count(rightSynonym) == 0) {
        return std::vector<Value>();
    }
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleRights =
        getStatementsHelper(statements, rightSynonym);

    return solveRightHelper(&possibleRights, leftRef, ValueType::STMT_NUM);
};

std::vector<Value> StmtToStmtRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, StorageView *storage) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    StatementsTable *statements = storage->getTable<StatementsTable>();
    if (stmtRefSet.count(leftSynonym) == 0) {
        return std::vector<Value>();
    }
    std::vector<int> possibleLefts =
        getStatementsHelper(statements, leftSynonym);

    return solveLeftHelper(&possibleLefts, rightRef, ValueType::STMT_NUM);
};

std::vector<std::pair<Value, Value>> StmtToStmtRelationshipsTable::solveBoth(
    EntityName leftSynonym, EntityName rightSynonym, StorageView *storage) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    if (stmtRefSet.count(leftSynonym) == 0 ||
        stmtRefSet.count(rightSynonym) == 0) {
        return std::vector<std::pair<Value, Value>>();
    }
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleLefts =
        getStatementsHelper(statements, leftSynonym);
    std::vector<int> possibleRights =
        getStatementsHelper(statements, rightSynonym);

    return solveBothHelper(&possibleLefts, &possibleRights, ValueType::STMT_NUM,
                           ValueType::STMT_NUM);
}
