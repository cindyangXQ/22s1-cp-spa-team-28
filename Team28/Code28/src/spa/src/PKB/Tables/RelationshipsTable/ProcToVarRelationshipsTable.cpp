#include "ProcToVarRelationshipsTable.h"

bool ProcToVarRelationshipsTable::validate(Reference leftRef,
                                           Reference rightRef) {
    // TODO: Better way to handle wildcards
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        return !leftToRightsMap.empty();
    }

    if (leftRef.isWildcard()) {
        std::string right = rightRef.value.value;
        return !rightToLeftsMap[right].empty();
    }
    if (rightRef.isWildcard()) {
        std::string left = leftRef.value.value;
        return !leftToRightsMap[left].empty();
    }
    std::string left = leftRef.value.value;
    std::string right = rightRef.value.value;
    return leftToRightsMap[left].count(right) == 1;
};

std::vector<Value> ProcToVarRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, VariablesTable *variables) {
    // Validate rightSynonym is a variable. TODO: throw error if not
    if (rightSynonym != EntityName::VARIABLE) {
        return std::vector<Value>();
    }
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleRightsSet = variables->getAll();
    std::vector<std::string> possibleRights = std::vector<std::string>(
        possibleRightsSet.begin(), possibleRightsSet.end());
    std::unordered_set<Value> intermediateResult;
    if (leftRef.isWildcard()) {
        addNonemptyPossibleRights(&possibleRights, &intermediateResult,
                                  ValueType::VAR_NAME);
    } else {
        std::string left = leftRef.value.value;
        addPossibleRights(&possibleRights, left, &intermediateResult,
                          ValueType::VAR_NAME);
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<Value> ProcToVarRelationshipsTable::solveLeft(
    Reference rightRef, EntityName leftSynonym, ProceduresTable *procedures) {
    // Validate leftSynonym is a procedure. TODO: throw error if not
    if (leftSynonym != EntityName::PROCEDURE) {
        return std::vector<Value>();
    }
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleLeftsSet = procedures->getAll();
    std::vector<std::string> possibleLefts = std::vector<std::string>(
        possibleLeftsSet.begin(), possibleLeftsSet.end());
    std::unordered_set<Value> intermediateResult;
    if (rightRef.isWildcard()) {
        addNonemptyPossibleLefts(&possibleLefts, &intermediateResult,
                                 ValueType::VAR_NAME);
    } else {
        std::string right = rightRef.value.value;
        addPossibleLefts(&possibleLefts, right, &intermediateResult,
                         ValueType::VAR_NAME);
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<std::pair<Value, Value>> ProcToVarRelationshipsTable::solveBoth(
    EntityName leftSynonym, EntityName rightSynonym,
    ProceduresTable *procedures, VariablesTable *variables) {
    // Validate leftSynonym is a statement. TODO: throw error if not
    if (leftSynonym != EntityName::PROCEDURE ||
        rightSynonym != EntityName::VARIABLE) {
        return std::vector<std::pair<Value, Value>>();
    }
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleLeftsSet = procedures->getAll();
    std::vector<std::string> possibleLefts = std::vector<std::string>(
        possibleLeftsSet.begin(), possibleLeftsSet.end());
    std::unordered_set<std::string> possibleRightsSet = variables->getAll();
    std::vector<std::string> possibleRights = std::vector<std::string>(
        possibleRightsSet.begin(), possibleRightsSet.end());

    std::unordered_set<std::pair<Value, Value>, value_pair_hash>
        intermediateResult;
    addMatchingLeftRights(&possibleLefts, &possibleRights, &intermediateResult,
                          ValueType::VAR_NAME, ValueType::VAR_NAME);
    std::vector<std::pair<Value, Value>> result =
        std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                             intermediateResult.end());
    std::sort(result.begin(), result.end(), value_pair_sort());
    return result;
}