#pragma once

#include <algorithm>
#include <map>
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
