#include "ProcToProcRelationshipsTable.h"

bool ProcToProcRelationshipsTable::validate(Reference leftRef,
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

std::vector<Value> ProcToProcRelationshipsTable::solveRight(
    Reference leftRef, EntityName rightSynonym, Storage *storage) {
    // Validate rightSynonym is a procedure. TODO: throw error if not
    if (rightSynonym != EntityName::PROCEDURE) {
        return std::vector<Value>();
    }
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> possibleRightsSet = procedures->getAll();
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

std::vector<Value> ProcToProcRelationshipsTable::solveLeft(
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

std::vector<std::pair<Value, Value>>
ProcToProcRelationshipsTable::solveBoth(EntityName leftSynonym,
                                        EntityName rightSynonym,
                                        ProceduresTable *procedures) {
    // Validate synonyms are both procedures here. TODO: throw error if not
    if (leftSynonym != EntityName::PROCEDURE ||
        rightSynonym != EntityName::PROCEDURE) {
        return std::vector<std::pair<Value, Value>>();
    }
    // TODO: iterate through set don't convert to vector
    std::unordered_set<std::string> allProcedures = procedures->getAll();
    std::vector<std::string> possibleLefts =
        std::vector<std::string>(allProcedures.begin(), allProcedures.end());
    std::vector<std::string> possibleRights =
        std::vector<std::string>(allProcedures.begin(), allProcedures.end());

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
