#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <unordered_set>

#include "../commons/Reference.h"
#include "../commons/Relationship.h"
#include "../commons/StringUtil.h"
#include "NamesTable.h"
#include "StatementsTable.h"
#include "Table.h"

template <typename Left, typename Right>
class RelationshipsTable : public Table<Relationship<Left, Right>> {
public:
    /*
     * Stores a Relationship to both leftToRightsMap and rightToLeftsMap.
     */
    void store(Relationship<Left, Right> *relationship) {
        Left left = relationship->getLeft();
        Right right = relationship->getRight();
        storeRightToLeftMap(left, right);
        storeLeftToRightMap(right, left);
    }

    /*
     * Stores a Left to RightToLeftsMap.
     */
    void storeLeftToRightMap(Right right, Left left) {
        auto key = this->rightToLeftsMap.find(right);

        if (key != this->rightToLeftsMap.end()) {
            key->second.insert(left);
        } else {
            rightToLeftsMap[right] = {left};
        }
    }

    /*
     * Stores a Right to LeftToRightsMap.
     */
    void storeRightToLeftMap(Left left, Right right) {
        auto key = this->leftToRightsMap.find(left);

        if (key != this->leftToRightsMap.end()) {
            key->second.insert(right);
        } else {
            leftToRightsMap[left] = {right};
        }
    }

    /*
     * Retrieves all Rights associated to a given Left.
     */
    std::unordered_set<Right> retrieveLeft(Left left) {
        return this->leftToRightsMap[left];
    }

    /*
     * Retrieves all Lefts associated to a given Right.
     */
    std::unordered_set<Left> retrieveRight(Right right) {
        return this->rightToLeftsMap[right];
    }

    int getTableSize() const {
        return -1; // TODO change behaviour, now returning dummy value
    }

    std::map<Left, std::unordered_set<Right>> getLeftMap() {
        return this->leftToRightsMap;
    }

    std::map<Right, std::unordered_set<Left>> getRightMap() {
        return this->rightToLeftsMap;
    }

protected:
    std::map<Left, std::unordered_set<Right>> leftToRightsMap;
    std::map<Right, std::unordered_set<Left>> rightToLeftsMap;

    /*
     * Adds all possibleRights from the given left into intermediateResult.
     */
    void addPossibleRights(std::vector<Right> *possibleRights, Left left,
                           std::unordered_set<Value> *intermediateResult,
                           ValueType valueType) {
        for (Right right : *possibleRights) {
            if (rightToLeftsMap[right].count(left) == 1) {
                intermediateResult->insert(Value(valueType, toString(right)));
            }
        }
    }

    /*
     * Adds all possibleRights that are non-empty into intermediateResult.
     */
    void
    addNonemptyPossibleRights(std::vector<Right> *possibleRights,
                              std::unordered_set<Value> *intermediateResult,
                              ValueType valueType) {
        for (Right right : *possibleRights) {
            if (rightToLeftsMap[right].size() != 0) {
                intermediateResult->insert(Value(valueType, toString(right)));
            }
        }
    }

    /*
     * Adds all possibleLefts from the given right into intermediateResult.
     */
    void addPossibleLefts(std::vector<Left> *possibleLefts, Right right,
                          std::unordered_set<Value> *intermediateResult,
                          ValueType valueType) {
        for (Left left : *possibleLefts) {
            if (leftToRightsMap[left].count(right) == 1) {
                intermediateResult->insert(Value(valueType, toString(left)));
            }
        }
    }

    /*
     * Adds all possibleLefts that are non-empty into intermediateResult
     */
    void addNonemptyPossibleLefts(std::vector<Left> *possibleLefts,
                                  std::unordered_set<Value> *intermediateResult,
                                  ValueType valueType) {
        for (Left left : *possibleLefts) {
            if (leftToRightsMap[left].size() != 0) {
                intermediateResult->insert(Value(valueType, toString(left)));
            }
        }
    }

    /*
     * Adds all possibleLefts and possibleRights that are in a relationship
     * into the intermediateResults.
     */
    void addMatchingLeftRights(
        std::vector<Left> *possibleLefts, std::vector<Right> *possibleRights,
        std::unordered_set<std::pair<Value, Value>, value_pair_hash>
            *intermediateResult,
        ValueType leftValueType, ValueType rightValueType) {
        for (Left left : *possibleLefts) {
            for (Right right : *possibleRights) {
                if (leftToRightsMap[left].count(right) == 1) {
                    Value leftValue = Value(leftValueType, toString(left));
                    Value rightValue = Value(rightValueType, toString(right));
                    intermediateResult->insert(
                        std::make_pair(leftValue, rightValue));
                }
            }
        }
    }
};

