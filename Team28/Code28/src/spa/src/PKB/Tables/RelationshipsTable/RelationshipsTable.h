#pragma once

#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "../../../commons/Reference.h"
#include "../../../commons/Relationship.h"
#include "../../../commons/StringUtil.h"
#include "../NamesTable/NamesTable.h"
#include "../StatementsTable/StatementsTable.h"
#include "../Table.h"

template <typename Left, typename Right>
class RelationshipsTable : public Table {
public:
    /*
     * Stores a Relationship to both leftToRightsMap and rightToLeftsMap.
     */
    void store(TableValue *relationship) {
        Relationship<Left, Right> *rs =
            static_cast<Relationship<Left, Right> *>(relationship);
        Left left = rs->getLeft();
        Right right = rs->getRight();
        storeRightToLeftMap(left, right);
        storeLeftToRightMap(right, left);
        this->tableSize++;
    }

    int getTableSize() { return this->tableSize; }

    /*
     * Returns string form of all relationships.
     */
    std::vector<std::string> getAllAsString() {
        std::vector<std::string> result = {};
        for (auto const &[left, val] : leftToRightsMap) {
            for (Right right : val) {
                result.push_back(toString(left) + ":" + toString(right));
            }
        }
        return result;
    };

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

    std::map<Left, std::unordered_set<Right>> getLeftMap() {
        return this->leftToRightsMap;
    }

    std::map<Right, std::unordered_set<Left>> getRightMap() {
        return this->rightToLeftsMap;
    }

    bool isLeftValueExist(Left left) {
        return this->retrieveLeft(left).size() != 0;
    }

    bool isRelationshipExist(Left left, Right right) {
        return this->retrieveLeft(left).count(right) > 0;
    }

    /*
     * Retrieves the only Right from the given Left.
     * Throws an exception if Left is mapped to multiple Rights or if Left does
     * not exist.
     */
    Right retrieveSingleRight(Left left) {
        if (this->retrieveLeft(left).size() == 0) {
            throw std::invalid_argument(LEFT_DOES_NOT_EXIST);
        }
        if (this->retrieveLeft(left).size() != 1) {
            throw std::invalid_argument(MORE_THAN_ONE_RIGHT);
        }
        return *this->retrieveLeft(left).begin();
    }

    virtual std::vector<Value> getMatchingValue(std::string value,
                                                EntityName entity) {
        UNUSED(value);
        UNUSED(entity);
        return std::vector<Value>{};
    };

    virtual std::map<Value, std::vector<Value>>
    getAllValues(EntityName entity) {
        UNUSED(entity);
        return std::map<Value, std::vector<Value>>{};
    };

protected:
    std::map<Left, std::unordered_set<Right>> leftToRightsMap;
    std::map<Right, std::unordered_set<Left>> rightToLeftsMap;
    int tableSize = 0;

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

    /*
     * Helper function to validate that both left and right exists in table for
     * Solvable RelationshipsTable.
     */
    bool validateHelper(Reference leftRef, Reference rightRef) {
        if (leftRef.isWildcard() && rightRef.isWildcard()) {
            return !leftToRightsMap.empty();
        }

        if (leftRef.isWildcard()) {
            Right right = convertToType<Right>(rightRef.getValueString());
            return !rightToLeftsMap[right].empty();
        }
        if (rightRef.isWildcard()) {
            Left left = convertToType<Left>(leftRef.getValueString());
            return !leftToRightsMap[left].empty();
        }
        Left left = convertToType<Left>(leftRef.getValueString());
        Right right = convertToType<Right>(rightRef.getValueString());
        return leftToRightsMap[left].count(right) == 1;
    }

    /*
     * Helper function to solveRight for Solvable RelationshipsTable.
     */
    std::vector<Value> solveRightHelper(std::vector<Right> *possibleRights,
                                        Reference leftRef,
                                        ValueType valueType) {
        std::unordered_set<Value> intermediateResult;
        if (leftRef.isWildcard()) {
            addNonemptyPossibleRights(possibleRights, &intermediateResult,
                                      valueType);
        } else {
            Left left = convertToType<Left>(leftRef.getValueString());
            addPossibleRights(possibleRights, left, &intermediateResult,
                              valueType);
        }
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        return result;
    }

    /*
     * Helper function to solveLeft for Solvable RelationshipsTable.
     */
    std::vector<Value> solveLeftHelper(std::vector<Left> *possibleLefts,
                                       Reference rightRef,
                                       ValueType valueType) {
        std::unordered_set<Value> intermediateResult;
        if (rightRef.isWildcard()) {
            addNonemptyPossibleLefts(possibleLefts, &intermediateResult,
                                     valueType);
        } else {
            Right right = convertToType<Right>(rightRef.getValueString());
            addPossibleLefts(possibleLefts, right, &intermediateResult,
                             valueType);
        }
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        return result;
    }

    /*
     * Helper function to solveBoth for Solvable RelationshipsTable.
     */
    std::vector<std::pair<Value, Value>>
    solveBothHelper(std::vector<Left> *possibleLefts,
                    std::vector<Right> *possibleRights, ValueType leftValueType,
                    ValueType rightValueType) {
        std::unordered_set<std::pair<Value, Value>, value_pair_hash>
            intermediateResult;
        addMatchingLeftRights(possibleLefts, possibleRights,
                              &intermediateResult, leftValueType,
                              rightValueType);
        std::vector<std::pair<Value, Value>> result =
            std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                                 intermediateResult.end());
        return result;
    }

    /*
     * Helper function to get StmtNum based on EntityName.
     */
    std::vector<int> getStatementsHelper(StatementsTable *statements,
                                         EntityName synName) {
        if (synName == EntityName::STMT) {
            return statements->getAllLineNumbers();
        }
        StatementType statementType =
            Statement::getStmtTypeFromEntityName(synName);
        return statements->getStatementsByType(statementType);
    }

    /*
     * Helper function to get names from the given NamesTable.
     */
    template <typename T>
    std::vector<std::string> getNamesHelper(NamesTable<T> *namesTable) {
        // TODO: consider returning set directly
        std::unordered_set<std::string> namesSet = namesTable->getAll();
        std::vector<std::string> namesVector =
            std::vector<std::string>(namesSet.begin(), namesSet.end());
        return namesVector;
    }

private:
    const std::string LEFT_DOES_NOT_EXIST =
        "The given Left does not have any mapped values";
    const std::string MORE_THAN_ONE_RIGHT =
        "There exists more than 1 Right value mapped to given Left";
};
