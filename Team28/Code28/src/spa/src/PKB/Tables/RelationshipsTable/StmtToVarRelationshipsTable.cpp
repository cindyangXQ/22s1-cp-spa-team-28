#include "StmtToVarRelationshipsTable.h"

bool StmtToVarRelationshipsTable::validate(Reference leftRef,
                                           Reference rightRef) {
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        return !leftToRightsMap.empty();
    }
    if (leftRef.isWildcard()) {
        std::string right = rightRef.value.value;
        return !rightToLeftsMap[right].empty();
    }
    if (rightRef.isWildcard()) {
        int left = std::stoi(leftRef.value.value);
        return !leftToRightsMap[left].empty();
    }
    int left = std::stoi(leftRef.value.value);
    std::string right = rightRef.value.value;
    return leftToRightsMap[left].count(right) == 1;
};

std::vector<Value> StmtToVarRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, Storage *storage) {
    // Validate rightSynonym is a variable. TODO: throw error if not
    if (rightSynonym != EntityName::VARIABLE) {
        return std::vector<Value>();
    }
    VariablesTable *variables = storage->getTable<VariablesTable>();
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleRightsSet = variables->getAll();
    std::vector<std::string> possibleRights = std::vector<std::string>(
        possibleRightsSet.begin(), possibleRightsSet.end());
    std::unordered_set<Value> intermediateResult;
    if (leftRef.isWildcard()) {
        addNonemptyPossibleRights(&possibleRights, &intermediateResult,
                                  ValueType::VAR_NAME);
    } else {
        int left = std::stoi(leftRef.value.value);
        addPossibleRights(&possibleRights, left, &intermediateResult,
                          ValueType::VAR_NAME);
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<Value> StmtToVarRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, StatementsTable *statements) {
    // Validate leftSynonym is a statement. TODO: throw error if not
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
        std::string right = rightRef.value.value;
        addPossibleLefts(&possibleLefts, right, &intermediateResult,
                         ValueType::STMT_NUM);
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<std::pair<Value, Value>> StmtToVarRelationshipsTable::solveBoth(
    EntityName leftSynonym, EntityName rightSynonym,
    StatementsTable *statements, VariablesTable *variables) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    if (stmtRefSet.count(leftSynonym) == 0 ||
        rightSynonym != EntityName::VARIABLE) {
        return std::vector<std::pair<Value, Value>>();
    }
    std::vector<int> possibleLefts;
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleRightsSet = variables->getAll();
    std::vector<std::string> possibleRights = std::vector<std::string>(
        possibleRightsSet.begin(), possibleRightsSet.end());
    if (leftSynonym == EntityName::STMT) {
        possibleLefts = statements->getAllLineNumbers();
    } else {
        StatementType statementType =
            Statement::getStmtTypeFromEntityName(leftSynonym);
        possibleLefts = statements->getStatementsByType(statementType);
    }

    std::unordered_set<std::pair<Value, Value>, value_pair_hash>
        intermediateResult;
    addMatchingLeftRights(&possibleLefts, &possibleRights, &intermediateResult,
                          ValueType::STMT_NUM, ValueType::VAR_NAME);
    std::vector<std::pair<Value, Value>> result =
        std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                             intermediateResult.end());
    std::sort(result.begin(), result.end(), value_pair_sort());
    return result;
}