class StmtToVarRelationshipsTable
    : public RelationshipsTable<int, std::string> {
public:
    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    bool validate(Reference leftRef, Reference rightRef) {
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
    }

    /*
     * Returns list of possible values that the right synonym can be.
     */
    std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym,
                                  VariablesTable *variables) {
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
            int left = std::stoi(leftRef.value.value);
            addPossibleRights(&possibleRights, left, &intermediateResult,
                              ValueType::VAR_NAME);
        }
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        std::sort(result.begin(), result.end());
        return result;
    };

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 StatementsTable *statements) {
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
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        std::sort(result.begin(), result.end());
        return result;
    };

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym,
                                                   EntityName rightSynonym,
                                                   StatementsTable *statements,
                                                   VariablesTable *variables) {
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
        addMatchingLeftRights(&possibleLefts, &possibleRights,
                              &intermediateResult, ValueType::STMT_NUM,
                              ValueType::VAR_NAME);
        std::vector<std::pair<Value, Value>> result =
            std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                                 intermediateResult.end());
        std::sort(result.begin(), result.end(), value_pair_sort());
        return result;
    }
};

class ProcToVarRelationshipsTable
    : public RelationshipsTable<std::string, std::string> {
public:
    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    bool validate(Reference leftRef, Reference rightRef) {
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

    /*
     * Returns list of possible values that the right synonym can be.
     */
    std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym,
                                  VariablesTable *variables) {
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
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        std::sort(result.begin(), result.end());
        return result;
    };

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 ProceduresTable *procedures) {
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
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        std::sort(result.begin(), result.end());
        return result;
    };

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym,
                                                   EntityName rightSynonym,
                                                   ProceduresTable *procedures,
                                                   VariablesTable *variables) {
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
        addMatchingLeftRights(&possibleLefts, &possibleRights,
                              &intermediateResult, ValueType::VAR_NAME,
                              ValueType::VAR_NAME);
        std::vector<std::pair<Value, Value>> result =
            std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                                 intermediateResult.end());
        std::sort(result.begin(), result.end(), value_pair_sort());
        return result;
    }
};

class StmtToStmtRelationshipsTable : public RelationshipsTable<int, int> {
public:
    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    bool validate(Reference leftRef, Reference rightRef) {
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

    /*
     * Returns list of possible values that the right synonym can be.
     */
    std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym,
                                  StatementsTable *statements) {
        // Validate rightSynonym is a statement. TODO: throw error if not
        if (stmtRefSet.count(rightSynonym) == 0) {
            return std::vector<Value>();
        }
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
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        std::sort(result.begin(), result.end());
        return result;
    };

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 StatementsTable *statements) {
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
            int right = std::stoi(rightRef.value.value);
            addPossibleLefts(&possibleLefts, right, &intermediateResult,
                             ValueType::STMT_NUM);
        }
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        std::sort(result.begin(), result.end());
        return result;
    };

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>>
    solveBoth(EntityName leftSynonym, EntityName rightSynonym,
              StatementsTable *statements) {
        // Validate leftSynonym is a statement. TODO: throw error if not
        if (stmtRefSet.count(leftSynonym) == 0 ||
            stmtRefSet.count(rightSynonym) == 0) {
            return std::vector<std::pair<Value, Value>>();
        }
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
        addMatchingLeftRights(&possibleLefts, &possibleRights,
                              &intermediateResult, ValueType::STMT_NUM,
                              ValueType::STMT_NUM);
        std::vector<std::pair<Value, Value>> result =
            std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                                 intermediateResult.end());
        std::sort(result.begin(), result.end(), value_pair_sort());
        return result;
    }
};

typedef StmtToStmtRelationshipsTable ParentTable;
typedef StmtToStmtRelationshipsTable ParentTTable;
typedef StmtToStmtRelationshipsTable FollowsTable;
typedef StmtToStmtRelationshipsTable FollowsTTable;
typedef StmtToVarRelationshipsTable ModifiesSTable;
typedef StmtToVarRelationshipsTable UsesSTable;
typedef ProcToVarRelationshipsTable ModifiesPTable;
typedef ProcToVarRelationshipsTable UsesPTable;
