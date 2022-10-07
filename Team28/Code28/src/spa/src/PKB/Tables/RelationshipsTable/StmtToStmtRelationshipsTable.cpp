#include "StmtToStmtRelationshipsTable.h"

bool StmtToStmtRelationshipsTable::validate(Reference leftRef,
                                            Reference rightRef) {
    // TODO: Better way to handle wildcards
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        return !leftToRightsMap.empty();
    }

    if (leftRef.isWildcard()) {
        int right = std::stoi(rightRef.value.value);
        return !rightToLeftsMap[right].empty();
    }
    if (rightRef.isWildcard()) {
        int left = std::stoi(leftRef.value.value);
        return !leftToRightsMap[left].empty();
    }
    int left = std::stoi(leftRef.value.value);
    int right = std::stoi(rightRef.value.value);
    return leftToRightsMap[left].count(right) == 1;
};

std::vector<Value> StmtToStmtRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, StorageView *storage) {
    // Validate rightSynonym is a statement. TODO: throw error if not
    if (stmtRefSet.count(rightSynonym) == 0) {
        return std::vector<Value>();
    }
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleRights;
    if (rightSynonym == EntityName::STMT) {
        possibleRights = statements->getAllLineNumbers();
    } else {
        StatementType statementType =
            Statement::getStmtTypeFromEntityName(rightSynonym);
        possibleRights = statements->getStatementsByType(statementType);
    }
    std::unordered_set<Value> intermediateResult;
    if (leftRef.isWildcard()) {
        addNonemptyPossibleRights(&possibleRights, &intermediateResult,
                                  ValueType::STMT_NUM);
    } else {
        int left = std::stoi(leftRef.value.value);
        addPossibleRights(&possibleRights, left, &intermediateResult,
                          ValueType::STMT_NUM);
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<Value> StmtToStmtRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, StorageView* storage) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    StatementsTable *statements = storage->getTable<StatementsTable>();
    if (stmtRefSet.count(leftSynonym) == 0) {
        return std::vector<Value>();
    }
    std::vector<int> possibleLefts;
    if (leftSynonym == EntityName::STMT) {
        possibleLefts = statements->getAllLineNumbers();
    } else {
        StatementType statementType =
            Statement::getStmtTypeFromEntityName(leftSynonym);
        possibleLefts = statements->getStatementsByType(statementType);
    }
    std::unordered_set<Value> intermediateResult;
    if (rightRef.isWildcard()) {
        addNonemptyPossibleLefts(&possibleLefts, &intermediateResult,
                                 ValueType::STMT_NUM);
    } else {
        int right = std::stoi(rightRef.value.value);
        addPossibleLefts(&possibleLefts, right, &intermediateResult,
                         ValueType::STMT_NUM);
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<std::pair<Value, Value>>
StmtToStmtRelationshipsTable::solveBoth(EntityName leftSynonym,
                                        EntityName rightSynonym,
                                        StorageView* storage) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    if (stmtRefSet.count(leftSynonym) == 0 ||
        stmtRefSet.count(rightSynonym) == 0) {
        return std::vector<std::pair<Value, Value>>();
    }
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleLefts;
    std::vector<int> possibleRights;
    if (leftSynonym == EntityName::STMT) {
        possibleLefts = statements->getAllLineNumbers();
    } else {
        StatementType statementType =
            Statement::getStmtTypeFromEntityName(leftSynonym);
        possibleLefts = statements->getStatementsByType(statementType);
    }
    if (rightSynonym == EntityName::STMT) {
        possibleRights = statements->getAllLineNumbers();
    } else {
        StatementType statementType =
            Statement::getStmtTypeFromEntityName(rightSynonym);
        possibleRights = statements->getStatementsByType(statementType);
    }

    std::unordered_set<std::pair<Value, Value>, value_pair_hash>
        intermediateResult;
    addMatchingLeftRights(&possibleLefts, &possibleRights, &intermediateResult,
                          ValueType::STMT_NUM, ValueType::STMT_NUM);
    std::vector<std::pair<Value, Value>> result =
        std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                             intermediateResult.end());
    std::sort(result.begin(), result.end(), value_pair_sort());
    return result;
}
